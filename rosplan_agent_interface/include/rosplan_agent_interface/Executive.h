/*
 * This file describes the Executive component for an
 * abstract agent
 */

#ifndef CMU_EXECUTIVE_H_
#define CMU_EXECUTIVE_H_

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
#include "rosplan_dispatch_msgs/CompletePlan.h"
#include "rosplan_dispatch_msgs/EsterelPlan.h"
#include "rosplan_dispatch_msgs/ParsingService.h"
#include "rosplan_dispatch_msgs/DispatchService.h"

namespace KCL_rosplan
{
    class Executive
    {
    private:
        ros::NodeHandle* node_handle_;
        ros::Publisher dispatch_pub_;
        std::string configurator_topic_;
        std::string dispatch_topic_;
        
    public:
        Executive(ros::NodeHandle& nh, std::string configurator_topic, std::string dispatch_topic);
        ~Executive();

        void planCallback(const rosplan_dispatch_msgs::EsterelPlan plan);
        void configureCallback(const rosplan_dispatch_msgs::ConfigureReq msg);

        
    };
}

#endif
