(define (domain bag_transport)
    (:requirements
        :strips
        :typing
        :fluents
        :durative-actions
    )

    (:types
     object
     location - object
     bag - object
     robot - object
    )

    (:functions
     (robot-move-speed ?r - robot)
     )
    
    (:predicates

        ;; robot taking a break
        (robot-not-busy ?r - robot) ;; keeping in model, but not super useful right now

        ;; only one robot at a location at a time
        (no-robot ?location - location)

        ;; object locations
        (at ?obj - object ?l - location)

        ;; robot is holding bag
        (robot-holding ?r - robot ?b - bag)

        ;; robot hand empty
        (robot-hand-empty ?r - robot)

        ;; static facts
        ;; connected locations
        (connected ?loc1 - location ?loc2 - location)

    )


    (:durative-action move_to
        :parameters (?movingrobot - robot ?from - location ?to - location)
        :duration (= ?duration (robot-move-speed ?movingrobot))
        :condition (and
                    (at start (robot-not-busy ?movingrobot)) ;; a-s: don't be busy
                    (at start (at ?movingrobot ?from)) ;; a-s: be where you start
                    (over all (connected ?from ?to)) ;; o-a: can move between locations
                    (over all (no-robot ?to)) ;; o-a: no one else is there when reached
                    )
        :effect (and
                 (at start (not (robot-not-busy ?movingrobot))) ;; a-s: busy
                 (at start (not (at ?movingrobot ?from))) ;; a-s: not at source
                 (at start (no-robot ?from)) ;; a-s: empty source location
                 (at end (robot-not-busy ?movingrobot)) ;; a-e: robot is not busy
                 (at end (not (no-robot ?to))) ;; a-e: occupied destination
                 (at end (at ?movingrobot ?to)) ;; a-e: at destination
                 )
        )

    ;; pick-up
    ;; picks up bag at a location
    (:durative-action pick_up
        :parameters (?robot - robot ?b - bag ?loc -location)
        :duration (= ?duration 10)
        :condition (and
                    (at start (robot-not-busy ?robot)) ;; a-s: don't be busy
                    (at start (robot-hand-empty ?robot)) ;; a-s: empty hand
                    (over all (at ?robot ?loc)) ;; o-a: robot at location
                    (over all (at ?b ?loc)) ;; o-a: bag at same location
        )
        :effect (and
                 (at start (not (robot-not-busy ?robot))) ;; a-s: busy robot
                 (at start (not (robot-hand-empty ?robot))) ;; a-s: full hand (not empty)
                 (at end (robot-holding ?robot ?b)) ;; a-e: robot holding bag
                 (at end (robot-not-busy ?robot)) ;; a-e: robot is not busy
                 (at end (not (at ?b ?loc))) ;; a-e: bag no longer in location
                 )
        )

    ;; place
    ;; drops bag at location
    (:durative-action place
        :parameters (?robot - robot ?b - bag ?loc - location)
        :duration (= ?duration 10)
        :condition (and
                    (at start (robot-not-busy ?robot)) ;; a-s: robot not busy
                    (at start (robot-holding ?robot ?b)) ;; a-s: robot holding bag
                    (over all (at ?robot ?loc)) ;; o-a: robot at location
        )
        :effect (and
                 (at start (not (robot-not-busy ?robot))) ;; a-s: robot busy
                 (at end (robot-hand-empty ?robot)) ;; a-e: robot hand empty
                 (at end (at ?b ?loc)) ;; a-e: bag is at location
                 (at end (not (robot-holding ?robot ?b))) ;; a-e: robot not holding bag
                 (at end (robot-not-busy ?robot)) ;; a-e: robot is not busy
                 )
        )
    

)
