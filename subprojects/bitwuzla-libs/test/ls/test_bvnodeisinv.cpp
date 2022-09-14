#include "test_bvnode.h"

namespace bzla::ls::test {

class TestBvNodeIsInv : public TestBvNode
{
};

TEST_F(TestBvNodeIsInv, add)
{
  test_binary<BitVectorAdd>(IS_INV, ADD, 0);
  test_binary<BitVectorAdd>(IS_INV, ADD, 1);
}

TEST_F(TestBvNodeIsInv, and)
{
  test_binary<BitVectorAnd>(IS_INV, AND, 0);
  test_binary<BitVectorAnd>(IS_INV, AND, 1);
  test_binary<BitVectorAnd>(IS_INV, AND, 0, UNSIGNED);
  test_binary<BitVectorAnd>(IS_INV, AND, 1, UNSIGNED);
  test_binary<BitVectorAnd>(IS_INV, AND, 0, SIGNED);
  test_binary<BitVectorAnd>(IS_INV, AND, 1, SIGNED);
  test_binary<BitVectorAnd>(IS_INV, AND, 0, BOTH);
  test_binary<BitVectorAnd>(IS_INV, AND, 1, BOTH);
}

TEST_F(TestBvNodeIsInv, concat)
{
  test_binary<BitVectorConcat>(IS_INV, OpKind::CONCAT, 0);
  test_binary<BitVectorConcat>(IS_INV, OpKind::CONCAT, 1);
}

TEST_F(TestBvNodeIsInv, eq)
{
  test_binary<BitVectorEq>(IS_INV, EQ, 0);
  test_binary<BitVectorEq>(IS_INV, EQ, 1);
}

TEST_F(TestBvNodeIsInv, mul)
{
  test_binary<BitVectorMul>(IS_INV, MUL, 0);
  test_binary<BitVectorMul>(IS_INV, MUL, 1);
}

TEST_F(TestBvNodeIsInv, shl)
{
  test_binary<BitVectorShl>(IS_INV, SHL, 0);
  test_binary<BitVectorShl>(IS_INV, SHL, 1);
}

TEST_F(TestBvNodeIsInv, shr)
{
  test_binary<BitVectorShr>(IS_INV, SHR, 0);
  test_binary<BitVectorShr>(IS_INV, SHR, 1);
}

TEST_F(TestBvNodeIsInv, ashr)
{
  test_binary<BitVectorAshr>(IS_INV, ASHR, 0);
  test_binary<BitVectorAshr>(IS_INV, ASHR, 1);
}

TEST_F(TestBvNodeIsInv, udiv)
{
  test_binary<BitVectorUdiv>(IS_INV, UDIV, 0);
  test_binary<BitVectorUdiv>(IS_INV, UDIV, 1);
}

TEST_F(TestBvNodeIsInv, ult)
{
  test_binary<BitVectorUlt>(IS_INV, ULT, 0);
  test_binary<BitVectorUlt>(IS_INV, ULT, 1);
  test_binary<BitVectorUlt>(IS_INV, ULT, 0, NONE, OptimizationKind::CONCAT);
  test_binary<BitVectorUlt>(IS_INV, ULT, 1, NONE, OptimizationKind::CONCAT);
  test_binary<BitVectorUlt>(IS_INV, ULT, 0, NONE, OptimizationKind::SEXT);
  test_binary<BitVectorUlt>(IS_INV, ULT, 1, NONE, OptimizationKind::SEXT);
  test_binary<BitVectorUlt>(IS_INV, ULT, 0, UNSIGNED);
  test_binary<BitVectorUlt>(IS_INV, ULT, 1, UNSIGNED);
  test_binary<BitVectorUlt>(IS_INV, ULT, 0, UNSIGNED, OptimizationKind::CONCAT);
  test_binary<BitVectorUlt>(IS_INV, ULT, 1, UNSIGNED, OptimizationKind::CONCAT);
  test_binary<BitVectorUlt>(IS_INV, ULT, 0, UNSIGNED, OptimizationKind::SEXT);
  test_binary<BitVectorUlt>(IS_INV, ULT, 1, UNSIGNED, OptimizationKind::SEXT);
  test_binary<BitVectorUlt>(IS_INV, ULT, 0, SIGNED);
  test_binary<BitVectorUlt>(IS_INV, ULT, 1, SIGNED);
  test_binary<BitVectorUlt>(IS_INV, ULT, 0, SIGNED, OptimizationKind::CONCAT);
  test_binary<BitVectorUlt>(IS_INV, ULT, 1, SIGNED, OptimizationKind::CONCAT);
  test_binary<BitVectorUlt>(IS_INV, ULT, 0, SIGNED, OptimizationKind::SEXT);
  test_binary<BitVectorUlt>(IS_INV, ULT, 1, SIGNED, OptimizationKind::SEXT);
  test_binary<BitVectorUlt>(IS_INV, ULT, 0, BOTH);
  test_binary<BitVectorUlt>(IS_INV, ULT, 1, BOTH);
  test_binary<BitVectorUlt>(IS_INV, ULT, 0, BOTH, OptimizationKind::CONCAT);
  test_binary<BitVectorUlt>(IS_INV, ULT, 1, BOTH, OptimizationKind::CONCAT);
  test_binary<BitVectorUlt>(IS_INV, ULT, 0, BOTH, OptimizationKind::SEXT);
  test_binary<BitVectorUlt>(IS_INV, ULT, 1, BOTH, OptimizationKind::SEXT);
}

TEST_F(TestBvNodeIsInv, slt)
{
  test_binary<BitVectorSlt>(IS_INV, SLT, 0);
  test_binary<BitVectorSlt>(IS_INV, SLT, 1);
  test_binary<BitVectorUlt>(IS_INV, SLT, 0, NONE, OptimizationKind::CONCAT);
  test_binary<BitVectorUlt>(IS_INV, SLT, 1, NONE, OptimizationKind::CONCAT);
  test_binary<BitVectorUlt>(IS_INV, SLT, 0, NONE, OptimizationKind::SEXT);
  test_binary<BitVectorUlt>(IS_INV, SLT, 1, NONE, OptimizationKind::SEXT);
  test_binary<BitVectorSlt>(IS_INV, SLT, 0, SIGNED);
  test_binary<BitVectorSlt>(IS_INV, SLT, 1, SIGNED);
}

TEST_F(TestBvNodeIsInv, urem)
{
  test_binary<BitVectorUrem>(IS_INV, UREM, 0);
  test_binary<BitVectorUrem>(IS_INV, UREM, 1);
}

TEST_F(TestBvNodeIsInv, xor)
{
  test_binary<BitVectorXor>(IS_INV, XOR, 0);
  test_binary<BitVectorXor>(IS_INV, XOR, 1);
}

TEST_F(TestBvNodeIsInv, ite)
{
  test_ite(IS_INV, 0);
  test_ite(IS_INV, 1);
  test_ite(IS_INV, 2);
}

TEST_F(TestBvNodeIsInv, not ) { test_not(IS_INV); }

TEST_F(TestBvNodeIsInv, extract) { test_extract(IS_INV); }

TEST_F(TestBvNodeIsInv, sext) { test_sext(IS_INV); }

}  // namespace bzla::ls::test
