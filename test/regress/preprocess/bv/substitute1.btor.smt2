(set-logic QF_BV)
(set-info :status sat)
(declare-const v0 (_ BitVec 1))
(declare-const v1 (_ BitVec 1))
(declare-const v2 (_ BitVec 1))
(assert (= #b1 (ite (= v0 (bvand (bvnot (bvand v1 v2)) (bvor v1 v2))) #b1 #b0)))
(check-sat)
