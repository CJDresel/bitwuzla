(set-logic QF_BV)
(set-info :status unsat)
(declare-const v0 (_ BitVec 8))
(declare-const v1 (_ BitVec 8))
(assert (= #b1 (bvnot (bvor (bvnot (ite (distinct v1 (_ bv0 8)) #b1 #b0)) (bvule (bvudiv v0 v1) v0)))))
(check-sat)
