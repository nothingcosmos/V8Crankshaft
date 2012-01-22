type inference
################################################################################

rubyの例を参考にjsのコードでテストしてみる

1. def  id(x) x end

2. def fact(x) x == 0 ? 1 : x * fact(x – 1) end 

3. def  foo(x) x ? 1 : 1.0 end

4. a= 1; p a; a= “a”; p a

5. def harray a = [1, “A”]; a[1] end

6. def itest(x) x.collect {|e| e.to_f}

itest([1, 2, 3]

jsの例) ... commitしていなかったので、自宅に忘れてきた。

 ...


type-info
================================================================================

型情報はこのファイルに記述されている

type-info.h

type-info.c


Class TypeInfo
--------------------------------------------------------------------------------

TypeInfoの構造 ::

  //         Unknown
  //           |   \____________
  //           |                |
  //      Primitive       Non-primitive
  //           |   \_______     |
  //           |           |    |
  //        Number       String |
  //         /   \         |    |
  //    Double  Integer32  |   /
  //        |      |      /   /
  //        |     Smi    /   /
  //        |      |    / __/
  //        Uninitialized.

Class TypeFeedbackOracle
--------------------------------------------------------------------------------

  基本的に辞書っぽいが、辞書への登録、参照用のメソッドを多数定義している。

  辞書のkeyは、ASTのNodeクラスか、unsigned ast_id

  ASTの全Nodeはid()を内包してる。

  内部では、GetInfo(expr->id())


Type-infoの活用方法
################################################################################

Crankshaftでの活用方法
================================================================================

初期化
--------------------------------------------------------------------------------

compiler.cc::

  TypeFeedbackOracle oracle(code, global_context, info->isolate());
  HGraphBuilder builder(info, &oracle);

  HGraphBuilder::HGraphBuilder(CompilationInfo* info,
                             TypeFeedbackOracle* oracle)
    initial_function_state_(this, info, oracle, false)    <-- hydrogen.h::FunctionState oracle_に設定

TypeFeedbackOracleを生成したのち、Hydrogenを生成するHGraphBuilderの第2引数で渡している。
hydrogen中では、oracle_ もしくはoracle() で参照可能らしいので、参照箇所を探す。


hydrogenの中でoracleを参照してる箇所
--------------------------------------------------------------------------------

oracleへの型情報のフィードバックは、hydrogenのgraphを生成する際に行う。

