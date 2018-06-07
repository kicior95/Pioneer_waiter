cmake_minimum_required(VERSION 2.8.3)
project(pioneer_waiter)

# Catkin components
find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  sensor_msgs
  geometry_msgs
  tf
  image_transport
  cv_bridge
  message_filters
)

# OpenCV
find_package(OpenCV REQUIRED)
if (OpenCV_FOUND)
    message("-- OpenCV ${OpenCV_VERSION} was found")
    message("-- OpenCV ${OpenCV_INCLUDE_DIRS} was found")
    include_directories(${OpenCV_INCLUDE_DIRS})
else(OpenCV_FOUND)
    message(FATAL_ERROR "OpenCV was not found")
endif(OpenCV_FOUND)

# Catkin
catkin_package(
#  INCLUDE_DIRS include
  CATKIN_DEPENDS roscpp std_msgs sensor_msgs geometry_msgs tf message_filters
#  DEPENDS system_lib
)



# Includes
include_directories(
  ${catkin_INCLUDE_DIRS}
)

# Exe & linking (pioneer_control)
add_executable(pioneer_control src/pioneer_control.cpp)
target_link_libraries(pioneer_control ${catkin_LIBRARIES})

# Exe & linking (pioneer_line_tracing)
add_executable(pioneer_line_tracing src/pioneer_line_tracing.cpp)
target_link_libraries(pioneer_line_tracing ${catkin_LIBRARIES} ${OpenCV_LIBS})

# Exe & linking (pioneer_qrcode)
add_executable(pioneer_qrcode src/pioneer_qrcode.cpp)
target_link_libraries(pioneer_qrcode zbar ${catkin_LIBRARIES} ${OpenCV_LIBS} ${ZBAR_LIBRARIES})

# Exe & linking (pioneer_object_detector)
add_executable(pioneer_object_detector src/pioneer_object_detector.cpp)
target_link_libraries(pioneer_object_detector ${catkin_LIBRARIES})

# Exe & linking (pioneer_route_planning)
add_executable(pioneer_route_planning src/pioneer_route_planning.cpp)
target_link_libraries(pioneer_route_planning ${catkin_LIBRARIES})
