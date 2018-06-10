
#pragma once

// ROS
#include "ros_grabber.h"

enum Retval
{
    PROGRAM_EXIT = 0,
    SUCCESS = 1
};

class Main
{
public:
    static bool stop;
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