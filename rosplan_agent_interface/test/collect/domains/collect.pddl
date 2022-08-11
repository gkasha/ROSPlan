(define (domain collect)


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



(:durative-action goto_room
	:parameters (?v - robot ?from ?to - room)
	:duration (= ?duration 30)
	:condition (and
				(at start (at ?v ?from))
				(at start (not (busy ?v)))
				(at start (connected ?from ?to)))
	:effect (and 
				(at start (busy ?v)) 
				(at end (not (at ?v ?from)))
				(at end (at ?v ?to))
				(at end (not (busy ?v)))
			)
)

(:durative-action pickup_key
	:parameters (?v - robot ?r - room ?k - key)
	:duration (= ?duration 10)
	:condition (and
				(at start (at ?v ?r))
				(at start (in ?k ?r))
				(at start (not (holding_key ?v ?k)))
				)
	:effect (and
				(at end (holding_key ?v ?k))
				(at end (not (in ?k ?r)))
				)
)


(:durative-action pickup_doc
	:parameters (?v - robot ?d - document ?r - room)
	:duration (= ?duration 30)
	:condition (and
				(at start (in ?d ?r))
				(at start (not (busy ?v)))
				(over all (at ?v ?r))
				(at start (not (holding_doc ?v ?d)))
	) 
	:effect (and 
				(at start (busy ?v))
				(at end (holding_doc ?v ?d))
				(at end (not (in ?d ?r)))
				(at end (not (busy ?v)))
				)
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
