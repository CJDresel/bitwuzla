(set-logic QF_ABV)
(set-info :status sat)
(set-option :produce-models true)
(define-fun m@0 () (Array (_ BitVec 2) (_ BitVec 8)) (store (store ((as const (Array (_ BitVec 2) (_ BitVec 8))) (_ bv2 8)) (_ bv0 2) (_ bv1 8)) (_ bv3 2) (_ bv3 8)))
(check-sat)
(get-value (m@0))
