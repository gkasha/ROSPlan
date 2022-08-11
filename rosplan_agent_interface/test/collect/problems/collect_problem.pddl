(define (problem collect_problem)
(:domain collect)
(:objects
case1 - case
r0 r1 r2 r3 - room
astrobee - robot
key1 - key
doc1 doc2 - document
)
(:init


(holding_case astrobee case1)

(not (holding_key astrobee key1))

(not (holding_doc astrobee doc1))
(not (holding_doc astrobee doc2))

(in doc1 r1)
(in doc2 r3)

(connected r0 r1)
(connected r1 r0)
(connected r1 r2)
(connected r2 r1)
(connected r2 r3)
(connected r3 r2)


(at astrobee r0)
(not (busy astrobee))

)
(:goal ())
)
