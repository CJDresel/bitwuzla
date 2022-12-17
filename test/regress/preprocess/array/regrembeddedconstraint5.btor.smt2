(set-logic QF_ABV)
(set-info :status sat)
(declare-const v0 (_ BitVec 3))
(declare-const a0 (Array (_ BitVec 4) (_ BitVec 2) ))
(declare-const a1 (Array (_ BitVec 4) (_ BitVec 2) ))
(declare-const v1 (_ BitVec 1))
(assert (= #b1 (bvand ((_ extract 2 2) v0) (bvnot (ite (= (bvnot ((_ extract 0 0) (select (ite (= ((_ extract 2 2) v0) #b1) a0 a1) (bvnot (_ bv0 4))))) v1) #b1 #b0)))))
(check-sat)