(set-logic QF_BV)
(set-info :status unsat)
(declare-fun s () (_ BitVec 4))
(declare-fun t () (_ BitVec 4))
(assert (not (= (bvsge s t) (bvsle t s))))
(check-sat)
(exit)
