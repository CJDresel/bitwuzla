(set-logic QF_ABV)
(set-info :status sat)
(declare-const a0 (Array (_ BitVec 2) (_ BitVec 8) ))
(assert (= #b1 (bvor (bvor (bvnot (ite (= (select a0 #b00) (select a0 #b01)) #b1 #b0)) (bvnot (ite (= (select a0 #b00) (select a0 #b10)) #b1 #b0))) (bvnot (ite (= (select a0 #b01) (select a0 #b10)) #b1 #b0)))))
(check-sat)
