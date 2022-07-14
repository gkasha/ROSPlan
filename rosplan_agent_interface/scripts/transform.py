import sys, os, datetime, json, rospy, roslaunch
from jinja2 import Template, Environment
from typing import Dict, List

from rosplan_knowledge_msgs.srv import *
from rosplan_knowledge_msgs.msg import *

# read the template from the standard input
# input = "".join(sys.stdin.readlines())
def callPropService(predicate_name, unknown):
    # print "Waiting for service"
    rospy.wait_for_service('/rosplan_knowledge_base/state/propositions')
    try:
        # print "Calling Service"
        query_proxy = rospy.ServiceProxy('rosplan_knowledge_base/state/propositions', GetAttributeService)
        resp1 = query_proxy(predicate_name)
        return resp1
    except (rospy.ServiceException, e):
        # print "Service call failed: %s"%e
        return None

def tif_filter(time, value, *function_name):
    """ Creates time-initial fluent if time>0, or plain initialization otherwise """
    assignment = "(= ({}) {})".format(' '.join(function_name), value)
    return "(at {} {})".format(time, assignment) if time > 0\
        else assignment

def load_template_from_string(template_text):
    jinja2_env = Environment(trim_blocks = False, lstrip_blocks = False)
    # jinja2_env.filters['tif'] = tif_filter

    return jinja2_env.from_string(template_text)

def remove_doubled_whitespace(text):
    lines = text.splitlines(True)

    output = ''
    was_white_space = False

    for line in lines:
        is_white_space = len(line.strip()) == 0
        if not is_white_space:
            output = output + line
        elif not was_white_space:
            output = output + line
        
        was_white_space = is_white_space

    return output

def processFluents(file_in):
    json_file = open(file_in, 'r')
    data = json.load(json_file)
    json_file.close()
    
    fluents = []

    for x in data:
        for pred in data[x]:
            # Construct query, get result
            predicate_name = pred['predicate_name']
            keys = pred['key']
            val = None

            rep = callPropService(predicate_name)
            
            for elt in rep.attributes:
                match = True
                n = len(elt.values)
                unknowns = False
                if (pred == "unknowns"):
                    n -= 1
                    unknowns = True

                # See if predicate matches
                for i in range(n):
                    if elt.values[i].value != keys[i]:
                        match = False

                if match:
                    s = "("
                    neg = False
                    if unknowns:
                        val = elt.values[-1].value
                        s += predicate_name + " " + " ".join(keys) + " " + val + ")"
                    else:
                        s += predicate_name + " " + " ".join(keys) + ")"

                    if elt.is_negative:
                        s = "not (" + s + ")"
                    fluents.append(s)


    return fluents

# def processStatics(file_in):
#     f = open(file_in, 'r')
#     data = f.read().split("\n")
#     return data

def transform(fluents_in, template_string):
    """ transforms the template; this function may be called from other Python code, e.g. Flask web service """

    fluents = processFluents(fluents_in)
    # statics = processStatics(statics_in)
    template = load_template_from_string(template_string)
    transformed = template.render(fluents=fluents)
    compacted = remove_doubled_whitespace(transformed)
    return compacted

def processJson(filename):
    jF = open(filename, 'r')
    data = json.load(jF)
    jF.close()

    return data

def main(args):
    """ transforms the problem file template """
    
    filename = args[0]
    pddl_files = args[0]
    filename = pddl_files + "root.json"
    goal = args[1]
    data = processJson(filename)

    if not goal in data:
        # print("Goal not reachable from root")
        return

    v = data[goal]
    domain_file = pddl_files + v['domain_file']

    initial_problem = ""
    if "initial_problem" in data[v]:
        initial_problem = pddl_files + data[v][initial_problem]
        # Since this is an intial problem, we want to start up nodes
        configurator = 'configurator_node:='+'rosplan_configurator_interface'
        knowledgeBase = 'knowledge_node:='+'rosplan_knowledge_base'
        executive = 'executive_node:='+'rosplan_executive_interface'
        dispatcher = 'dispatcher_node:='+'rosplan_plan_dispatcher'

        initial_domain = domain_file
        if "initial_domain" in data[v]:
            initial_domain = pddl_files + data[v][initial_domain]
            
        uuid = roslaunch.rlutil.get_or_generate_uuid(None, False)
        roslaunch.configure_logging(uuid)

        cli_args1 = ['rosplan_agent_interface', 'spawnNodes.launch', configurator, knowledgeBase, executive, dispatcher, initial_domain, initial_problem]
        roslaunch_file1 = roslaunch.rlutil.resolve_launch_arguments(cli_args1)
        roslaunch_args1 = cli_args1[2:]

        launch_files = [(roslaunch_file1, roslaunch_args1)]
        parent = roslaunch.parent.ROSLaunchParent(uuid, launch_files)

        parent.start()


    
    problem_template = min(v['problem_templates'], key=lambda f : f['cost'])
    goals = v['goals']

    # Get args
    fluents_file = pddl_files + problem_template['fluents']
    template_file = open(pddl_files + problem_template['name'], 'r')
    output_file = pddl_files + problem_template['output']
    template_string = template_file.read()
    template_file.close()

    # Transform data and write to output
    transformed = transform(fluents_file, template_string)
    with open (output_file, 'w') as f:
        f.write(transformed)
        f.write("\n;; This PDDL problem file was generated on " +  str(datetime.datetime.now()))
    print(domain_file + " " + output_file + " " + " ".join(goals))

if __name__ == "__main__":
    main(sys.argv[1:])
