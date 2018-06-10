#include "ros_grabber.h"

using namespace cv;
using namespace std;

ROSGrabber::ROSGrabber(std::string i_scope) : it_(node_handle_) {
    image_sub_ = it_.subscribe(i_scope, 1, &ROSGrabber::imageCallback, this);
    frame_nr = -1;
    pyr = 0;
    ROS_DEBUG(">>> ROS grabber init done");
    ROS_INFO(">>> ROS grabber RGB %s", i_scope.c_str());
}

ROSGrabber::~ROSGrabber() { }

void ROSGrabber::imageCallback(const sensor_msgs::ImageConstPtr &msg) {
    ROS_DEBUG("Grabber callback called");
    cv_bridge::CvImagePtr cv_ptr;
    try {
        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception &e) {
        ROS_ERROR("E >>> CV_BRIDGE exception: %s", e.what());
        return;
    }

    mtx.lock();
    frame_time = msg->header.stamp;
    frame_nr = (int)msg->header.seq;
    frame_id = msg->header.frame_id;
    source_frame = cv_ptr->image;
    if (pyr > 0) {
        cv::pyrUp(source_frame, output_frame, cv::Size(source_frame.cols*2, source_frame.rows*2));
    } else {
        output_frame = source_frame;
    }
    mtx.unlock();
}

void ROSGrabber::getImage(cv::Mat *mat) {
    mtx.lock();
    *mat = output_frame;
    mtx.unlock();
}

void ROSGrabber::setPyr(bool _pyr) {
    pyr = _pyr;
}

ros::Time ROSGrabber::getTimestamp() {
    return frame_time;
}

int ROSGrabber::getLastFrameNr() {
    return frame_nr;
}