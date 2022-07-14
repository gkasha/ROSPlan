import sys, os, datetime, json, rospy, roslaunch, subprocess
from jinja2 import Template, Environment
from typing import Dict, List

from rosplan_knowledge_msgs.srv import *
from rosplan_knowledge_msgs.msg import *

def processJson(root):

    file = open(root, 'r')
    data = json.load(file)
    file.close()

    return data

def main(args):
    """ Process the goal in root.json """
    
    root = args[0]
    goal = args[1]
    data = processJson(root)

    if not goal in data:
        return

    v = data[goal]
    output = ""

    if "initial_domain" in v:
        initial_domain = v['initial_domain']
        initial_problem = v['initial_problem']
        output = initial_domain + " " + initial_problem + " "

    domain_file = v['domain_file']
    problem_template = min(v['problem_templates'], key=lambda x: x['cost'])

    output += domain_file + " " + problem_template
    print(output)
    return

if __name__ == "__main__":
    main(sys.argv[1:])
