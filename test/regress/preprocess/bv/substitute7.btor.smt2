(set-logic QF_BV)
(set-info :status unsat)
(declare-const v0 (_ BitVec 8))
(assert (= #b1 (bvand (ite (bvult #b01111111 v0) #b1 #b0) (bvnot ((_ extract 7 7) v0)))))
(check-sat)