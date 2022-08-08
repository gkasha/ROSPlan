(define (domain survey)


(:requirements :strips :typing :fluents :disjunctive-preconditions :durative-actions :timed-initial-literals :duration-inequalities)

(:types
		waypoint
		inspectionpoint
		leak		
		panel
		
		chain
		robot)

(:predicates

	(waypoint_not_occupied ?wp - waypoint)
	(connected ?wp1 ?wp2 - waypoint)

	(at ?v - robot ?wp - waypoint)
	(near ?v - robot ?wp - waypoint)
	(found ?l - leak ?p - panel)

	(cansee ?v - robot ?wp - waypoint ?ip - inspectionpoint)
	(on ?a - leak ?p - panel)
	
	
	(canexamine ?v - robot ?wp - waypoint ?p - panel)
	(canreach ?v - robot ?wp - waypoint ?p - panel)
	(examined ?p - panel)

	(chainat ?c - chain ?wp - waypoint)
	(chain_examined ?c - chain)

	(leak-found)
)

(:functions

		(arm_calibration ?auv - robot)

		(observed ?ip - inspectionpoint)
		(obs ?ip - inspectionpoint ?wp - waypoint)

		(distance ?wp1 ?wp2 - waypoint)

)


(:durative-action do_hover_controlled
	:parameters (?v - robot ?from ?to - waypoint)
	:duration ( = ?duration (* (distance ?from ?to) 2))
	:condition (and
				(at start (at ?v ?from)) 
				(over all (waypoint_not_occupied ?to))
				(at start (connected ?from ?to)))
	:effect (and 
				(at start (not (at ?v ?from)))
			    (at end (waypoint_not_occupied ?from))
				(at end (not (waypoint_not_occupied ?to)))
				(at end (at ?v ?to)))
)


(:durative-action do_hover_fast
	:parameters (?v - robot ?from ?to - waypoint)
	:duration ( = ?duration (* (distance ?from ?to) 1))
	:condition (and
				(at start (at ?v ?from))
				(over all (waypoint_not_occupied ?to))
				(at start (connected ?from ?to)))
	:effect (and
				(at start (not (at ?v ?from)))
				(at start (waypoint_not_occupied ?from))
				(at end (not (waypoint_not_occupied ?to)))
				(at end (near ?v ?to)))
)


(:durative-action correct_position
	:parameters (?v - robot ?target - waypoint)
	:duration ( = ?duration 3)
	:condition (and
				(at start (near ?v ?target)))
	:effect (and
				(at start (not (near ?v ?target)))
				(at end (at ?v ?target)))
)


(:durative-action observe_inspection_point
	:parameters (?v - robot ?wp - waypoint ?ip - inspectionpoint)
	:duration ( = ?duration 10)
	:condition (and
				(at start (at ?v ?wp))
				(at start (cansee ?v ?wp ?ip)))
	:effect (and 
				(at start (not (cansee ?v ?wp ?ip)))
				(at start (not (at ?v ?wp)))
				(at end (increase (observed ?ip) (obs ?ip ?wp)))
				(at end (near ?v ?wp)))
)


(:durative-action examine_panel
	:parameters (?v - robot ?wp - waypoint ?p - panel)
	:duration ( = ?duration 10)
	:condition (and
				(at start (at ?v ?wp))
				(at start (canexamine ?v ?wp ?p)))
	:effect (and
				(at start (not (canexamine ?v ?wp ?p)))
				(at end (examined ?p)))
)


(:durative-action follow_chain
	:parameters (?v - robot ?c - chain ?from - waypoint)
	:duration ( = ?duration 90)
	:condition (and
				(at start (at ?v ?from))
				(at start (chainat ?c ?from)))
	:effect (and
				(at start (not (at ?v ?from)))
				(at end (chain_examined ?c))
				(at end (near ?v ?from)))
)

)
