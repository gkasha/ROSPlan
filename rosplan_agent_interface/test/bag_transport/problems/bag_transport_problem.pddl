(define (problem test-problem)
    (:domain bag_transport)
    (:objects
       ;; intialize all the objects in your world
     bag1 - bag
     loc1 loc2 loc3 loc4 loc5 locs1 locs2- location
     r2 astrobee - robot
     )
    (:init

        ;; numerics
        (= (robot-move-speed r2) 15)
        (= (robot-move-speed astrobee) 15)
        
        ;; state
        (at astrobee loc1)
        (at r2 loc2)
        (at bag1 loc1)

        (robot-hand-empty astrobee)
        (robot-hand-empty r2)

        (robot-not-busy astrobee)
        (robot-not-busy r2)

        (not (no-robot loc1))
        (not (no-robot loc2))
        (no-robot loc3)
        (no-robot loc4)
        (no-robot loc5)
        (no-robot locs1)
        (no-robot locs2)


        ;; statics
        (connected loc1 loc5)
        (connected loc5 loc1)
        (connected loc2 loc5)
        (connected loc5 loc2)
        (connected loc3 loc5)
        (connected loc5 loc3)
        (connected loc4 loc5)
        (connected loc5 loc4)

        ; alternate path
        (connected loc1 locs1)
        (connected locs1 loc1)
        (connected locs1 locs2)
        (connected locs2 locs1)
        (connected locs2 loc4)
        (connected loc4 locs2)
         
    )

    ; goal for the task planner to solve
    ; can be multiple
    (:goal ())

)
