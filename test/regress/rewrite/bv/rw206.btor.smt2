(set-logic QF_BV)
(set-info :status unsat)
(declare-const v0 (_ BitVec 1))
(declare-const v1 (_ BitVec 8))
(declare-const v2 (_ BitVec 8))
(declare-const v3 (_ BitVec 8))
(assert (= #b1 (ite (distinct (ite (= v0 #b1) v3 (ite (= v0 #b1) v1 (bvnot v2))) (ite (= v0 #b1) v3 (bvnot v2))) #b1 #b0)))
(check-sat)
