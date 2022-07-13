#include <rosplan_agent_interface/Configurator.h>

namespace KCL_rosplan {

    Configurator::Configurator(ros::NodeHandle& nh, std::string pddl_files,
                               std::string scripts, std::string planner, std::string output) {
        node_handle_ = &nh;
        configure_pub_ = node_handle_->advertise<rosplan_dispatch_msgs::ConfigureReq>(output, 1);
        pddl_files_ = pddl_files;
        scripts_ = scripts;
        planner_ = planner;
        output_ = output;
    }

    Configurator::~Configurator() {

    }

    // Helper for system calls
    std::string exec(const char* cmd) {
        std::array<char, 128> buffer;
        std::string result;
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
        if (!pipe) {
            throw std::runtime_error("popen() failed!");
        }
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
        return result;
    }

    // Helper for splitting string
    template <typename Out>
    void split(const std::string &s, char delim, Out result) {
        std::istringstream iss(s);
        std::string item;
        while (std::getline(iss, item, delim)) {
            *result++ = item;
        }
    }

    std::vector<std::string> split(const std::string &s, char delim) {
        std::vector<std::string> elems;
        split(s, delim, std::back_inserter(elems));
        return elems;
    }


    /*
     * Take goal, looks at problem templates to find best match
     * Transforms problem template into pddl problem file
     */
    std::string Configurator::genProblemFile(std::string goal) {

        // Finally, call transform.py to generate problem file
        // This will query the KB based on the fluents file, and then fill in
        // information from both the fixed and fluents
        std::string command = "python3 " + scripts_ + "transform.py " + pddl_files_ + " " + goal;

        return exec(command.c_str());
    }


    bool Configurator::configure(rosplan_dispatch_msgs::ConfigureService::Request &req,
                                 rosplan_dispatch_msgs::ConfigureService::Response &res) {
        ROS_INFO("KCL: (%s) RECEIVED CALL TO CONFIGURE", ros::this_node::getName().c_str());
        ROS_INFO("\tPDDL Files: %s",  pddl_files_.c_str());
        ROS_INFO("\tScripts: %s",  scripts_.c_str());

        
        // Get domain file and generate problem file
        std::vector<std::string> probresp = split(genProblemFile(req.goal), ' ');
        std::string domain_file = probresp[0];
        std::string problem_file = probresp[1];
        std::vector<std::string> goals(probresp.begin() + 2, probresp.end());
        problem_file.erase(std::remove(problem_file.begin(), problem_file.end(), '\n'), problem_file.end());
        
        // Initialize KB if not already
        ros::ServiceClient client = node_handle_->serviceClient<std_srvs::Empty>("/rosplan_knowledge_base/clear");
        std_srvs::Emtpy clear_srv;
        if (client.call(clear_srv)) {
            ROS_INFO("CLEARED KB");
        } else{
            ROS_INFO("UNABLE TO CLEAR KB");
        }

        // Generate plan
        rosplan_dispatch_msgs::PlanningService srv;
        srv.request.domain_path = domain_file;
        srv.request.problem_path = problem_file;
        srv.request.data_path = pddl_files_ + "plans/";
        srv.request.planner_command = "timeout 10 " + planner_ + " DOMAIN PROBLEM";
        srv.request.use_problem_topic = false;

        client = node_handle_->serviceClient<rosplan_dispatch_msgs::PlanningService>("/rosplan_planner_interface/planning_server_params");
        if (client.call(srv)) {
            ROS_INFO("Plan found: %d", srv.response.plan_found);
        } else {
            ROS_ERROR("Failed to call planning service");
            return false;
        }

        // Publish plan, send it to Executive
        rosplan_dispatch_msgs::ConfigureReq msg;
        msg.plan_topic = srv.request.data_path + "plan.pddl";//"/rosplan_planner_interface/planner_output";
        for (std::string g : goals) {
            g.erase(std::remove(g.begin(), g.end(), '\n'), g.end());
            msg.goals.push_back(g);
        }

        configure_pub_.publish(msg);
        ROS_INFO("SENT MESSAGE TO EXECUTIVE: %s", msg.plan_topic.c_str());
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
        std::string pddl_files, scripts, planner, output;
        nh.getParam("pddl_file_path", pddl_files);
        nh.getParam("scripts_path", scripts);
        nh.getParam("planner", planner);
        nh.getParam("output", output);

        KCL_rosplan::Configurator config(nh, pddl_files, scripts, planner, output);
        
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
