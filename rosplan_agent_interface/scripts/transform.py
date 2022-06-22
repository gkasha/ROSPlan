import sys, os, datetime, json
from jinja2 import Template, Environment
from typing import Dict, List

# read the template from the standard input
# input = "".join(sys.stdin.readlines())

def tif_filter(time: float, value: float, *function_name) -> str:
    """ Creates time-initial fluent if time>0, or plain initialization otherwise """
    assignment = "(= ({}) {})".format(' '.join(function_name), value)
    return "(at {} {})".format(time, assignment) if time > 0\
        else assignment

def load_template_from_string(template_text: str) -> Template:
    jinja2_env = Environment(trim_blocks = False, lstrip_blocks = False)
    # jinja2_env.filters['tif'] = tif_filter

    return jinja2_env.from_string(template_text)

def remove_doubled_whitespace(text: str) -> str:
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

def transform(input_data, template_string: str) -> str:
    """ transforms the template; this function may be called from other Python code, e.g. Flask web service """

    json_file = open(input_data, "r")
    data = json.load(json_file)
    json_file.close()

    template = load_template_from_string(template_string)
    transformed = template.render(data=data)
    compacted = remove_doubled_whitespace(transformed)
    return compacted

def main(args):
    """ transforms the problem file template """
    
    # Get args
    input_data = args[0]
    template_file = open(args[1], 'r')
    output_file = args[2]
    template_string = template_file.read()
    template_file.close()

    # Transform data and write to output
    transformed = transform(input_data, template_string)
    with open (output_file, 'w') as f:
        f.write(transformed)
        f.write("\n;; This PDDL problem file was generated on " +  str(datetime.datetime.now()))

if __name__ == "__main__":
    main(sys.argv[1:])
