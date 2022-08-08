(define (problem {{problem}})
    (:domain {{domain}})
    (:objects
     {% for i in instances %}{% for v in instances[i] %}{{v}} {% endfor %}- {{i}}
     {% endfor %}
     )
    (:init

        ;; numerics
        (= (robot-move-speed r2) 15)
        (= (robot-move-speed astrobee) 15)

        ;; fluents
        {% for p in predicates %}{% for v in predicates[p] %}
        {{v}}{% endfor %}
        {% endfor %}

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
    (:goal (and
        {% for goal in goals %}
        {{goal}}{% endfor %}
    ))

    (:metric minimize (total-time))
)
