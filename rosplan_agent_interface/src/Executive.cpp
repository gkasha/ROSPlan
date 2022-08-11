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
            ROS_INFO("Dispatcher was called, success: %d, goal_achieved: %d", srv.response.success, srv.response.goal_achieved);
        } else {
            ROS_INFO("Failed to call dispatcher service");
            return;
        }
        
    }

    std::string Executive::monitorExecution(std::vector<std::string> opportunities) {
        ros::Rate loop_rate(5);

        while (ros::ok()) {
            // Want to query the KB for the opportunity list
            for (std::string o : opportunities) {
                // Construct query
                rosplan_knowledge_msgs::GetAttributeService k_srv;
                k_srv.request.predicate_name = o;
                ros::ServiceClient k_client = node_handle_->serviceClient<rosplan_knowledge_msgs::GetAttributeService>("/rosplan_knowledge_base/state/propositions");
                if (k_client.call(k_srv)) {
                    for (auto attribute : k_srv.response.attributes) {
                        if (attribute.is_negative == false) {
                            ROS_INFO("\n ---------------\nGOAL FOUND: %s\n ---------------\n", o.c_str());
                            // Cancel dispatch
                            std_srvs::Empty cancel_dispatch;
                            ros::ServiceClient cancel_client = node_handle_->serviceClient<std_srvs::Empty>("/rosplan_plan_dispatcher/cancel_dispatch");
                            if (cancel_client.call(cancel_dispatch)) {
                                ROS_INFO("DISPATCH CANCELLED");
                                return o;
                            } else {
                                ROS_INFO("Unable to cancel dispatch");
                                return "";
                            }
                        }
                    }
                }
            }
        }
    }

    void Executive::configureCallback(const rosplan_dispatch_msgs::ConfigureReq msg) {
        ROS_INFO("RECEIVED CONFIGURE REQUEST");
        std::string plan_topic = msg.plan_topic;
        std::vector<std::string> opportunities;

        for (std::string o : msg.opportunities) {
            if (o == "") continue;
            opportunities.push_back(o);
            ROS_INFO("Opportunity: %s", o.c_str());
        }
        
        // Received plan topic, now want to invoke plan parser
        rosplan_dispatch_msgs::ParsingService parse_srv;
        parse_srv.request.plan_path = plan_topic;

        ros::ServiceClient client = node_handle_->serviceClient<rosplan_dispatch_msgs::ParsingService>("/rosplan_parsing_interface/parse_plan_from_file");
        if (client.call(parse_srv)) {
            ROS_INFO("Parser was called on %s", plan_topic.c_str());
        } else {
            ROS_INFO("Failed to call parsing service");
            return;
        }

        // Handle dispatcher in separate thread
        ros::NodeHandle nh_dispatch;
        ros::CallbackQueue dispatch_queue;
        nh_dispatch.setCallbackQueue(&dispatch_queue);
        ros::AsyncSpinner spinner(2, &dispatch_queue);
        // Get plan from planner
        ros::Subscriber sub = nh_dispatch.subscribe("/rosplan_parsing_interface/complete_plan", 1, &KCL_rosplan::Executive::planCallback, this);
        
        // Monitor execution for opportunities
        spinner.start();
        std::string new_goal = monitorExecution(opportunities);
        spinner.stop();

        if (new_goal != "") {
            // Achieved a goal, now want to post new goal to Configurator
            client = node_handle_->serviceClient<rosplan_dispatch_msgs::ConfigureService>("/rosplan_configurator_interface/configure");
            rosplan_dispatch_msgs::ConfigureService configure_srv;
            configure_srv.request.goal = new_goal;
            if (client.call(configure_srv)) {
                ROS_INFO("POSTED NEW GOAL: %s", new_goal.c_str());

                rosplan_knowledge_msgs::KnowledgeUpdateService k_srv;
                k_srv.request.update_type = 2;
                k_srv.request.knowledge.knowledge_type = 1;
                k_srv.request.knowledge.attribute_name = new_goal;
                ros::ServiceClient client = node_handle_->serviceClient<rosplan_knowledge_msgs::KnowledgeUpdateService>("/rosplan_knowledge_base/update");
                if (client.call(k_srv)) {
                    ROS_INFO("Cleared goal from KB: %s", new_goal.c_str());
                } else {
                    ROS_INFO("Unable to clear goal from KB: %s", new_goal.c_str());
                }
                return;
            } else {
                ROS_INFO("UNABLE TO POST NEW GOAL: %s", new_goal.c_str());
                return;
            }
        } else {
            // Something unexpected happened, how should we handle it?
        }
        return;
        
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
