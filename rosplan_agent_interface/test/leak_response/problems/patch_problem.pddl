(define (problem patch_problem)
(:domain patch)
(:objects
leak_000 - leak
start_point_1 loc_1 loc_2 end_point - waypoint
r2 astrobee - robot
panel_000 panel_001 - panel

)
(:init

(canreach astrobee loc_1 panel_000)
(canreach astrobee loc_2 panel_001)
(canreach r2 loc_1 panel_000)
(canreach r2 loc_2 panel_001)

(canfix r2)

(waypoint_not_occupied loc_2)
(waypoint_not_occupied end_point)
(not (waypoint_not_occupied start_point_1))
(not (waypoint_not_occupied loc_1))

(canexamine astrobee loc_2 panel_001)
(canexamine r2 loc_1 panel_000)
(canexamine r2 loc_2 panel_001)

(found leak_000 panel_000)
(not (found leak_000 panel_001))

(at r2 start_point_1)
(at astrobee loc_1)

;; statics 

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

;; end statics

)
(:goal (and
    
    (>= (leak_goal_completed leak_000) 1)
)))
;; This PDDL problem file was generated on 2022-08-23 11:18:22.331699