(set-logic QF_ABV)
(set-info :status sat)
(declare-const a0 (Array (_ BitVec 2) (_ BitVec 2) ))
(declare-const v0 (_ BitVec 2))
(assert (= #b1 (bvredxor (select a0 (select a0 v0)))))
(check-sat)