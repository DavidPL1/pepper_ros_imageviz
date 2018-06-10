
#pragma once

// ROS
#include "ros_grabber.h"

enum Retval
{
    PROGRAM_EXIT = 0,
    SUCCESS = 1
};


    static bool stop = false;

class Main
{
public:
    ROSGrabber *ros_grabber_tracking;
    ROSGrabber *ros_grabber_tf;

    bool show_tf;
    bool show_tracking;

    Main() {
        show_tf = false;
        show_tracking = false;
    }

    ~Main() {}

    void doWork();
};