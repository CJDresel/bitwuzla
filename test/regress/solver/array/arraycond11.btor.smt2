(set-logic QF_ABV)
(set-info :status unsat)
(declare-const a0 (Array (_ BitVec 1) (_ BitVec 1) ))
(declare-const a1 (Array (_ BitVec 1) (_ BitVec 1) ))
(declare-const a2 (Array (_ BitVec 1) (_ BitVec 1) ))
(assert (= #b1 (bvand (select a1 (bvnot #b0)) (bvand (bvand (ite (= a0 (store a1 #b0 #b0)) #b1 #b0) (ite (= a2 (ite (= (select a1 (bvnot #b0)) #b1) a0 (store a0 (bvnot #b0) (bvnot #b0)))) #b1 #b0)) (bvnot (select a2 (bvnot #b0)))))))
(check-sat)
