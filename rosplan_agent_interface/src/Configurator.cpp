#include <rosplan_agent_interface/Configurator.h>

namespace KCL_rosplan {

    Configurator::Configurator(ros::NodeHandle& nh, 
                               std::string dp, 
                               std::string ptd,
                               std::string scripts) {
        node_handle_ = &nh;
        domain_path_ = dp;
        problem_template_dir_ = ptd;
        scripts_ = scripts;
    }

    Configurator::~Configurator() {

    }

    void Configurator::getCurrentState() {

    }

    void Configurator::genProblemFile() {

    }

    bool Configurator::configure(rosplan_dispatch_msgs::ConfigureService::Request &req,
                                 rosplan_dispatch_msgs::ConfigureService::Response &res) {
        ROS_INFO("KCL: (%s) RECEIVED CALL TO CONFIGURE", ros::this_node::getName().c_str());
        ROS_INFO("\tDomain: %s",  domain_path_.c_str());
        ROS_INFO("\tPTD: %s",  problem_template_dir_.c_str());
        ROS_INFO("\tScripts: %s",  scripts_.c_str());

        std::string data = problem_template_dir_ + "../data.json";
        std::string problem_in = problem_template_dir_ + "../problem.pddl";
        std::string problem_out = problem_template_dir_ + "../problem_out.pddl";
        std::string command = "python3 " + scripts_ + "transform.py " + data + " " 
                                + problem_in + " " + problem_out;

        std::system(command.c_str());

        return true;
    }

    void Configurator::goalRequestCallback(const rosplan_dispatch_msgs::ActionDispatch msg) {
        // ROS_INFO("KCL: (%s) Goal received.", ros::this_node::getName().c_str());
        
        // mission_start_time = ros::WallTime::now.toSec();
        // current_goal_ = msg;

        // Configurator::configure();
    }

} // close namespace

    int main(int argc, char **argv) {
        auto start = std::chrono::system_clock::now().time_since_epoch();
        std::string name = "rosplan_agent_configurator_" + std::to_string(start.count());
        
        ros::init(argc, argv, name);
        ros::NodeHandle nh("~");

        // Get Configurator Info
        std::string domain_path, ptd_path, scripts;
        nh.getParam("domain_path", domain_path);
        nh.getParam("problem_template_dir_path", ptd_path);
        nh.getParam("scripts_path", scripts);

        KCL_rosplan::Configurator config(nh, domain_path, ptd_path, scripts);
        
        // Subscribe to receive goals
        std::string goalRequestTopic = "goalRequest";
        nh.getParam("goal_request_topic", goalRequestTopic);
        ros::Subscriber goal_sub = nh.subscribe(goalRequestTopic, 1000, &KCL_rosplan::Configurator::goalRequestCallback, &config);

        ROS_INFO("KCL: (%s) Ready to receive", ros::this_node::getName().c_str());
        
        // Advertise service
        ros::ServiceServer service = nh.advertiseService("configure", &KCL_rosplan::Configurator::configure, &config);
        ros::spin();
        
        return 0;
    }
