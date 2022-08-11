;;Number of literals: 16
Constructing lookup tables: [10%] [20%] [30%] [40%] [50%] [60%] [70%] [80%] [90%] [100%] [110%] [120%] [130%]
Post filtering unreachable actions:  [10%] [20%] [30%] [40%] [50%] [60%] [70%] [80%] [90%] [100%] [110%] [120%] [130%]
[01;34mNo analytic limits found, not considering limit effects of goal-only operators[00m
Pruning (do_hover_controlled astrobee start_point_1 loc_1) - never appeared in initial RPG
Pruning (do_hover_controlled astrobee start_point_1 loc_2) - never appeared in initial RPG
Pruning (do_hover_controlled astrobee loc_1 start_point_1) - never appeared in initial RPG
Pruning (do_hover_controlled astrobee loc_2 start_point_1) - never appeared in initial RPG
Pruning (do_hover_fast astrobee start_point_1 loc_1) - never appeared in initial RPG
Pruning (do_hover_fast astrobee start_point_1 loc_2) - never appeared in initial RPG
Pruning (do_hover_fast astrobee loc_1 start_point_1) - never appeared in initial RPG
Pruning (do_hover_fast astrobee loc_2 start_point_1) - never appeared in initial RPG
Pruning (correct_position astrobee start_point_1) - never appeared in initial RPG
All the ground actions in this problem are compression-safe
Initial heuristic = 4.000
b (3.000 | 10.000)b (2.000 | 90.001)b (1.000 | 93.002);;;; Solution Found
; States evaluated: 6
; Cost: 103.003
; Time 0.00
0.000: (examine_panel astrobee loc_1 panel_000)  [10.000]
0.001: (do_hover_fast astrobee loc_1 loc_2)  [90.000]
90.002: (correct_position astrobee loc_2)  [3.000]
93.003: (examine_panel astrobee loc_2 panel_001)  [10.000]
