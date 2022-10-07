#ifndef BZLA_REWRITE_REWRITES_BOOL_H_INCLUDED
#define BZLA_REWRITE_REWRITES_BOOL_H_INCLUDED

#include "rewrite/rewriter.h"

namespace bzla {

/* and ---------------------------------------------------------------------- */

template <>
Node RewriteRule<RewriteRuleKind::AND_EVAL>::_apply(Rewriter& rewriter,
                                                    const Node& node);

/* equal -------------------------------------------------------------------- */

template <>
Node RewriteRule<RewriteRuleKind::EQUAL_EVAL>::_apply(Rewriter& rewriter,
                                                      const Node& node);

template <>
Node RewriteRule<RewriteRuleKind::EQUAL_SPECIAL_CONST>::_apply(
    Rewriter& rewriter, const Node& node);

/* distinct ----------------------------------------------------------------- */

template <>
Node RewriteRule<RewriteRuleKind::DISTINCT_CARD>::_apply(Rewriter& rewriter,
                                                         const Node& node);

/* not ---------------------------------------------------------------------- */

template <>
Node RewriteRule<RewriteRuleKind::NOT_EVAL>::_apply(Rewriter& rewriter,
                                                    const Node& node);

/* --- Elimination Rules ---------------------------------------------------- */

template <>
Node RewriteRule<RewriteRuleKind::DISTINCT_ELIM>::_apply(Rewriter& rewriter,
                                                         const Node& node);
template <>
Node RewriteRule<RewriteRuleKind::IMPLIES_ELIM>::_apply(Rewriter& rewriter,
                                                        const Node& node);
template <>
Node RewriteRule<RewriteRuleKind::OR_ELIM>::_apply(Rewriter& rewriter,
                                                   const Node& node);
template <>
Node RewriteRule<RewriteRuleKind::XOR_ELIM>::_apply(Rewriter& rewriter,
                                                    const Node& node);

/* -------------------------------------------------------------------------- */
}  // namespace bzla

#endif