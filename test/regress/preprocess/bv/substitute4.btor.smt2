(set-logic QF_BV)
(set-info :status sat)
(declare-const v0 (_ BitVec 1))
(declare-const v1 (_ BitVec 1))
(declare-const v2 (_ BitVec 1))
(assert (= #b1 (bvand (bvand (bvand (ite (= (bvnot (_ bv0 1)) v0) #b1 #b0) (bvnot (bvand v1 (bvnot (ite (= v2 v0) #b1 #b0))))) (ite (= v2 (_ bv0 1)) #b1 #b0)) (bvnot (bvand v1 (bvnot (ite (= v0 (_ bv0 1)) #b1 #b0)))))))
(check-sat)
