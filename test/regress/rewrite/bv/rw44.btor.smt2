(set-logic QF_BV)
(set-info :status sat)
(declare-const v0 (_ BitVec 8))
(assert (= #b1 (bvredor (bvsub v0 (bvnot v0)))))
(check-sat)
