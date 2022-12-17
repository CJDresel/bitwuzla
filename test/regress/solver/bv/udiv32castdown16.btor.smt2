(set-logic QF_BV)
(set-info :status unsat)
(declare-const v0 (_ BitVec 16))
(declare-const v1 (_ BitVec 16))
(assert (= #b1 (bvnot (bvand (ite (= (bvudiv v0 v1) ((_ extract 15 0) (bvudiv ((_ zero_extend 16) v0) ((_ zero_extend 16) v1)))) #b1 #b0) (ite (= (bvurem v0 v1) ((_ extract 15 0) (bvurem ((_ zero_extend 16) v0) ((_ zero_extend 16) v1)))) #b1 #b0)))))
(check-sat)
