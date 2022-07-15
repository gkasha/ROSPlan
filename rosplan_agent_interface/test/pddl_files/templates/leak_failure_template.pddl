(define (problem leak_failure)
(:domain {{domain}})
(:objects
{% for r in resources['v'] %}{{r}} {% endfor %} - robot
panel_000 panel_001 - panel
leak_000 - leak
start_point_1 loc_1 loc_2 end_point - Waypoint
)
(:init

{% for p in predicates %}
{{p}}{% endfor %}

(connected start_point_1 start_point_1)  (= (distance start_point_1 start_point_1) 0)
(connected start_point_1 start_point_1)  (= (distance start_point_1 start_point_1) 0)
(connected start_point_1 loc_2)  (= (distance start_point_1 loc_2) 50.1498)
(connected start_point_1 loc_1)  (= (distance start_point_1 loc_1) 50.1498)
(connected start_point_1 loc_2)  (= (distance start_point_1 loc_2) 50.1498)
(connected start_point_1 loc_1)  (= (distance start_point_1 loc_1) 50.1498)
(connected loc_2 start_point_1)  (= (distance loc_2 start_point_1) 50.1498)
(connected loc_2 loc_2)  (= (distance loc_2 loc_2) 0)
(connected loc_2 loc_2)  (= (distance loc_2 loc_2) 0)
(connected loc_2 loc_1)  (= (distance loc_2 loc_1) 90)
(connected loc_2 loc_1)  (= (distance loc_2 loc_1) 90)
(connected loc_1 start_point_1)  (= (distance loc_1 start_point_1) 50.1498)
(connected loc_1 loc_2)  (= (distance loc_1 loc_2) 90)
(connected loc_1 loc_1)  (= (distance loc_1 loc_1) 0)
(connected loc_1 loc_1)  (= (distance loc_1 loc_1) 0)
(connected loc_1 end_point) (= (distance loc_1 end_point) 50)
(connected end_point loc_1) (= (distance end_point loc_1) 50)
(connected loc_2 end_point) (= (distance loc_2 end_point) 50)
(connected end_point loc_2) (= (distance end_point loc_2) 50)

)
(:goal (and
    {% for goal in goals %}
    {{goal}}{% endfor %}
)))
