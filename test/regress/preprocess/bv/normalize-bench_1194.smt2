(declare-const T (_ BitVec 1))
(declare-const T4 (_ BitVec 1))
(assert (= (bvadd (_ bv1 32) ((_ zero_extend 31) T4)) (bvadd ((_ zero_extend 31) T4) (_ bv1 32) ((_ zero_extend 31) T) (_ bv3 32))))
(check-sat)
