(set-logic QF_BV)
(set-info :status unsat)
(declare-const v0 (_ BitVec 8))
(declare-const v1 (_ BitVec 8))
(assert (= #b1 (ite (distinct (bvand (ite (= ((_ extract 7 5) v0) (_ bv0 3)) #b1 #b0) (bvand (ite (= (bvnot (_ bv0 5)) (bvor ((_ extract 4 0) v0) ((_ extract 4 0) v1))) #b1 #b0) (ite (= ((_ extract 7 5) v1) (_ bv0 3)) #b1 #b0))) (ite (= (bvor v0 v1) #b00011111) #b1 #b0)) #b1 #b0)))
(check-sat)
