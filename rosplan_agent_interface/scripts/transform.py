import sys, os, datetime, json, rospy, roslaunch, subprocess
from jinja2 import Template, Environment
from typing import Dict, List

from rosplan_knowledge_msgs.srv import *
from rosplan_knowledge_msgs.msg import *

# read the template from the standard input
# input = "".join(sys.stdin.readlines())
def callPropService(predicate_name):
    # print "Waiting for service"
    rospy.wait_for_service('/rosplan_knowledge_base/state/propositions')
    try:
        # print "Calling Service"
        query_proxy = rospy.ServiceProxy('/rosplan_knowledge_base/state/propositions', GetAttributeService)
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

    problem = data['problem']
    domain = data['domain']
    instances = data['instances']
    # resources = data['resources']
    raw_predicates = data['predicates']
    goals = data['goals']
    
    predicates = []

    instances_set = set()
    for i in data['instances']:
        for x in data['instances'][i]:
            instances_set.add(x)

    for p in raw_predicates:
        # Construct query, get result
        rep = callPropService(p)
        
        
        for elt in rep.attributes:
            match = True
            line = "(" + p
            for x in elt.values:
                line += " " + x.value
                if not x.value in instances_set:
                    match = False
                    break
            if match:
                line += ")"
                if elt.is_negative:
                    line = "(not " + line + ")"
                predicates.append(line)


    return problem,domain,instances,predicates,goals

# def processStatics(file_in):
#     f = open(file_in, 'r')
#     data = f.read().split("\n")
#     return data

def transform(fluents_in, template_string):
    """ transforms the template; this function may be called from other Python code, e.g. Flask web service """

    problem,domain,instances,predicates,goals = processFluents(fluents_in)
    # statics = processStatics(statics_in)
    template = load_template_from_string(template_string)
    transformed = template.render(problem=problem,domain=domain,instances=instances,predicates=predicates,goals=goals)
    compacted = remove_doubled_whitespace(transformed)
    return compacted

def processJson(filename):
    jF = open(filename, 'r')
    data = json.load(jF)
    jF.close()

    return data

def main(args):
    """ transforms the problem file template """
    
    pddl_files = args[0]
    filename = pddl_files + "root.json"
    goal = args[1]
    data = processJson(filename)

    if not goal in data:
        # print("Goal not reachable from root")
        return

    v = data[goal]
    domain_file = pddl_files + v['domain_file']
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
