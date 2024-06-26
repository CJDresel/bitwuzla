(set-logic QF_BV)
(declare-fun r () (_ BitVec 28))
(declare-fun s () (_ BitVec 56))
(assert (= s (bvmul ((_ zero_extend 28) r) ((_ zero_extend 28) r))))
(assert (= ((_ extract 55 55) s) ((_ extract  0  0) s)))
(assert (= ((_ extract 54 54) s) ((_ extract  1  1) s)))
(assert (= ((_ extract 53 53) s) ((_ extract  2  2) s)))
(assert (= ((_ extract 52 52) s) ((_ extract  3  3) s)))
(assert (= ((_ extract 51 51) s) ((_ extract  4  4) s)))
(assert (= ((_ extract 50 50) s) ((_ extract  5  5) s)))
(assert (= ((_ extract 49 49) s) ((_ extract  6  6) s)))
(assert (= ((_ extract 48 48) s) ((_ extract  7  7) s)))
(assert (= ((_ extract 47 47) s) ((_ extract  8  8) s)))
(assert (= ((_ extract 46 46) s) ((_ extract  9  9) s)))
(assert (= ((_ extract 45 45) s) ((_ extract 10 10) s)))
(assert (= ((_ extract 44 44) s) ((_ extract 11 11) s)))
(assert (= ((_ extract 43 43) s) ((_ extract 12 12) s)))
(assert (= ((_ extract 42 42) s) ((_ extract 13 13) s)))
(assert (= ((_ extract 41 41) s) ((_ extract 14 14) s)))
(assert (= ((_ extract 40 40) s) ((_ extract 15 15) s)))
(assert (= ((_ extract 39 39) s) ((_ extract 16 16) s)))
(assert (= ((_ extract 38 38) s) ((_ extract 17 17) s)))
(assert (= ((_ extract 37 37) s) ((_ extract 18 18) s)))
(assert (= ((_ extract 36 36) s) ((_ extract 19 19) s)))
(assert (= ((_ extract 35 35) s) ((_ extract 20 20) s)))
(assert (= ((_ extract 34 34) s) ((_ extract 21 21) s)))
(assert (= ((_ extract 33 33) s) ((_ extract 22 22) s)))
(assert (= ((_ extract 32 32) s) ((_ extract 23 23) s)))
(assert (= ((_ extract 31 31) s) ((_ extract 24 24) s)))
(assert (= ((_ extract 30 30) s) ((_ extract 25 25) s)))
(assert (= ((_ extract 29 29) s) ((_ extract 26 26) s)))
(assert (= ((_ extract 28 28) s) ((_ extract 27 27) s)))
; assume the highest bit of 's' is set
;
(assert ((_ extract 55 55) s))
; ... or alternatively disallow the following spurious solutions:
;
;(assert (distinct r #b0000000000000000000000000000))
;(assert (distinct r #b0000000000000110000000000000))
;(assert (distinct r #b0000000101110110101110000000))
;(assert (distinct r #b0001100000101011101100011000))
;(assert (distinct r #b0010111110100100111011100100))
; there is no solution for 56 bits ...
(check-sat)
(exit)
