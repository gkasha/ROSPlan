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

#include "ros/ros.h"
#include "rosplan_dispatch_msgs/ActionDispatch.h"
#include "rosplan_dispatch_msgs/ConfigureService.h"

namespace KCL_rosplan
{
    class Configurator
    {
    private:
        std::string domain_path_;
        std::string problem_template_dir_;
        std::string scripts_;

        double mission_start_time_;
        rosplan_dispatch_msgs::ActionDispatch current_goal_;

        ros::NodeHandle* node_handle_;

        void getCurrentState();
        void genProblemFile();


    public:
        Configurator(ros::NodeHandle& nh, std::string dp, std::string ptd, std::string scripts);
        ~Configurator();
        
        void goalRequestCallback(const rosplan_dispatch_msgs::ActionDispatch msg);
        bool configure(rosplan_dispatch_msgs::ConfigureService::Request &req,
                       rosplan_dispatch_msgs::ConfigureService::Response &res);
    };
}

#endif
