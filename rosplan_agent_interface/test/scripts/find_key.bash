#!/bin/bash
# Discover path is blocked

update_type="update_type:"
knowledge="knowledge:"

update_type="$update_type 
    - 0";
knowledge="$knowledge
    - knowledge_type: 1
      attribute_name: 'collect-with-key'"

update_type="$update_type 
    - 0";
knowledge="$knowledge
    - knowledge_type: 1
      attribute_name: 'holding_key'
      values:
        - {key: 'v', value: 'astrobee'}
        - {key: 'k', value: 'key1'}"

update_type="$update_type 
    - 0";
knowledge="$knowledge
    - knowledge_type: 1
      attribute_name: 'holding_case'
      values:
        - {key: 'v', value: 'astrobee'}
        - {key: 'c', value: 'case1'}"

rosservice call /rosplan_knowledge_base/update_array "
$update_type
$knowledge";
