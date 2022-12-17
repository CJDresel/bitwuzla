(set-logic QF_BV)
(set-info :status unsat)
(declare-const v0 (_ BitVec 8))
(declare-const v1 (_ BitVec 8))
(assert (= #b1 (bvand (bvand (bvule v0 v1) (bvule v1 v0)) (bvnot (ite (= v0 v1) #b1 #b0)))))
(check-sat)