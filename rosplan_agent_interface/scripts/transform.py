import sys, os, datetime, json, rospy
from jinja2 import Template, Environment
from typing import Dict, List

from rosplan_knowledge_msgs.srv import *
from rosplan_knowledge_msgs.msg import *

# read the template from the standard input
# input = "".join(sys.stdin.readlines())
def callPropService(predicate_name):
    print "Waiting for service"
    rospy.wait_for_service('/rosplan_knowledge_base/state/propositions')
    try:
        print "Calling Service"
        query_proxy = rospy.ServiceProxy('rosplan_knowledge_base/state/propositions', GetAttributeService)
        resp1 = query_proxy(predicate_name)
        return resp1
    except rospy.ServiceException, e:
        print "Service call failed: %s"%e
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

    for pred in data['data']:
        # Construct query, get result
        predicate_name = pred['predicate_name']
        key = pred['key']
        val = None

        rep = callPropService(predicate_name)
        
        for elt in rep.attributes:
            if elt.values[0].value == key:
                s = "("
                neg = False
                val = elt.values[1].value
                if elt.is_negative:
                    s += "not ("
                    neg = True
                s += predicate_name + " " + key + " " + val + ")"
                if neg:
                    s += ")\n"
                fluents.append(s)

    return fluents

def processStatics(file_in):
    f = open(file_in, 'r')
    data = f.read().split("\n")
    return data

def transform(fluents_in, statics_in, template_string):
    """ transforms the template; this function may be called from other Python code, e.g. Flask web service """

    fluents = processFluents(fluents_in)
    statics = processStatics(statics_in)
    template = load_template_from_string(template_string)
    transformed = template.render(fluents=fluents, statics=statics)
    compacted = remove_doubled_whitespace(transformed)
    return compacted

def main(args):
    """ transforms the problem file template """
    
    # Get args
    fluents_file = args[0]
    statics_file = args[1]
    template_file = open(args[2], 'r')
    output_file = args[3]
    template_string = template_file.read()
    template_file.close()

    # Transform data and write to output
    transformed = transform(fluents_file, statics_file, template_string)
    with open (output_file, 'w') as f:
        f.write(transformed)
        f.write("\n;; This PDDL problem file was generated on " +  str(datetime.datetime.now()))

if __name__ == "__main__":
    main(sys.argv[1:])
