cmake_minimum_required(VERSION 2.8.3)
add_definitions(-std=c++11)
project(pepper_ros_image_viz)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-narrowing")
SET(OpenCV_MIN_VERSION "3.3.1")

## Get all source files
FILE(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
    roscpp
    std_msgs
    visualization_msgs
    image_transport
    cv_bridge
)
find_package(Boost REQUIRED COMPONENTS thread)
find_package(OpenCV REQUIRED)

###################################
## catkin specific configuration ##
###################################
catkin_package(
  INCLUDE_DIRS include
  CATKIN_DEPENDS roscpp std_msgs visualization_msgs image_transport cv_bridge
)

###########
## Build ##
########### 

IF(OpenCV_FOUND)
        IF(${OpenCV_VERSION} VERSION_LESS ${OpenCV_MIN_VERSION} AND (SUPPORT_VIDEO OR SUPPORT_IMAGE) )
                MESSAGE(FATAL_ERROR "OpenCV version (${OpenCV_VERSION} is present, while ${OpenCV_MIN_VERSION}+ is required")
        ENDIF()

        MESSAGE(STATUS "OpenCV_VERSION: " ${OpenCV_VERSION})
        # MESSAGE(STATUS "OpenCV_INCLUDE_DIRS: " ${OpenCV_INCLUDE_DIRS})
        # MESSAGE(STATUS "OpenCV_LIBRARIES: " ${OpenCV_LIBS})
ELSE()
        MESSAGE(FATAL_ERROR "OpenCV NOT FOUND")
ENDIF()

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
  ${OPENCV_INCLUDE_DIRS}
)

add_executable(pepper_ros_image_viz
  src/pepperImgViz.cpp
  ${SOURCES}
)

add_dependencies(pepper_ros_image_viz ${catkin_EXPORTED_TARGETS})

target_link_libraries(pepper_ros_image_viz
  ${catkin_LIBRARIES}
  ${Boost_LIBRARIES}
  ${OpenCV_LIBS}
)

#############
## Install ##
#############

install(TARGETS ${PROJECT_NAME}
  EXPORT  ${PROJECT_NAME}Targets
  PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}/${PROJECT_NAME}"
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY include/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
  FILES_MATCHING PATTERN "*.h"
  PATTERN ".git" EXCLUDE
)

install(DIRECTORY launch
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)