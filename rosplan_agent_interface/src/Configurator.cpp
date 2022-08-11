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
        running_ = false;
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
     * This also intializes the KB, dispatcher, and Executive if they do
     * not exist
     */
    std::string Configurator::genProblemFile(std::string goal) {
        ROS_INFO("GENERATING PROBLEM FILE");
        // Finally, call transform.py to generate problem file
        // This will query the KB based on the fluents file, and then fill in
        // information from both the fixed and fluents
        std::string command = "python " + scripts_ + "transform.py " + pddl_files_ + " " + goal;
        running_ = true;
        return exec(command.c_str());
    }

    std::string Configurator::genPlanFile(std::string domain_file, std::string problem_file) {
        ROS_INFO("GENERATING PLAN FILE");
        // Invoke planner on problem and domain file
        std::string temp = std::regex_replace(planner_, std::regex("DOMAIN"), domain_file);
        std::string command = std::regex_replace(temp, std::regex("PROBLEM"), problem_file);
        exec(command.c_str());

        // Modify plan for parser
        std::string plan_file = pddl_files_ + "/plans/plan.pddl";
        command = "python3 " + scripts_ + "formatPlan.py " + plan_file;
        exec(command.c_str());
        return plan_file;
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
        std::vector<std::string> opportunities(probresp.begin() + 2, probresp.end());
        problem_file.erase(std::remove(problem_file.begin(), problem_file.end(), '\n'), problem_file.end());

        // Generate plan
        std::string plan = genPlanFile(domain_file, problem_file);
        
        // Publish plan, send it to Executive
        ROS_INFO("PUBLISHING PLAN");
        rosplan_dispatch_msgs::ConfigureReq msg;
        msg.plan_topic = plan;//"/rosplan_planner_interface/planner_output";
        for (std::string o : opportunities) {
            o.erase(std::remove(o.begin(), o.end(), '\n'), o.end());
            msg.opportunities.push_back(o);
        }

        configure_pub_.publish(msg);
        ROS_INFO("SENT MESSAGE TO EXECUTIVE: %s", msg.plan_topic.c_str());
        return true;
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
        
        ROS_INFO("KCL: (%s) Ready to receive", ros::this_node::getName().c_str());
        
        // Advertise service
        ros::ServiceServer service = nh.advertiseService("configure", &KCL_rosplan::Configurator::configure, &config);
        ros::spin();
        
        return 0;
    }