code ::

  void TestContext::BuildBranch(HValue* value) {
  ...
  ToBooleanStub::Types expected(builder->oracle()->ToBooleanTypes(test_id));


  void HGraphBuilder::VisitSwitchStatement(SwitchStatement* stmt) {
  if (switch_type == SMI_SWITCH) {
    clause->RecordTypeFeedback(oracle());
  }

  void HGraphBuilder::HandlePropertyAssignment(Assignment* expr) {
  expr->RecordTypeFeedback(oracle());


  void HGraphBuilder::VisitProperty(Property* expr) {
  expr->RecordTypeFeedback(oracle());


  bool HGraphBuilder::TryInline(Call* expr, bool drop_extra) {
  // ----------------------------------------------------------------
  // After this point, we've made a decision to inline this function (so
  // TryInline should always return true).

  // Save the pending call context and type feedback oracle. Set up new ones
  // for the inlined function.
  TypeFeedbackOracle target_oracle(
    Handle<Code>(target_shared->code()),
    Handle<Context>(target->context()->global_context()),
    isolate());
  // The function state is new-allocated because we need to delete it
  // in two different places.
  FunctionState* target_state =
    new FunctionState(this, &target_info, &target_oracle, drop_extra);

  void HGraphBuilder::VisitCall(Call* expr) {
  // Named function call.
  expr->RecordTypeFeedback(oracle(), CALL_AS_METHOD);

  expr->RecordTypeFeedback(oracle(), CALL_AS_FUNCTION);

  void HGraphBuilder::VisitSub(UnaryOperation* expr) {
  TypeInfo info = oracle()->UnaryType(expr);

  以降、UnaryOperationに同様の処理が続くので省略

  HInstruction* HGraphBuilder::BuildBinaryOperation(BinaryOperation* expr,
                                                  HValue* left,
                                                  HValue* right) {
  TypeInfo info = oracle()->BinaryType(expr);

  void HGraphBuilder::VisitCompareOperation(CompareOperation* expr) {
  TypeInfo type_info = oracle()->CompareType(expr);


ASTでoracleを参照している箇所
================================================================================

ASTでもoracleの参照箇所はあり、上記のRecordTypeFeedback()からASTのNodeへfeedbackしているように見える

RecordTypeFeedback()が用意されているASTのNode ::

  class CaseClause : public ZoneObject
  class Property: public Expression
  class Call: public Expression          <-- CallKind call_kind を追加で与える CALL_AS_METHOD|CALL_AS_FUNCTION
  class CountOperation: public Expression
  class CompareOperation: public Expression
  class Assignment: public Expression


class CaseClause : public ZoneObject
--------------------------------------------------------------------------------

void CaseClause::RecordTypeFeedback(TypeFeedbackOracle* oracle) ::

  TypeInfo info = oracle->SwitchType(this);                             <--
  if (info.IsSmi()) {
    compare_type_ = SMI_ONLY;
  } else if (info.IsSymbol()) {
    compare_type_ = SYMBOL_ONLY;
  } else if (info.IsNonSymbol()) {
    compare_type_ = STRING_ONLY;
  } else if (info.IsNonPrimitive()) {
    compare_type_ = OBJECT_ONLY;
  } else {
    ASSERT(compare_type_ == NONE);
  }



class Property: public Expression
--------------------------------------------------------------------------------

void Property::RecordTypeFeedback(TypeFeedbackOracle* oracle) ::

    ...
    is_monomorphic_ = oracle->LoadIsMonomorphicNormal(this);             <--
    ...
    } else if (is_monomorphic_) {
      receiver_types_.Add(oracle->LoadMonomorphicReceiverType(this));    <--
    } else if (oracle->LoadIsMegamorphicWithTypeInfo(this)) {            <--
      receiver_types_.Reserve(kMaxKeyedPolymorphism);
      oracle->CollectKeyedReceiverTypes(this->id(), &receiver_types_);   <--
    }



class Call: public Expression          <-- CallKind call_kind を追加で与える CALL_AS_METHOD|CALL_AS_FUNCTION
--------------------------------------------------------------------------------

void Call::RecordTypeFeedback(TypeFeedbackOracle* oracle, CallKind call_kind) ::

  is_monomorphic_ = oracle->CallIsMonomorphic(this);                    <--
  Property* property = expression()->AsProperty();
  if (property == NULL) {
    // Function call.  Specialize for monomorphic calls.
    if (is_monomorphic_) target_ = oracle->GetCallTarget(this);         <--
  } else {
    Handle<String> name = Handle<String>::cast(key->handle());
    oracle->CallReceiverTypes(this, name, call_kind, &receiver_types_); <--

    check_type_ = oracle->GetCallCheckType(this);                       <--
    if (is_monomorphic_) {
      if (receiver_types_.length() > 0) {
      } else {
        holder_ = Handle<JSObject>(
          oracle->GetPrototypeForPrimitiveCheck(check_type_));          <--
      }
      is_monomorphic_ = ComputeTarget(map, name);
    }



class CountOperation: public Expression
--------------------------------------------------------------------------------

void CountOperation::RecordTypeFeedback(TypeFeedbackOracle* oracle) ::

  is_monomorphic_ = oracle->StoreIsMonomorphicNormal(this);            <--
  ...
  if (is_monomorphic_) {
    // Record receiver type for monomorphic keyed stores.
    receiver_types_.Add(oracle->StoreMonomorphicReceiverType(this));   <--
  } else if (oracle->StoreIsMegamorphicWithTypeInfo(this)) {           <--
    receiver_types_.Reserve(kMaxKeyedPolymorphism);
    oracle->CollectKeyedReceiverTypes(this->id(), &receiver_types_);   <--
  }


class CompareOperation: public Expression
--------------------------------------------------------------------------------

void CompareOperation::RecordTypeFeedback(TypeFeedbackOracle* oracle) ::

    TypeInfo info = oracle->CompareType(this);                         <--
    if (info.IsSmi()) {
      compare_type_ = SMI_ONLY;
    } else if (info.IsNonPrimitive()) {
      compare_type_ = OBJECT_ONLY;


class Assignment: public Expression
--------------------------------------------------------------------------------

void Assignment::RecordTypeFeedback(TypeFeedbackOracle* oracle) ::

  ...
  is_monomorphic_ = oracle->StoreIsMonomorphicNormal(this);          <--
  ...
  } else if (is_monomorphic_) {
    // Record receiver type for monomorphic keyed stores.
    receiver_types_.Add(oracle->StoreMonomorphicReceiverType(this)); <--
  } else if (oracle->StoreIsMegamorphicWithTypeInfo(this)) {         <--
    receiver_types_.Reserve(kMaxKeyedPolymorphism);
    oracle->CollectKeyedReceiverTypes(this->id(), &receiver_types_); <--
  }





ASTの各メソッドの概要
================================================================================

--------------------------------------------------------------------------------
...


hydrogenの型推論
================================================================================

HValueが基底クラスで、各NodeがCalculateInferredType()を定義

bool HValue::UpdateInferredType() {
  HType type = CalculateInferredType();
  bool result = (!type.Equals(type_));
  type_ = type;
  return result;
}

InitializeInferredTypes()
InferTypes()

HInferRepresentation


================================================================================
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
