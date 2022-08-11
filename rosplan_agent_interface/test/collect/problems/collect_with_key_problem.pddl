(define (problem collect_with_key_problem)
(:domain collect_with_key)
(:objects
case1 - case
doc1 doc2 - document
r0 r1 r2 r3 - room
key1 - key
astrobee - robot

)
(:init

(holding_case astrobee case1)

(not (holding_doc astrobee doc1))
(not (holding_doc astrobee doc2))

(holding_key astrobee key1)

(connected r0 r1)
(connected r1 r0)
(connected r1 r2)
(connected r2 r1)
(connected r2 r3)
(connected r3 r2)

(at astrobee r1)

(in doc1 r1)
(in doc2 r3)

)
(:goal (and
    
    (collected)
)))
;; This PDDL problem file was generated on 2022-08-11 12:52:58.393756