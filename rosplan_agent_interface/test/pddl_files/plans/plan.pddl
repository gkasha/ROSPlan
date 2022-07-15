;;Number of literals: 25
Constructing lookup tables: [10%] [20%] [30%] [40%] [50%] [60%] [70%] [80%] [90%] [100%]
Post filtering unreachable actions:  [10%] [20%] [30%] [40%] [50%] [60%] [70%] [80%] [90%] [100%]
Have identified that smaller values of (arm_calibration r2) are preferable
Have identified that bigger values of (leak_goal_completed leak_000) are preferable
[01;34mNo analytic limits found, not considering limit effects of goal-only operators[00m
97% of the ground temporal actions in this problem are compression-safe
Initial heuristic = 5.000
b (4.000 | 90.000)b (3.000 | 90.000)b (2.000 | 90.000);;;; Solution Found
; States evaluated: 6
; Cost: 173.153
; Time 0.00
0.000: (do_hover_fast astrobee loc_1 loc_2)  [90.000]
0.001: (do_hover_fast r2 start_point_1 loc_1)  [50.150]
50.152: (correct_position r2 loc_1)  [3.000]
53.153: (patch_leak r2 loc_1 panel_000 leak_000)  [120.000]
