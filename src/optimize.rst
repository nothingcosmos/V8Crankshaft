Crankshaft optimize
################################################################################

MakeCrankshaftCode
================================================================================

コンパイルのメイン処理 ::

  Handle<Context> global_context(info->closure()->context()->global_context());
  TypeFeedbackOracle oracle(code, global_context, info->isolate());
  HGraphBuilder builder(info, &oracle);
  HPhase phase(HPhase::kTotal);
  HGraph* graph = builder.CreateGraph();                     <-- high-level

  if (graph != NULL && FLAG_build_lithium) {
    Handle<Code> optimized_code = graph->Compile(info);      <-- low-level
    if (!optimized_code.is_null()) {
      info->SetCode(optimized_code);
      FinishOptimization(info->closure(), start);
      return true;
    }
  }

builder.CreateGraph()
================================================================================

大まかな流れ ::

  //graph_の生成
  graph_ = HGraph(info())
  
  current_block_ = graph()->entry_block();
  
  HBasicBlock* body_entry = CreateBasicBlock(initial_env);
  current_block()->Goto(body_entry);
  
  VisitDeclarations();
  AddSimulate();
  VisitStatements();
  
  graph()->OrderBlocks();
  graph()->AssignDominators();
  
  graph()->PropagateDeoptimizingMark();
  graph()->EliminateRedundantPhis();
  graph()->EliminateUnreachablePhis();
  graph()->CollectPhis();
  
  HInferRepresentation rep(graph());
  rep.Analyze()
  
  graph()->MarkDeoptimizeOnUndefined();
  graph()->InsertRepresentationChanges();
  
  graph()->InitializeInferredTypes();  <-- 型推論らしい
  graph()->Canonicalize();
  
  HGlobalValueNumberer gvn()       <-- GVN
  gvn.Analyze()
    LoopInvariantCodeMotion()      <-- LICM
    AnalyzeBlock()
  
  HRangeAnalysis rangeAnalysis(graph());
  rangeAnalysis.Analyze();
  
  graph()->ComputeMinusZeroChecks();
  
  HStackCheckLiminator sce(graph());
  sce.Process();
  
  graph()->ReplacedCheckedValues();


HGraph::Compile()
================================================================================

Compile()は、low-level向けの最適化およびコード生成まで行う

lithiumが主役かな。各アーキテクチャ依存の処理を行う。

大まかな流れ ::

  LAllocator allocator();
  LChunkBuilder builder(info, this, &allocator);
  LChunk* chunk = builder.Build();
  
  allocator.Allocate(chunk);
  
  MacroAssembler assembler(info ...);
  LCodeGen generator(chunk, &assembler, info);
  
  generator.Generatecode();
  CodeGenerator::MarkCodePrologu(info);
  code = CodeGenerator::MarkCodeEpilogue(&assembler, flags, info);
  generator.FinishCode(code)
  CodeGenerator::PrintCode(code, info);

Optimizeの詳細
################################################################################

HGlobalValueNumberer::AnalyzeBlock
================================================================================

  AnalyzeBlockなのに、副作用ありでがんがん最適化していく


HGlobalValueNumberer::LoopInvariantCodeMotion()
================================================================================

graphのブロックを走査しながら, blockがLoopHeaderだったら、

  LoopのLastBackEdgeを取得

  ProcessLoopBlock()

    pre_header取得

    headerのHInstructionを走査

      instrのflagcheck

      instrのOperandを走査し、IsDefinedAfter(pre_header) ???

        pre_headerより後ろのブロックで定義されたOperandを参照しているかを、
        block_id()の比較だけで判定している

      invariant判定、かつShouldMove(instr, loop_header) ???

        opt_count()なるものと比較して、Maxを越えてないか判定
        && instr->block()がDeoptimizingの対象でなければ

        instrをUnlink()

        instrをpre_header->end()へInsertBefore

inlining
================================================================================

HGraphBuilder::VisitCall() ::

  expr->RecordTypeFeedback()
  TryCallApply(expr)
  types = expr->GetReceiverTypes()
  if (expr->IsMonomorphic()) {
    if (HasCustomCallGenerator() || type != RECEIVER_MAP_CHECK) {
      call = PreProcessCall(HCallNamed())
    } else {
      AddCheckConstantFunction()
      if (TryInline()) return
      call = PreProcessCall(HCallConstantFunction())
    }
  } else if (types != NULL && types->length() > 1) {
    HandlePolymorphicCallNamed()
  } else {
    call = PreProcessCall( HCallNamed())
  }


HGraphBuilder::HandlePolymorphicCallNamed()

  if展開してそれぞれjumpする

HGraphBuilder::TryInline()

  大きすぎる関数はしない

  継続やらcontext jumpを行うものはしない

  再帰関数はしない

  ParserApi::Parse(&target_info)
  Scope::Analyze(&target_info)

  EnableDeoptimizationSupport()          <-- 直接関係ない
  Compiler::RecordFunctionCompilation()

  AddInstruction(... HEnterInlined())

  VisitDeclarations()
  VisitStatemens()

  後続の処理で、ガチガチにinline展開する



HRangeAnalysis::Analyze
================================================================================

todo


HGraph::Canonicalize()
================================================================================

void HGraph::Canonicalize() ::

  ブロックを走査しながら、
    ブロック中のInstrを走査しながら、
      instr->Canonicalize()を叩いてまわる
      // if (value != instr) instr->DeleteAndReplaceWith(value);
      副作用があれば、旧ノードを消して、旧ノードの使用点を新しいノードが引き継ぐ

Canonicalize()の詳細
--------------------------------------------------------------------------------

型推論後のinstr前提の処理であり、

型が確定しているNodeの冗長なチェック処理を削除していく


見どころ
--------------------------------------------------------------------------------

instr->DeleteAndReplaceWith(value)の詳細 ::

  if (other != NULL) ReplaceAllUsesWith(other);
  ClearOperands();
  DeleteFromGraph();

ReplaceAllUsesWithとか、llvmを参考にしてるように思う。

でも中間表現はgraphベースだから、最適化のアルゴリズムは難しい。





--------------------------------------------------------------------------------
--------------------------------------------------------------------------------


################################################################################
================================================================================
--------------------------------------------------------------------------------
