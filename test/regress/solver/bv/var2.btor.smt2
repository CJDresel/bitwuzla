(set-logic QF_BV)
(set-info :status sat)
(declare-const v0 (_ BitVec 1))
(assert (= #b1 (bvnot v0)))
(check-sat)
