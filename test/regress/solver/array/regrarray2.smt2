(declare-const m (_ BitVec 1))
(declare-fun r () (Array (_ BitVec 32) (_ BitVec 32)))
(assert (= r (ite (bvult ((_ zero_extend 31) m) (_ bv1 32)) (store r (_ bv0 32) (_ bv0 32)) r)))
(assert (= (_ bv1 32) (select r (_ bv0 32))))
(check-sat)
