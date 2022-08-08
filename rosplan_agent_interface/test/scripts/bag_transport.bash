#!/bin/bash
# Discover path is blocked

update_type="update_type:"
knowledge="knowledge:"

update_type="$update_type 
    - 0";
knowledge="$knowledge
    - knowledge_type: 1
      attribute_name: 'no-robot'
      values:
        - {key: 'location', value: 'loc5'}
      is_negative: true"

update_type="$update_type 
    - 2";
knowledge="$knowledge
    - knowledge_type: 1
      attribute_name: 'no-robot'
      values:
        - {key: 'location', value: 'loc2'}
      is_negative: false"

update_type="$update_type 
    - 2";
knowledge="$knowledge
    - knowledge_type: 1
      attribute_name: 'at'
      values:
        - {key: 'obj', value: 'r2'}
        - {key: 'l', value: 'loc2'}"

update_type="$update_type 
    - 0";
knowledge="$knowledge
    - knowledge_type: 1
      attribute_name: 'at'
      values:
        - {key: 'obj', value: 'r2'}
        - {key: 'l', value: 'loc5'}"

update_type="$update_type 
    - 0";
knowledge="$knowledge
    - knowledge_type: 1
      attribute_name: 'robot-not-busy'
      values:
        - {key: 'r', value: 'r2'}
      is_negative: true"

update_type="$update_type 
    - 0";
knowledge="$knowledge
    - knowledge_type: 1
      attribute_name: 'transport-bag'"

rosservice call /rosplan_knowledge_base/update_array "
$update_type
$knowledge";
