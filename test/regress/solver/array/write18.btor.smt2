(set-logic QF_ABV)
(set-info :status sat)
(declare-const a0 (Array (_ BitVec 8) (_ BitVec 8) ))
(declare-const v0 (_ BitVec 8))
(assert (= #b1 (bvredand (select (store a0 (bvadd (_ bv5 8) v0) v0) v0))))
(check-sat)
