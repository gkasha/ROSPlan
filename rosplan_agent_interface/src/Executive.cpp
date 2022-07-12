#include <rosplan_agent_interface/Executive.h>


namespace KCL_rosplan {

    Executive::Executive(ros::NodeHandle &nh, std::string configurator_topic, std::string dispatch_topic) {
        node_handle_ = &nh;
        configurator_topic_ = configurator_topic;
        dispatch_topic_ = dispatch_topic;
        dispatch_pub_ = node_handle_->advertise<rosplan_dispatch_msgs::CompletePlan>(dispatch_topic, 1);
    }
    Executive::~Executive() {

    }

    void callService(ros::ServiceClient client, const rosplan_dispatch_msgs::CompletePlan plan) {
        // Now want to dispatch plan
        
    }
    /*
     * We have the Executive first receive the plan, then send
     * it to the dispatcher, because we may need to do some
     * preprocessing of the plan
     */
    void Executive::planCallback(const rosplan_dispatch_msgs::CompletePlan plan) {
        ROS_INFO("RECEIVED PLAN TO DISPATCH");
        dispatch_pub_.publish(plan);
        ros::ServiceClient client = node_handle_->serviceClient<rosplan_dispatch_msgs::DispatchService>("/rosplan_plan_dispatcher/dispatch_plan");
        
        rosplan_dispatch_msgs::DispatchService srv;

        if (client.call(srv)) {
            ROS_INFO("Dispatcher was called");
        } else {
            ROS_INFO("Failed to call dispatcher service");
            return;
        }
        
    }

    void Executive::configureCallback(const rosplan_dispatch_msgs::ConfigureReq msg) {
        ROS_INFO("RECEIVED CONFIGURE REQUEST");
        std::string plan_topic = msg.plan_topic;
        std::vector<std::string> goals;

        for (std::string g : msg.goals) {
            goals.push_back(g);
            ROS_INFO("GOAL: %s", g.c_str());
        }
        
        // Received plan topic, now want to invoke plan parser
        rosplan_dispatch_msgs::ParsingService srv;
        srv.request.plan_path = plan_topic;

        ros::ServiceClient client = node_handle_->serviceClient<rosplan_dispatch_msgs::ParsingService>("/rosplan_parsing_interface/parse_plan_from_file");
        if (client.call(srv)) {
            ROS_INFO("Parser was called");
        } else {
            ROS_INFO("Failed to call parsing service");
            return;
        }

        ros::MultiThreadedSpinner spinner(4);
        // Get plan from planner
        ros::Subscriber sub = node_handle_->subscribe("/rosplan_parsing_interface/complete_plan", 1, &KCL_rosplan::Executive::planCallback, this);
        
        // Monitor execution for goals
        ros::Rate loop_rate(5);
        while (ros::ok()) {
            // Want to query the KB for the opportunity list
            for (std::string g : goals) {
                // Construct query
                rosplan_knowledge_msgs::GetAttributeService k_srv;
                k_srv.request.predicate_name = g;
                ros::ServiceClient k_client = node_handle_->serviceClient<rosplan_knowledge_msgs::GetAttributeService>("/rosplan_knowledge_base/state/propositions");
                if (k_client.call(k_srv)) {
                    ROS_INFO("GET ATTRIBUTE SERVICE CALLED");
                    for (auto attribute : k_srv.response.attributes) {
                        if (attribute.is_negative == false) {
                            ROS_INFO("\n ---------------\nGOAL FOUND\n ---------------\n");
                            return;
                        }
                    }
                } else {
                    // ROS_INFO("GET ATTRIBUTE SERVICE FAILED");
                }
            }
            ros::spinOnce();
        }
    }
}

    int main(int argc, char **argv) {
        ros::init(argc, argv, "Executive");
        ros::NodeHandle nh("~");


        std::string configurator_topic, dispatch_topic;
        nh.getParam("dispatch_topic", dispatch_topic);
        nh.getParam("configurator_topic", configurator_topic);
        KCL_rosplan::Executive exec(nh, configurator_topic, dispatch_topic);

        ros::Subscriber sub = nh.subscribe(configurator_topic, 1000, &KCL_rosplan::Executive::configureCallback, &exec);
        ROS_INFO("KCL: (%s) Ready to receive", ros::this_node::getName().c_str());
        ros::spin();
        return 0;
    }
