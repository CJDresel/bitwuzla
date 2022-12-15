(set-option :produce-models true)
(set-logic QF_ABV)
(push 1)
(define-fun m@0 () (Array (_ BitVec 2) (_ BitVec 8)) ((as const (Array (_ BitVec 2) (_ BitVec 8))) (_ bv0 8)))
(define-fun _resetCount@0 () Bool false)
(declare-fun reset@0 () Bool)
(declare-fun readAddr@0 () (_ BitVec 2))
(define-fun m.out_MPORT.addr@0 () (_ BitVec 2) readAddr@0)
(define-fun m.out_MPORT.data@0 () (_ BitVec 8) (select m@0 m.out_MPORT.addr@0))
(define-fun out@0 () (_ BitVec 8) m.out_MPORT.data@0)
(define-fun _T@0 () Bool (= out@0 (_ bv1 8)))
(define-fun _T_2@0 () Bool (not reset@0))
(define-fun _T_3@0 () Bool (not _T@0))
(define-fun _resetPhase@0 () Bool (not (bvuge (ite _resetCount@0 (_ bv1 1) (_ bv0 1)) (ite true (_ bv1 1) (_ bv0 1)))))
(define-fun assert@0 () Bool (not (=> _T_2@0 _T@0)))
(define-fun _resetActive@0 () Bool (=> _resetPhase@0 reset@0))
(define-fun m.out_MPORT.en@0 () Bool true)
(assert _resetActive@0)
(push 1)
(assert assert@0)
(set-info :status unsat)
(check-sat)
(pop 1)
(define-fun m@1 () (Array (_ BitVec 2) (_ BitVec 8)) m@0)
(define-fun _resetCount@1 () Bool (ite _resetPhase@0 (= ((_ extract 0 0) (bvadd ((_ zero_extend 1) (ite _resetCount@0 (_ bv1 1) (_ bv0 1))) (_ bv1 2))) (_ bv1 1)) _resetCount@0))
(declare-fun reset@1 () Bool)
(declare-fun readAddr@1 () (_ BitVec 2))
(define-fun m.out_MPORT.addr@1 () (_ BitVec 2) readAddr@1)
(define-fun m.out_MPORT.data@1 () (_ BitVec 8) (select m@1 m.out_MPORT.addr@1))
(define-fun out@1 () (_ BitVec 8) m.out_MPORT.data@1)
(define-fun _T@1 () Bool (= out@1 (_ bv1 8)))
(define-fun _T_2@1 () Bool (not reset@1))
(define-fun _T_3@1 () Bool (not _T@1))
(define-fun _resetPhase@1 () Bool (not (bvuge (ite _resetCount@1 (_ bv1 1) (_ bv0 1)) (ite true (_ bv1 1) (_ bv0 1)))))
(define-fun assert@1 () Bool (not (=> _T_2@1 _T@1)))
(define-fun _resetActive@1 () Bool (=> _resetPhase@1 reset@1))
(define-fun m.out_MPORT.en@1 () Bool true)
(assert _resetActive@1)
(push 1)
(assert assert@1)
(set-info :status sat)
(check-sat)
(get-value (m@0))
