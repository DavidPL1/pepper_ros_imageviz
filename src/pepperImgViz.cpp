/*  Copyright 2011 AIT Austrian Institute of Technology
*
*   This file is part of OpenTLD.
*
*   OpenTLD is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   OpenTLD is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with OpenTLD.  If not, see <http://www.gnu.org/licenses/>.
*
*/

/**
  * @author Georg Nebehay
*/

#include "main.h"

int main(int argc, char **argv)
{
    ros::init(argc, argv, "pepper_ros_image_viz", ros::init_options::AnonymousName);
    ros::NodeHandle* n;

    Main *main = new Main();

    std::string tracking_topic = "/cftld/detection";
    std::string tf_topic = "/tf_pose/result";
    
    ROSGrabber *ros_grabber_tracking = new ROSGrabber(tracking_topic);
    ROSGrabber *ros_grabber_tf = new ROSGrabber(tf_topic);
    
    main->ros_grabber_tf = ros_grabber_tf;
    main->ros_grabber_tracking = ros_grabber_tracking;
    main->doWork();

    delete main;
    main = NULL;

    return EXIT_SUCCESS;
}
