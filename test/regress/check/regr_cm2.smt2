(declare-const _z (_ BitVec 1))
(declare-fun z () (_ BitVec 1))
(declare-fun B ((_ BitVec 8) (_ BitVec 8)) (_ BitVec 8))
(assert (not (= (_ bv0 8) (B (_ bv0 8) ((_ zero_extend 7) _z)))))
(assert (= z (bvcomp (_ bv0 8) (B (_ bv0 8) (_ bv0 8)))))
(set-info :status sat)
(check-sat)
