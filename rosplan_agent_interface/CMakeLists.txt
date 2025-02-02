cmake_minimum_required(VERSION 3.0.2)
project(rosplan_agent_interface)
add_compile_options(-std=c++11)
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  actionlib
  std_msgs
  diagnostic_msgs
  rosplan_dispatch_msgs
  rosplan_dependencies
  roslib
  rostest
  message_generation
)

# add_service_files(
#   FILES
#   ConfigureService.srv
# )

# generate_messages(
#    DEPENDENCIES
#    actionlib_msgs
#    diagnostic_msgs
#    std_msgs
# )


catkin_package(
  INCLUDE_DIRS include ${catkin_INCLUDE_DIRS}
  LIBRARIES rosplan_action_interface
  CATKIN_DEPENDS roscpp rospy actionlib std_msgs diagnostic_msgs
                 rosplan_dispatch_msgs roslib rostest
  DEPENDS
)
include_directories(include)
include_directories(${catkin_INCLUDE_DIRS})

find_package(FLEX REQUIRED)

add_definitions(-Wno-deprecated-declarations)

add_executable(configurator src/Configurator.cpp)
add_executable(executive src/Executive.cpp)

add_dependencies(configurator ${catkin_EXPORTED_TARGETS})
add_dependencies(executive ${catkin_EXPORTED_TARGETS})

target_link_libraries(configurator ${catkin_LIBRARIES})
target_link_libraries(executive ${catkin_LIBRARIES})



