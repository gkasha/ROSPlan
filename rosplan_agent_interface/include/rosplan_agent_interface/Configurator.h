/*
 * This file describes the Configurator component for an
 * abstract agent
 * This is a temp line
 */

#ifndef CMU_CONFIGURATOR_H_
#define CMU_CONFIGURATOR_H_

#include <iostream>
#include <string>
#include <chrono>
#include <array>
#include <vector>
#include <sstream>

#include "ros/ros.h"
#include "rosplan_dispatch_msgs/ActionDispatch.h"
#include "rosplan_dispatch_msgs/ConfigureService.h"
#include "rosplan_dispatch_msgs/PlanningService.h"
#include "rosplan_dispatch_msgs/ConfigureReq.h"

namespace KCL_rosplan
{
    class Configurator
    {
    private:
        std::string pddl_files_;
        std::string scripts_;
        std::string planner_;
        std::string output_;

        ros::Publisher configure_pub_;

        double mission_start_time_;
        rosplan_dispatch_msgs::ActionDispatch current_goal_;

        ros::NodeHandle* node_handle_;

        void getCurrentState();
        std::string genProblemFile();


    public:
        Configurator(ros::NodeHandle& nh, std::string pddl_files, std::string scripts, std::string planner, std::string output);
        ~Configurator();
        
        void goalRequestCallback(const rosplan_dispatch_msgs::ActionDispatch msg);
        bool configure(rosplan_dispatch_msgs::ConfigureService::Request &req,
                       rosplan_dispatch_msgs::ConfigureService::Response &res);
    };
}

#endif
