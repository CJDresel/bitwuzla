(set-logic QF_BV)
(set-info :status unsat)
(declare-const v0 (_ BitVec 8))
(declare-const v1 (_ BitVec 8))
(assert (= #b1 (bvand (ite (bvult v0 v1) #b1 #b0) (ite (bvult v1 v0) #b1 #b0))))
(check-sat)
