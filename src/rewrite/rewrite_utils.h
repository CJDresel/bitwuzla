#ifndef BZLA_REWRITE_REWRITE_UTILS_H_INCLUDED
#define BZLA_REWRITE_REWRITE_UTILS_H_INCLUDED

#include "node/node.h"

namespace bzla::rewrite::utils {

/**
 * Helper to determine if two nodes are always disequal.
 *
 * Nodes `a` and `b` can be determined to be always disequal if:
 *   `a` and `b` NOT of function sort
 *   AND (
 *     match (= (bvnot a) a)
 *     OR match (= a (bvnot a))
 *     OR match (= (bvadd a c) a) with c a non-zero value
 *     OR match (= (bvnot (bvadd a c)) (bvnot a)) with c a non-zero value
 *     OR match (= (bvadd a c0) (bvadd a c1))
 *        with c0,c1 values and c0 != c1
 *     OR match (= (bvnot (bvadd a c0)) (bvnot (bvadd a c1)))
 *        with c0,c1 values and c0 != c1
 *   )
 *
 * @param a The first node.
 * @param b The second node.
 * @return True if the two nodes can be determined to be always disequal.
 */
bool is_always_disequal(const Node& a, const Node& b);

}  // namespace bzla::rewrite::utils
#endif
