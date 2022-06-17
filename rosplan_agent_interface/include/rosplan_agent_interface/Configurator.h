/*
 * This file describes the Configurator component for an
 * abstract agent
 */

#ifndef CMU_CONFIGURATOR_H_
#define CMU_CONFIGURATOR_H_

#include "ros/ros.h"

namespace KCL_rosplan
{
    class Configurator
    {
    private:
        std::string domain_file_;
        std::string problem_template_dir_;

        void getCurrentState();
        void genProblemFile();
        void configureExec();

    public:
        Configurator(ros::NodeHandle& nh);
        ~Configurator();
        
        void execute();
    };
}

#endif
