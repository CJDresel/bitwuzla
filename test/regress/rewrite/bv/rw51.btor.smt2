(set-logic QF_BV)
(set-info :status unsat)
(declare-const v0 (_ BitVec 32))
(declare-const v1 (_ BitVec 32))
(assert (= #b1 (bvnot (ite (= (bvadd (bvadd v0 (_ bv1 32)) (bvadd v1 (_ bv1 32))) (bvadd (bvadd (bvadd v0 v1) (_ bv1 32)) (_ bv1 32))) #b1 #b0))))
(check-sat)
