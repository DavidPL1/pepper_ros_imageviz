#include <chrono>
#include <thread>
#include <signal.h>

#include "main.h"
#include "opencv2/imgproc/imgproc.hpp"

// ROS
#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>

using namespace cv;

void inthand(int signum) {
    ROS_INFO(">> Shutting down...");
    Main::stop = true;
}

void Main::doWork() {
    signal(SIGINT, inthand);

    Mat imgTF, imgTracking;

    Mat sideBySide = Mat::zeros(240, 640, CV_8UC3);
    Mat lastTracking = Mat::zeros(240, 320, CV_8UC3);
    Mat lastTF = Mat::zeros(240, 320, CV_8UC3);
    // first spin, to get callback in the queue
    bool initing = true;
    cvNamedWindow("Pepper Image Viz");

    while (initing) {
        ros::spinOnce();
        ros_grabber_tf->getImage(&imgTF);
        ros_grabber_tracking->getImage(&imgTracking);
    
        if (imgTF.rows*imgTF.cols > 0) {
            initing = false;
            if (imgTF.channels() == 1)
                cv::cvtColor(imgTF, imgTF, cv::COLOR_GRAY2BGR);
        }

        if (imgTracking.rows * imgTracking.cols > 0) {
            initing = false;
            if (imgTracking.channels() == 1)
                cv::cvtColor(imgTracking, imgTracking, cv::COLOR_GRAY2BGR);
        }

        if (stop)
            break;
    }
    
    // imAcqHasMoreFrames(imAcq)
    while (stop == false) {
        // Loop spinner
        ros::spinOnce();

        ros_grabber_tf->getImage(&imgTF);
        ros_grabber_tracking->getImage(&imgTracking);

        if (imgTracking.rows * imgTracking.cols > 0) {
            if (imgTracking.channels() == 1)
                cv::cvtColor(imgTracking, imgTracking, cv::COLOR_GRAY2BGR);
            lastTracking = imgTracking;
        }
        
        if (imgTF.rows*imgTF.cols > 0) {
            if (imgTF.channels() == 1)
                cv::cvtColor(imgTF, imgTF, cv::COLOR_GRAY2BGR);
            lastTF = imgTF;
            cv::resize(lastTF, lastTF, imgTracking.size(), 0, 0, CV_INTER_LINEAR);
        }

        cv::hconcat(lastTF, lastTracking, sideBySide);

        imshow("Pepper Image Viz", sideBySide);
        if (cv::waitKey(10) == 27)
            stop = true;

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    
        if(stop) { break; }
    }

    delete ros_grabber_tf;
    delete ros_grabber_tracking;
}
