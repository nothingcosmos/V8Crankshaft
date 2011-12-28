V8 JIT Compiler Overview
###############################################################################

overview
===============================================================================

V8のJITコンパイラは2種類

(1) full-codegen

  jsをインタプリタ実行せず、汎用的に動作するコードを生成するコンパイラ

(2) crankshaft

  Crankshaftは、V8に搭載されたJITコンパイラの名前

  JavaScriptのASTをhigh-levelなSSA形式のIR(Hydrogen)へ変換する。
  Hydrogenは、graphベースらしい

  Hydrogenを機種依存のlow-levelなIR(Lithium)へ変換する。

  機種依存コードは、arm mips ia32/x64が用意されている


構成
===============================================================================


規模
-------------------------------------------------------------------------------

src/XX.cc 146k step

src/XX.h   71k step

src/ia32/XX 43k step

src/x64/XX  44k step

Crankshaft関連のファイル::

  hydrogen               8.5kstep
  hydrogen-instructions  6.5kstep
  lithium                  1kstep
  lithium-allocator      3.0kstep

  ia32/lithium*         10  kstep
  ia32/codegen           0.7kstep


keyword
===============================================================================

crankshaft UseCrankshaft

MakeCrankshaftCode

MarkForLazyRecompilation

Option Flags
===============================================================================

// Flags for Crankshaft. ::

  DEFINE_bool(crankshaft, true, "use crankshaft")
  DEFINE_string(hydrogen_filter, "", "hydrogen use/trace filter")
  DEFINE_bool(use_hydrogen, true, "use generated hydrogen for compilation")
  DEFINE_bool(build_lithium, true, "use lithium chunk builder")
  DEFINE_bool(alloc_lithium, true, "use lithium register allocator")
  DEFINE_bool(use_lithium, true, "use lithium code generator")
  DEFINE_bool(use_range, true, "use hydrogen range analysis")
  DEFINE_bool(eliminate_dead_phis, true, "eliminate dead phis")
  DEFINE_bool(use_gvn, true, "use hydrogen global value numbering")
  DEFINE_bool(use_canonicalizing, true, "use hydrogen instruction canonicalizing")
  DEFINE_bool(use_inlining, true, "use function inlining")
  DEFINE_bool(limit_inlining, true, "limit code size growth from inlining")
  DEFINE_bool(eliminate_empty_blocks, true, "eliminate empty blocks")
  DEFINE_bool(loop_invariant_code_motion, true, "loop invariant code motion")
  DEFINE_bool(collect_megamorphic_maps_from_stub_cache,
              true,
                          "crankshaft harvests type feedback from stub cache")
  DEFINE_bool(hydrogen_stats, false, "print statistics for hydrogen")
  DEFINE_bool(trace_hydrogen, false, "trace generated hydrogen to file")
  DEFINE_bool(trace_inlining, false, "trace inlining decisions")
  DEFINE_bool(trace_alloc, false, "trace register allocator")
  DEFINE_bool(trace_all_uses, false, "trace all use positions")
  DEFINE_bool(trace_range, false, "trace range analysis")
  DEFINE_bool(trace_gvn, false, "trace global value numbering")
  DEFINE_bool(trace_representation, false, "trace representation types")
  DEFINE_bool(stress_pointer_maps, false, "pointer map for every instruction")
  DEFINE_bool(stress_environments, false, "environment for every instruction")
  DEFINE_int(deopt_every_n_times,
             0,
                        "deoptimize every n times a deopt point is passed")
  DEFINE_bool(trap_on_deopt, false, "put a break point before deoptimizing")
  DEFINE_bool(deoptimize_uncommon_cases, true, "deoptimize uncommon cases")
  DEFINE_bool(polymorphic_inlining, true, "polymorphic inlining")
  DEFINE_bool(use_osr, true, "use on-stack replacement")
  
  DEFINE_bool(trace_osr, false, "trace on-stack replacement")
  DEFINE_int(stress_runs, 0, "number of stress runs")
  DEFINE_bool(optimize_closures, true, "optimize closures")


compiler.cc:MakeCrankshaftCode()
===============================================================================

CompilationInfo
EnableDeoptimizationSupport
HPhase phase()
CompilationInfo unoptimized
FullCodeGenerator::MakeCode()
// using hydrogen
HTracer:::Instance()->TraceCompilation(info->function())


TypeFeedbackOracle
HGraphBuilder builder()
HPHase phase()
HGraph* graph = builder.CreateGraph:()
Optimized_code = graph->Compile()
info->SetCode(optimized_code)
FinishOptimization(info->closure(), start)


-------------------------------------------------------------------------------
-------------------------------------------------------------------------------

###############################################################################
===============================================================================
===============================================================================

-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
