profile
################################################################################

runtime-profiler.cc
================================================================================

  OptimizeNow()
    IsMarkedForLazyRecompilation()

  threshod = sampler_threshod_ * threshold_size_factor


keyword
--------------------------------------------------------------------------------

Runtime_GetOptimizationCount

Runtime_CompileForOnStackReplacement

Runtime_LazyRecompile

opt_count

kMaxOptCount

RuntimeProfilerRateLimiter


--------------------------------------------------------------------------------
--------------------------------------------------------------------------------

profile-generator
================================================================================

--------------------------------------------------------------------------------
--------------------------------------------------------------------------------

IC
================================================================================


type-info.h included ic.h
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------

ia32/code-stubs-ia32.hpp

// Operand type information determined at runtime.
BinaryOpIC::TypeInfo operands_type_;
BinaryOpIC::TypeInfo result_type_;

// Operand type information determined at runtime.
UnaryOpIC::TypeInfo operand_type_;


最初のfull-codegenが生成するのは、UninitializedなIC


結局、ic.cc/hpp


file:flag-definitions.hpp
//
// Logging and profiling flags
//
#undef FLAG
#define FLAG FLAG_FULL


もしかして、storeICとかloadICに対して、
type profileしているのか





################################################################################
================================================================================
--------------------------------------------------------------------------------
