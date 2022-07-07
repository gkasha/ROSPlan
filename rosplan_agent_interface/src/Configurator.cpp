#include <rosplan_agent_interface/Configurator.h>

namespace KCL_rosplan {

    Configurator::Configurator(ros::NodeHandle& nh, std::string pddl_files,
                               std::string scripts, std::string planner) {
        node_handle_ = &nh;
        pddl_files_ = pddl_files;
        scripts_ = scripts;
        planner_ = planner;
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
    std::string Configurator::genProblemFile() {

        // Finally, call transform.py to generate problem file
        // This will query the KB based on the fluents file, and then fill in
        // information from both the fixed and fluents
        std::string goal = "leak-alert";
        std::string command = "python3 " + scripts_ + "transform.py " + pddl_files_ + " " + goal;

        return exec(command.c_str());
    }

    bool Configurator::configure(rosplan_dispatch_msgs::ConfigureService::Request &req,
                                 rosplan_dispatch_msgs::ConfigureService::Response &res) {
        ROS_INFO("KCL: (%s) RECEIVED CALL TO CONFIGURE", ros::this_node::getName().c_str());
        ROS_INFO("\tPDDL Files: %s",  pddl_files_.c_str());
        ROS_INFO("\tScripts: %s",  scripts_.c_str());

        
        // Get domain file and generate problem file
        std::vector<std::string> probresp = split(genProblemFile(), ' ');
        std::string domain_file = probresp[0];
        std::string problem_file = probresp[1];
        problem_file.erase(std::remove(problem_file.begin(), problem_file.end(), '\n'), problem_file.end());
        
        // Generate plan
        rosplan_dispatch_msgs::PlanningService srv;
        srv.request.domain_path = domain_file;
        srv.request.problem_path = problem_file;
        srv.request.data_path = pddl_files_ + "plans/";
        srv.request.planner_command = "timeout 10 " + planner_ + " DOMAIN PROBLEM";
        srv.request.use_problem_topic = false;

        ros::ServiceClient client = node_handle_->serviceClient<rosplan_dispatch_msgs::PlanningService>("/rosplan_planner_interface/planning_server_params");
        if (client.call(srv)) {
            ROS_INFO("Plan found: %d", srv.response.plan_found);
        } else {
            ROS_ERROR("Failed to call planning service");
            return false;
        }
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
        std::string pddl_files, scripts, planner;
        nh.getParam("pddl_file_path", pddl_files);
        nh.getParam("scripts_path", scripts);
        nh.getParam("planner", planner);
        KCL_rosplan::Configurator config(nh, pddl_files, scripts, planner);
        
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
