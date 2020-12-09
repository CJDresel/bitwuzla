#include "bitvector_domain.h"
#include "gmpmpz.h"
#include "gtest/gtest.h"

namespace bzlals {

class TestBitVectorDomain : public ::testing::Test
{
};

TEST_F(TestBitVectorDomain, ctor_dtor)
{
  ASSERT_NO_FATAL_FAILURE(BitVectorDomain(1));
  ASSERT_NO_FATAL_FAILURE(BitVectorDomain(10));
  ASSERT_NO_FATAL_FAILURE(BitVectorDomain(BitVector(10, 4), BitVector(10, 5)));
  ASSERT_NO_FATAL_FAILURE(BitVectorDomain("00000000"));
  ASSERT_NO_FATAL_FAILURE(BitVectorDomain("11111111"));
  ASSERT_NO_FATAL_FAILURE(BitVectorDomain("10110100"));
  ASSERT_NO_FATAL_FAILURE(BitVectorDomain("x01xxxxx"));
  ASSERT_NO_FATAL_FAILURE(BitVectorDomain("xxxxxxxx"));
  ASSERT_NO_FATAL_FAILURE(BitVectorDomain(BitVector(4, "0000")));
  ASSERT_NO_FATAL_FAILURE(BitVectorDomain(BitVector(8, "1010")));
  ASSERT_NO_FATAL_FAILURE(BitVectorDomain(4, 0));
  ASSERT_NO_FATAL_FAILURE(BitVectorDomain(8, 5));
  ASSERT_NO_FATAL_FAILURE(BitVectorDomain(BitVectorDomain(8, 5)));
  ASSERT_DEATH(BitVectorDomain(0), "size > 0");
  ASSERT_DEATH(BitVectorDomain(BitVector(10, 4), BitVector(11, 5)),
               "lo.get_size\\(\\) == hi.get_size\\(\\)");
  ASSERT_DEATH(BitVectorDomain(""), "size > 0");
  ASSERT_DEATH(BitVectorDomain("12345"), "is_bin_str");
}

TEST_F(TestBitVectorDomain, get_size)
{
  ASSERT_EQ(BitVectorDomain(1).get_size(), 1);
  ASSERT_EQ(BitVectorDomain(10).get_size(), 10);
  ASSERT_EQ(BitVectorDomain(BitVector(10, 4), BitVector(10, 5)).get_size(), 10);
  ASSERT_EQ(BitVectorDomain("00000000").get_size(), 8);
  ASSERT_EQ(BitVectorDomain("11111111").get_size(), 8);
  ASSERT_EQ(BitVectorDomain("10110100").get_size(), 8);
  ASSERT_EQ(BitVectorDomain("x01xxxxx").get_size(), 8);
  ASSERT_EQ(BitVectorDomain("xxxxxxxx").get_size(), 8);
  ASSERT_EQ(BitVectorDomain(BitVector(4, "0000")).get_size(), 4);
  ASSERT_EQ(BitVectorDomain(BitVector(8, "1010")).get_size(), 8);
  ASSERT_EQ(BitVectorDomain(4, 0).get_size(), 4);
  ASSERT_EQ(BitVectorDomain(8, 5).get_size(), 8);
  ASSERT_EQ(BitVectorDomain(BitVectorDomain(8, 5)).get_size(), 8);
}

TEST_F(TestBitVectorDomain, is_valid)
{
  ASSERT_TRUE(BitVectorDomain(1).is_valid());
  ASSERT_TRUE(BitVectorDomain(10).is_valid());
  ASSERT_TRUE(BitVectorDomain(BitVector(10, 4), BitVector(10, 5)).is_valid());
  ASSERT_TRUE(BitVectorDomain("00000000").is_valid());
  ASSERT_TRUE(BitVectorDomain("11111111").is_valid());
  ASSERT_TRUE(BitVectorDomain("10110100").is_valid());
  ASSERT_TRUE(BitVectorDomain("x01xxxxx").is_valid());
  ASSERT_TRUE(BitVectorDomain("xxxxxxxx").is_valid());
  ASSERT_TRUE(BitVectorDomain(BitVector(4, "0000")).is_valid());
  ASSERT_TRUE(BitVectorDomain(BitVector(8, "1010")).is_valid());
  ASSERT_TRUE(BitVectorDomain(4, 0).is_valid());
  ASSERT_TRUE(BitVectorDomain(8, 5).is_valid());
  ASSERT_TRUE(BitVectorDomain(BitVectorDomain(8, 5)).is_valid());
  ASSERT_FALSE(
      BitVectorDomain(BitVector(4, "1000"), BitVector(4, "0111")).is_valid());
  ASSERT_FALSE(
      BitVectorDomain(BitVector(4, "0100"), BitVector(4, "1011")).is_valid());
  ASSERT_FALSE(
      BitVectorDomain(BitVector(4, "0110"), BitVector(4, "1001")).is_valid());
  ASSERT_FALSE(
      BitVectorDomain(BitVector(4, "1001"), BitVector(4, "0110")).is_valid());
}
}  // namespace bzlals
