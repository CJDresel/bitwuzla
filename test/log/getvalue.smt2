(set-option :produce-models true)
(set-logic QF_AUFBVFP)
(declare-const bv (_ BitVec 32))
(declare-const rm RoundingMode)
(declare-const flp (_ FloatingPoint 5 11))
(define-fun A () (Array (_ BitVec 4) (_ BitVec 8)) ((as const (Array (_ BitVec 4) (_ BitVec 8))) (_ bv0 8)))
(define-fun B () (Array (_ FloatingPoint 5 11) (_ BitVec 8)) ((as const (Array (_ FloatingPoint 5 11) (_ BitVec 8))) (_ bv0 8)))
(define-fun C () (Array RoundingMode Bool) ((as const (Array RoundingMode Bool)) false))
(define-fun f ((x (_ BitVec 8))) (_ BitVec 8) (bvadd x (_ bv1 8)))
(declare-fun D () (Array (_ BitVec 2) (_ BitVec 32)))
(declare-fun g ((_ BitVec 32)) Bool)
(check-sat)
(get-value (A))
(get-value ((store A (_ bv4 4) (_ bv1 8))))
(get-value ((store B (fp #b0 #b01011 #b1001100110) (_ bv1 8))))
(get-value ((store (store B (fp #b0 #b01011 #b1001100110) (_ bv1 8)) flp (_ bv2 8))))
(get-value (C))
(get-value ((fp #b0 #b01011 #b1001100110)))
(get-value ((_ bv10 8) (fp #b0 #b01011 #b1001100110)))
(get-value (rm))
(get-value (bv (bvnot bv)))
(get-value (f))
(get-value (D))
(get-value (g))
(exit)
