(set-logic QF_BV)
(set-info :status sat)
(declare-const x (_ BitVec 3))
(declare-const y (_ BitVec 3))
(assert (or (= (bvredand x) #b1) (= (bvredand y) #b1)))
(check-sat)
