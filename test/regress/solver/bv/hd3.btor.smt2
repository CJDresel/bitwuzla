(set-logic QF_BV)
(set-info :status unsat)
(declare-const v0 (_ BitVec 8))
(declare-const v1 (_ BitVec 8))
(assert (= #b1 (bvor (bvor (ite (distinct (bvsub v0 v1) (bvadd (bvadd v0 (bvnot v1)) (_ bv1 8))) #b1 #b0) (ite (distinct (bvsub v0 v1) (bvsub (bvxor v0 v1) (bvmul (bvand (bvnot v0) v1) (_ bv2 8)))) #b1 #b0)) (bvor (ite (distinct (bvsub v0 v1) (bvsub (bvand v0 (bvnot v1)) (bvand (bvnot v0) v1))) #b1 #b0) (ite (distinct (bvsub v0 v1) (bvsub (bvmul (bvand v0 (bvnot v1)) (_ bv2 8)) (bvxor v0 v1))) #b1 #b0)))))
(check-sat)
