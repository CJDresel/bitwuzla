(set-logic QF_BV)
(set-info :status unsat)
(declare-const v0 (_ BitVec 32))
(declare-const v1 (_ BitVec 32))
(assert (= #b1 (bvnot (ite (= (bvmul (concat (ite (= ((_ extract 15 15) v0) #b1) (bvnot (_ bv0 16)) (_ bv0 16)) ((_ extract 15 0) v0)) (concat (ite (= ((_ extract 15 15) v1) #b1) (bvnot (_ bv0 16)) (_ bv0 16)) ((_ extract 15 0) v1))) ((_ extract 31 0) (bvmul (concat (ite (= ((_ extract 15 15) v1) #b1) (concat (bvnot (_ bv0 16)) ((_ extract 15 15) v1)) (concat (_ bv0 16) ((_ extract 15 15) v1))) ((_ extract 15 0) v1)) (concat (ite (= ((_ extract 15 15) v0) #b1) (concat (bvnot (_ bv0 16)) ((_ extract 15 15) v0)) (concat (_ bv0 16) ((_ extract 15 15) v0))) ((_ extract 15 0) v0))))) #b1 #b0))))
(check-sat)
