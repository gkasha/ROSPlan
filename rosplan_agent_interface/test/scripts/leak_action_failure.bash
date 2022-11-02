#!/bin/bash

update_type="update_type:"
knowledge="knowledge:"

update_type="$update_type 
    - 0";
knowledge="$knowledge
    - knowledge_type: 1
      attribute_name: 'canexamine'
      values:
        - {key: 'v', value: 'astrobee'}
        - {key: 'wp', value: 'loc_2'}
        - {key: 'p', value: 'panel_001'}
      is_negative: true"

update_type="$update_type
    - 0";
knowledge="$knowledge
    - knowledge_type: 1
      attribute_name: 'canexamine'
      values:
        - {key: 'v', value: 'r2'}
        - {key: 'wp', value: 'loc_2'}
        - {key: 'p', value: 'panel_001'}
      is_negative: false"
      
rosservice call /rosplan_knowledge_base/update_array "
$update_type
$knowledge";
