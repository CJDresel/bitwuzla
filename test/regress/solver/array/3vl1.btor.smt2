(set-logic QF_ABV)
(set-info :status unsat)
(declare-const v0 (_ BitVec 8))
(declare-const v1 (_ BitVec 15))
(declare-const a0 (Array (_ BitVec 16) (_ BitVec 1) ))
(assert (= #b1 (bvnot (ite (= (select (store a0 (concat v0 (_ bv0 8)) (_ bv1 1)) (concat v1 (_ bv1 1))) (select a0 (concat v1 (_ bv1 1)))) #b1 #b0))))
(check-sat)
