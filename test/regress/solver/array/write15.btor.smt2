(set-logic QF_ABV)
(set-info :status sat)
(declare-const a0 (Array (_ BitVec 2) (_ BitVec 1) ))
(declare-const a1 (Array (_ BitVec 2) (_ BitVec 1) ))
(declare-const v0 (_ BitVec 2))
(declare-const v1 (_ BitVec 2))
(declare-const v2 (_ BitVec 2))
(declare-const v3 (_ BitVec 2))
(declare-const v4 (_ BitVec 2))
(declare-const v5 (_ BitVec 2))
(declare-const v6 (_ BitVec 2))
(declare-const v7 (_ BitVec 1))
(declare-const v8 (_ BitVec 1))
(assert (= #b1 (bvand (bvand (bvand (bvand (bvand (bvand (bvand (bvnot (ite (= v1 v3) #b1 #b0)) (bvnot (ite (= v2 v3) #b1 #b0))) (bvand (bvand (bvnot (ite (= v0 v1) #b1 #b0)) (bvnot (ite (= v0 v2) #b1 #b0))) (bvand (bvnot (ite (= v0 v3) #b1 #b0)) (bvnot (ite (= v1 v2) #b1 #b0))))) (bvand (bvnot (ite (= v5 v6) #b1 #b0)) (bvand (bvnot (ite (= v4 v5) #b1 #b0)) (bvnot (ite (= v4 v6) #b1 #b0))))) (bvnot (ite (= (select a0 v6) (select a1 v6)) #b1 #b0))) (ite (= (select (store a0 v4 v7) v0) (select (store a1 v5 v8) v0)) #b1 #b0)) (ite (= (select (store a0 v4 v7) v1) (select (store a1 v5 v8) v1)) #b1 #b0)) (ite (= (select (store a0 v4 v7) v2) (select (store a1 v5 v8) v2)) #b1 #b0))))
(check-sat)
