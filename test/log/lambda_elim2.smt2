(define-fun m ((x (_ BitVec 11))) (_ BitVec 11) (_ bv0 11))
(define-fun s ((x (_ BitVec 11)) (y (_ BitVec 11))) Bool (= (m y) (_ bv0 11)))
(assert (and false (s (_ bv0 11) (_ bv0 11))))
(check-sat)
