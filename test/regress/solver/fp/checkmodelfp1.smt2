(set-logic ALL)
(set-info :status sat)
(declare-const _s0 (Array (_ FloatingPoint 15 113) (_ FloatingPoint 15 113)))
(declare-fun _s1 ((_ BitVec 117) (_ FloatingPoint 15 113) (_ FloatingPoint 15 113) (_ BitVec 117) (_ FloatingPoint 15 113) (_ FloatingPoint 15 113) (_ FloatingPoint 15 113) (_ BitVec 117) (_ BitVec 117) (_ BitVec 117) (_ BitVec 117)) (_ FloatingPoint 15 113))
(declare-fun _s3 ((_ BitVec 117) (_ FloatingPoint 15 113) (_ FloatingPoint 15 113) (_ BitVec 117) (_ FloatingPoint 15 113) (_ FloatingPoint 15 113) (_ FloatingPoint 15 113) (_ BitVec 117) (_ BitVec 117) (_ BitVec 117) (_ BitVec 117)) (_ FloatingPoint 15 113))
(declare-const _s4 (_ FloatingPoint 15 113))
(declare-const _s5 (Array (_ FloatingPoint 15 113) (_ FloatingPoint 15 113)))
(assert (xor (not (= (select _s5 (_ -zero 15 113)) (_ -zero 15 113) (_ -zero 15 113) (_ -zero 15 113) (select _s5 (_ -zero 15 113)))) (ite (= (select _s5 (_ -zero 15 113)) (_ -zero 15 113) (_ -zero 15 113) (_ -zero 15 113) (select _s5 (_ -zero 15 113))) (fp.geq ( _s1 (bvsub (_ bv106861532287353001828473652309914131 117) #b000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001) (select (store _s0 (_ NaN 15 113) (_ NaN 15 113)) (select (store _s0 (_ -zero 15 113) (_ NaN 15 113)) (_ NaN 15 113))) ( _s1 (_ bv59439327273251349126919848289095254 117) (_ NaN 15 113) (select _s5 (_ -zero 15 113)) (_ bv59439327273251349126919848289095254 117) (_ NaN 15 113) (_ NaN 15 113) _s4 (_ bv106861532287353001828473652309914131 117) (_ bv106861532287353001828473652309914131 117) (_ bv59439327273251349126919848289095254 117) (_ bv59439327273251349126919848289095254 117)) (bvashr #b111011001011001011010001000110110110010100010001010100110111100010110100111110011111011011000100110011101010011011110 #b111011001011001011010001000110110110010100010001010100110111100010110100111110011111011011000100110011101010011011110) (select (store _s0 (_ NaN 15 113) (_ NaN 15 113)) (select (store _s0 (_ -zero 15 113) (_ NaN 15 113)) (_ NaN 15 113))) ( _s3 (_ bv59439327273251349126919848289095254 117) (_ -zero 15 113) (_ -zero 15 113) (bvsub (_ bv106861532287353001828473652309914131 117) #b000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001) (_ -zero 15 113) (_ -zero 15 113) ( _s1 (_ bv59439327273251349126919848289095254 117) (select (store _s0 (_ -zero 15 113) (_ NaN 15 113)) (_ NaN 15 113)) _s4 (_ bv106861532287353001828473652309914131 117) ( _s1 (_ bv59439327273251349126919848289095254 117) (_ NaN 15 113) (select _s5 (_ -zero 15 113)) (_ bv59439327273251349126919848289095254 117) (_ NaN 15 113) (_ NaN 15 113) _s4 (_ bv106861532287353001828473652309914131 117) (_ bv106861532287353001828473652309914131 117) (_ bv59439327273251349126919848289095254 117) (_ bv59439327273251349126919848289095254 117)) _s4 (_ -zero 15 113) (bvsub (_ bv106861532287353001828473652309914131 117) #b000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001) (bvsub (_ bv106861532287353001828473652309914131 117) #b000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001) #b111011001011001011010001000110110110010100010001010100110111100010110100111110011111011011000100110011101010011011110 #b111011001011001011010001000110110110010100010001010100110111100010110100111110011111011011000100110011101010011011110) (bvsub (_ bv106861532287353001828473652309914131 117) #b000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001) (_ bv106861532287353001828473652309914131 117) (_ bv59439327273251349126919848289095254 117) (bvsub (_ bv106861532287353001828473652309914131 117) #b000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001)) (select (store _s0 (_ -zero 15 113) (_ NaN 15 113)) (_ NaN 15 113)) (_ bv59439327273251349126919848289095254 117) (_ bv106861532287353001828473652309914131 117) (bvsub (_ bv106861532287353001828473652309914131 117) #b000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001) (_ bv59439327273251349126919848289095254 117)) ( _s1 (bvsub (_ bv106861532287353001828473652309914131 117) #b000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001) (select (store _s0 (_ NaN 15 113) (_ NaN 15 113)) (select (store _s0 (_ -zero 15 113) (_ NaN 15 113)) (_ NaN 15 113))) ( _s1 (_ bv59439327273251349126919848289095254 117) (_ NaN 15 113) (select _s5 (_ -zero 15 113)) (_ bv59439327273251349126919848289095254 117) (_ NaN 15 113) (_ NaN 15 113) _s4 (_ bv106861532287353001828473652309914131 117) (_ bv106861532287353001828473652309914131 117) (_ bv59439327273251349126919848289095254 117) (_ bv59439327273251349126919848289095254 117)) (bvashr #b111011001011001011010001000110110110010100010001010100110111100010110100111110011111011011000100110011101010011011110 #b111011001011001011010001000110110110010100010001010100110111100010110100111110011111011011000100110011101010011011110) (select (store _s0 (_ NaN 15 113) (_ NaN 15 113)) (select (store _s0 (_ -zero 15 113) (_ NaN 15 113)) (_ NaN 15 113))) ( _s3 (_ bv59439327273251349126919848289095254 117) (_ -zero 15 113) (_ -zero 15 113) (bvsub (_ bv106861532287353001828473652309914131 117) #b000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001) (_ -zero 15 113) (_ -zero 15 113) ( _s1 (_ bv59439327273251349126919848289095254 117) (select (store _s0 (_ -zero 15 113) (_ NaN 15 113)) (_ NaN 15 113)) _s4 (_ bv106861532287353001828473652309914131 117) ( _s1 (_ bv59439327273251349126919848289095254 117) (_ NaN 15 113) (select _s5 (_ -zero 15 113)) (_ bv59439327273251349126919848289095254 117) (_ NaN 15 113) (_ NaN 15 113) _s4 (_ bv106861532287353001828473652309914131 117) (_ bv106861532287353001828473652309914131 117) (_ bv59439327273251349126919848289095254 117) (_ bv59439327273251349126919848289095254 117)) _s4 (_ -zero 15 113) (bvsub (_ bv106861532287353001828473652309914131 117) #b000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001) (bvsub (_ bv106861532287353001828473652309914131 117) #b000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001) #b111011001011001011010001000110110110010100010001010100110111100010110100111110011111011011000100110011101010011011110 #b111011001011001011010001000110110110010100010001010100110111100010110100111110011111011011000100110011101010011011110) (bvsub (_ bv106861532287353001828473652309914131 117) #b000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001) (_ bv106861532287353001828473652309914131 117) (_ bv59439327273251349126919848289095254 117) (bvsub (_ bv106861532287353001828473652309914131 117) #b000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001)) (select (store _s0 (_ -zero 15 113) (_ NaN 15 113)) (_ NaN 15 113)) (_ bv59439327273251349126919848289095254 117) (_ bv106861532287353001828473652309914131 117) (bvsub (_ bv106861532287353001828473652309914131 117) #b000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001) (_ bv59439327273251349126919848289095254 117)) (_ -zero 15 113) (select _s5 (_ -zero 15 113)) ( _s3 (_ bv59439327273251349126919848289095254 117) (_ -zero 15 113) (_ -zero 15 113) (bvsub (_ bv106861532287353001828473652309914131 117) #b000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001) (_ -zero 15 113) (_ -zero 15 113) ( _s1 (_ bv59439327273251349126919848289095254 117) (select (store _s0 (_ -zero 15 113) (_ NaN 15 113)) (_ NaN 15 113)) _s4 (_ bv106861532287353001828473652309914131 117) ( _s1 (_ bv59439327273251349126919848289095254 117) (_ NaN 15 113) (select _s5 (_ -zero 15 113)) (_ bv59439327273251349126919848289095254 117) (_ NaN 15 113) (_ NaN 15 113) _s4 (_ bv106861532287353001828473652309914131 117) (_ bv106861532287353001828473652309914131 117) (_ bv59439327273251349126919848289095254 117) (_ bv59439327273251349126919848289095254 117)) _s4 (_ -zero 15 113) (bvsub (_ bv106861532287353001828473652309914131 117) #b000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001) (bvsub (_ bv106861532287353001828473652309914131 117) #b000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001) #b111011001011001011010001000110110110010100010001010100110111100010110100111110011111011011000100110011101010011011110 #b111011001011001011010001000110110110010100010001010100110111100010110100111110011111011011000100110011101010011011110) (bvsub (_ bv106861532287353001828473652309914131 117) #b000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001) (_ bv106861532287353001828473652309914131 117) (_ bv59439327273251349126919848289095254 117) (bvsub (_ bv106861532287353001828473652309914131 117) #b000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001)) (_ NaN 15 113)) (fp.isPositive (select (store _s0 (_ NaN 15 113) (_ NaN 15 113)) (select (store _s0 (_ -zero 15 113) (_ NaN 15 113)) (_ NaN 15 113)))))))
(check-sat)
