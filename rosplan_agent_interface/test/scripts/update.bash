#!/bin/bash

update_type="update_type:"
knowledge="knowledge:"

update_type="$update_type 
    - 0";
knowledge="$knowledge
    - knowledge_type: 2
      attribute_name: 'leak_state'
      values:
        - {key: 'va', value: 'leak_000'}
      function_value: 0"

update_type="$update_type
    - 0";
knowledge="$knowledge
    - knowledge_type: 2
      attribute_name: 'leak_goal_completed'
      values:
      - {key: 'va', value: 'leak_000'}
      function_value: 0"

update_type="$update_type
    - 0";
knowledge="$knowledge
    - knowledge_type: 1
      attribute_name: 'leak-found'"

update_type="$update_type
    - 0";
knowledge="$knowledge
    - knowledge_type: 1
      attribute_name: 'found'
      values:
      - {key: 'a', value: 'leak_000'}
      - {key: 'p', value: 'panel_000'}"

update_type="$update_type
    - 0";
knowledge="$knowledge
    - knowledge_type: 1
      attribute_name: 'at'
      values:
        - {key: 'v', value: 'astrobee'}
        - {key: 'wp', value: 'loc1'}
      is_negative: True"

rosservice call /rosplan_knowledge_base/update_array "
$update_type
$knowledge";
