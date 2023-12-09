(define (domain basic_logistics)
(:requirements :strips :typing)
    
    (:types 
        location locatable - object
        items cook - locatable
        ingredient pot - items
        ground counter - location
    )
    
    (:predicates
        (connected ?from ?to - ground)
        (at ?c - cook ?g - ground)
        (in ?i - ingredient ?p - pot)
        (on ?itm - items ?ctr - counter)
        (hold ?i - ingredient ?c - cook)
        (adjacent ?g - ground ?ctr - counter)
    )
    
    ;;================;;
    ;; cook actions ;;
    ;;================;;
    
    (:action walk
      :parameters (?c - cook ?from ?to - ground)
      :precondition (and
        (connected ?from ?to)
        (at ?c ?from)
        (not (at ?c ?to))
        
        ;; TODO: Add the preconditions here
      )
      :effect (and
          (at ?c ?to)
          (not (at ?c ?from))
        ;; TODO: Add the effects here
      )
    )
    
    (:action pick_ingredient
      :parameters (?i - ingredient ?c - cook ?ctr - counter ?stand - ground)
      :precondition (and
      (not (hold ?i ?c))
      (on ?i ?ctr)
      (at ?c ?stand)
      (adjacent ?stand ?ctr)
      
        ;; TODO: Add the preconditions here
      )
      :effect (and
      (hold ?i ?c)
      (not (on ?i ?ctr))
        ;; TODO: Add the effects here
      )
    )
    
    (:action drop_ingredient
    :parameters (?i - ingredient ?c - cook ?ctr - counter ?p - pot ?stand - ground)
    :precondition (and
    (not (in ?i ?p))    ;ingred not in pot, cook at ground that is next to the ctr,
    (at ?c ?stand)     ;pot is on counter, cook holds ingred
    (on ?p ?ctr)
    (adjacent ?stand ?ctr)
    (hold ?i ?c)
    )
    :effect (and    ;cook no longer holds ingred, ingred in pot,
    (in ?i ?p)
    (not (hold ?i ?c))
    )
      ;; TODO: Complete defining this action
    )
    
)