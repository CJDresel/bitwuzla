(set-logic QF_BV)
(set-info :status unsat)
(declare-fun s () (_ BitVec 64))
(declare-fun t () (_ BitVec 64))
(assert (not (= (bvugt s t) (bvult t s))))
(check-sat)
(exit)
