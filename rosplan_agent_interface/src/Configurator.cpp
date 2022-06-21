#include <rosplan_agent_interface/Configurator.h>

namespace KCL_rosplan {

    Configurator::Configurator(ros::NodeHandle& nh, 
                               std::string dp, 
                               std::string ptd) {
        node_handle_ = &nh;
        domain_path_ = dp;
        problem_template_dir_ = ptd;
    }

    Configurator::~Configurator() {

    }

    void Configurator::getCurrentState() {

    }

    void Configurator::genProblemFile() {

    }

    void Configurator::configure() {

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
        std::string name = "rosplan_agent_" + std::to_string(start.count());
        
        ros::init(argc, argv, name);
        ros::NodeHandle nh("~");

        // Get Configurator Info
        std::string domain_path, ptd_path;
        nh.param("domain_path", domain_path, domain_path);
        nh.param("problem_template_dir_path", ptd_path, ptd_path);

        KCL_rosplan::Configurator config(nh, domain_path, ptd_path);
        
        // Subscribe to receive goals
        std::string goalRequestTopic = "goalRequest";
        nh.getParam("goal_request_topic", goalRequestTopic);
        ros::Subscriber goal_sub = nh.subscribe(goalRequestTopic, 1000, &KCL_rosplan::Configurator::goalRequestCallback, &config);

        ROS_INFO("KCL: (%s) Ready to receive", ros::this_node::getName().c_str());
        ros::spin();
        
        return 0;
    }
