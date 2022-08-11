(define (domain collect_with_key)


(:requirements :negative-preconditions :strips :typing :fluents :disjunctive-preconditions :durative-actions :duration-inequalities)

(:types
		object 
		case - object
		key - object
		document - object
		room
		table
		robot)

(:predicates

	
	(connected ?r1 ?r2 - room)
	(busy ?v - robot)
	(at ?v - robot ?r - room) ;; robot at room
	(holding_key ?r -robot ?k - key) ;; robot holds key
	(holding_doc ?r -robot ?k - document) ;; robot holds key
	(holding_case ?r -robot ?c - case) ;; robot holds case
	(loc ?t - table ?r - room) ;; table is in room
	(in ?o - object ?r - room) ;; table in room
	(collected) 
	
)


(:durative-action collect_all_with_key
	:parameters (?v - robot ?c - case ?k - key)
	:duration ( = ?duration 30)
	:condition (and
				(at start (holding_key ?v ?k))
				(at start (holding_case ?v ?c)) 
				) 			
	:effect (and 
				(at end (collected))
				)
)

)
