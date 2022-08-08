(define (problem leak_failure)
(:domain superDomain)
(:objects
astrobee r2 - robot
panel_000 panel_001 - panel
leak_000 - leak
start_point_1 loc_1 loc_2 end_point - Waypoint
)
(:init

(at r2 start_point_1)
(at astrobee loc_1)

(not (waypoint_not_occupied start_point_1))
(not (waypoint_not_occupied loc_1))
(waypoint_not_occupied loc_2)
(waypoint_not_occupied end_point)

(not (found leak_000 panel_000))
(not (found leak_000 panel_001))

(canexamine astrobee loc_1 panel_000)
(canexamine astrobee loc_2 panel_001)
(canreach astrobee loc_1 panel_000)
(canreach astrobee loc_2 panel_001)

(canexamine r2 loc_1 panel_000)
(canexamine r2 loc_2 panel_001)
(canreach r2 loc_1 panel_000)
(canreach r2 loc_2 panel_001)
(canfix r2)

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
(connected loc_1 end_point) (= (distance loc_1 endpoint) 50)
(connected end_point loc_1) (= (distance endpoint loc_1) 50)
(connected loc_2 end_point) (= (distance loc_2 endpoint) 50)
(connected end_point loc_2) (= (distance end_point loc_2) 50)

)
(:goal ()))
;; This PDDL problem file was generated on 2022-07-07 14:22:06.938646