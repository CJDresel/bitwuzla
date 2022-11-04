#ifndef BZLA_PREPROCESS_PREPROCESSOR_H_INCLUDED
#define BZLA_PREPROCESS_PREPROCESSOR_H_INCLUDED

#include "backtrack/assertion_stack.h"
#include "preprocess/pass/elim_lambda.h"
#include "preprocess/pass/flatten_and.h"
#include "preprocess/pass/rewrite.h"
#include "preprocess/pass/variable_substitution.h"
#include "solver/result.h"

namespace bzla {

class SolvingContext;

namespace preprocess {

class Preprocessor
{
  friend class BacktrackCallback;

 public:
  Preprocessor(SolvingContext& context);

  /** Preprocess current set of assertions. */
  Result preprocess();

  /** Preprocess given term based on last preprocess() call. */
  Node process(const Node& term);

 private:
  /** Apply all preprocessing passes to assertions until fixed-point. */
  void apply(AssertionVector& assertions);

  /** Update d_assertions for given level. */
  void update(size_t level,
              AssertionVector& assertions,
              std::vector<size_t>& indices);

  /** Synchronize d_backtrack_mgr up to given level. */
  void sync_scope(size_t level);

  /** Callback when d_global_backtrack_mgr.pop()) is called. */
  class PopCallback : public backtrack::Backtrackable
  {
   public:
    PopCallback(backtrack::BacktrackManager* mgr, Preprocessor& pp)
        : Backtrackable(mgr), d_preprocessor(pp)
    {
    }

    void push() override {}

    void pop() override
    {
      // Only pop if preprocessor backtrack manager is in sync with global one.
      if (d_preprocessor.d_backtrack_mgr.num_levels()
          == d_preprocessor.d_global_backtrack_mgr.num_levels())
      {
        d_preprocessor.d_backtrack_mgr.pop();
      }
    }

   private:
    Preprocessor& d_preprocessor;
  };

  PopCallback d_pop_callback;

  /** Current set of assertions. */
  backtrack::AssertionView& d_assertions;

  /** Preprocessor backtrack manager. */
  backtrack::BacktrackManager d_backtrack_mgr;

  /** Global backtrack manager of solving context. */
  const backtrack::BacktrackManager& d_global_backtrack_mgr;

  /** Preprocessing passes */
  pass::PassRewrite d_pass_rewrite;
  pass::PassElimLambda d_pass_elim_lambda;
  pass::PassVariableSubstitution d_pass_variable_substitution;
  pass::PassFlattenAnd d_pass_flatten_and;
};

}  // namespace preprocess
}  // namespace bzla

#endif
