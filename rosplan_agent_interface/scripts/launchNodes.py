import roslaunch, sys, os, rospy



def main(args):
    launchFile = args[0]
    configurator = args[1]
    knowledgeBase = args[2]
    executive = args[3]
    dispatcher = args[4]
    domainFile = args[5]
    problemFile = args[6]

    uuid = roslaunch.rlutil.get_or_generate_uuid(None, False)
    roslaunch.configure_logging(uuid)

    cli_args1 = [launchFile, 'configurator_node:='+configurator, 'knowledge_node:='+knowledgeBase, 'executive_node:='+executive, 'dispatcher_node:='+dispatcher, 'domain_path:=' + domainFile, 'problem_path:=' + problemFile]
    roslaunch_file1 = roslaunch.rlutil.resolve_launch_arguments(cli_args1)
    roslaunch_args1 = cli_args1[2:]

    launch_files = [(roslaunch_file1, roslaunch_args1)]
    parent = roslaunch.parent.ROSLaunchParent(uuid, launch_files)

    parent.start()
if __name__ == "__main__":
    main(sys.argv[1:])
