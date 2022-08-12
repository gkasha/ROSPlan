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
#include <regex>

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
        ros::Publisher configure_pub_;
        ros::NodeHandle* node_handle_;

        std::string pddl_files_;
        std::string scripts_;
        std::string planner_command_;
        std::string output_;

        bool running_;

        void getCurrentState();
        std::vector<std::string> genProblemFile(std::string goal);
        std::string genPlanFile(std::string domain_file, std::string problem_file);

    public:
        Configurator(ros::NodeHandle& nh, std::string pddl_files, std::string scripts, std::string planner_command, std::string output);
        ~Configurator();
        
        bool configure(rosplan_dispatch_msgs::ConfigureService::Request &req,
                       rosplan_dispatch_msgs::ConfigureService::Response &res);
    };
}

#endif
