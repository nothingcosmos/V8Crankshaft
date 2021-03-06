todo
################################################################################

興味を持った点と疑問点
================================================================================

(1) A base compiler(full-codegen)が生成したコード
--------------------------------------------------------------------------------

(1-1) ::

  どんなコードを吐くのか。JavaScript初心者なので、どんな汎用的なコードに落ちるのか興味あります。

full-codegenでcodeを参照して大まかには理解できた。

uninisializeなICをひたすら生成してあるく。らしい。

--print_codeで、full-codegenが生成したコードを参照するのがよいかも。

ICの中身はV8でAsmを叩いて生成している。

(1-2) ::

  V8は再帰関数がxxx monkeyと比較して速いという話を聞いたことがあったので、
  full-codegenが関数をstab越しに呼び出す際に何らかのトリックを使っていそう。

code cacheの仕組みであったり、ICがよくできているのだと思う。

恐らくだが、V8祭りの記事を熟読すれば理解できるはず

(1-3) ::

  hot codeを判断するため、runtimeと連携してprofileを取得する命令をfull-codegenは埋め込むはず。
  遅延を最小にする工夫と、どんなprofile情報を取得しているのか。

どこでやっているのか不明。

恐らくだが、uninisialized ICの中で行っているはず。

現時点での予想としては、load/storeのICの中で行っているのかも。

その結果を型推論して、operand returnに伝搬していくのかもしれない。

もしかしたら、binaryopやunaryのICに入っていないのかもしれない。

データの中身は、テーブルで持っているのだと思う。

(2) Crankshaftが生成したコード
--------------------------------------------------------------------------------

(2-1) ::

  crankshaftは最も高速なコードを生成するはずで、どんなコードを吐くのか。

--print_codeの結果を参照して大まかには理解できた。

smi/heapの違いに着目すれば、大体のコードは追える。

基本的には、smiをguardして、type-stableな区間を仮定してコードを生成していくはず。

smiをtype-stableにコーディングするかぎり、非常に高速に動作するはず。

追加の疑問点として、

stringを使用した場合、どのようにguardするのか。

stringaddが高速らしい。

stringaddは、StringAddStubを読んでいる。Crankshaftでも同様。

StringAddStubの中では、各アーキテクチャの下でasmを叩くのだが、かなり長い。

jsのStringはimmutableなので、StringAddが高速なropeアルゴリズムの使用しているらしい。

smiのguardが冗長に思うのだが、smiのguardがlithiumの段階でもmacroとして存在しているのが原因だろうか。

その辺を速い段階でmacro expandして、何らかの最適化をかければ、冗長なコードを除去できるように思う。

たとえば、llvmのlazy value infoとcorrepsed value propagationを使用すれば改善できる可能性がある。

実際にコーディングしてみるか？

rangecheckで、特にsmiによるガードに対して、cfgベースにrangeを伝搬して冗長なガードの除去を行っているらしい。

llvmの奴は、latticeを使ってif分岐全般を除去することを目的にしているので、
オーバースペックかも。合計3kstepくらいだし。

(2-2) ::

  deoptimizeが発生後、full-codegenへ戻るが、その後の挙動はどうなるのか。
  たとえば、full-codegenは再度プロファイル情報を取得しながらCrankshaftでのJITコンパイルの機会を伺うのか、
  同じ関数のCrankshaftでのJITコンパイルに上限を設けるのか。
  profile情報を落としてfull-codegenでコンパイルを行い、ずっとfull-codegenで実行するのか。

>> 未調査

(2-3) ::

  inliningの仕組み。たとえば、JVMは呼び出し候補が複数ある場合、かつ第1候補が9割の確率で呼ばれる場合、
  第1候補をinliningする。CrankshaftがStabコードのまま扱うのか、inliningする条件が気になる。

>> 調査途中の奴が、optimizeに記述しているはず

(2-4) ::

  runtime profilerで型情報に関する情報を取得し、型推論した上でCrankshaftでJITコンパイルするはず。
  aggressiveに型推論した場合の保証コード+Trapの有無と、型推論の実装はどうなっているのか。

>> smiのケースは、普通にguardするだけ。

(2-5) ::

  型推論の結果をどのように適用するのか。ASTレベルなのかHIRレベルなのか。

>> ASTからhydrogenへの変換持に、type-feedback-oracleによって、ASTへ簡単な型推論の情報を戻しているように思う。

load/storeに限定してだが。 型推論は、type-infe-initializeで行っており、
operand-typeやresult-typeの型をforwardingして伝搬していくようなアルゴリズムになっている。
伝搬してoperandtypeやresulttypeが確定したのち、冗長な型チェックを除去するパスが走る。

追加の疑問点

smi以外のケースや、確定していないケースにおいて、どのようなコードを生成するのかどうか。
ICに頼るのか？ICもuninitialize以外に多数存在したので、genericなICを生成するのかもしれない。


typefeedback oracleの結果は、具体的にどのように活用されるのか。
crankshaftでrecompileした際に使うはず。

異なる型だからこそ、craqnkshaftからdeoptimizeされ、またrecompileされる
そこでtype-feedback-oraleが活用されるのかもしれない。
テストするための具体的なコードを作成したい。


(2-6) ::

  JVM HotSpot C1の生成したコードとどっちが速いか。

未調査。

type-stableにコーディングする限り、かなり高速な印象。

loop-invariant-code-motionの有無やStringAddのため、C1より高速なコードを生成できるはず。

jsのクラスやプロパティの仕組みがまだよくわかってないが、

jsのその辺が動的であり、crankshaftで冗長性を除去できない場合、C1のほうが高速に動作するかも。

(3) hot codeのコンパイルの判断
--------------------------------------------------------------------------------

(3-1) ::

  最初にfull-codegenで生成したコードを実行し、hot codeだと判断したら、
  CrankshaftでJITコンパイルするはず。
  hot codeだと判断する条件は、しきい値以上に呼び出される関数であるかどうか、
  しきい値以上に実行されるループのどちらかのはず。
  hot codeであると判断する上で、runtime profilerとどのように連携するのかどうか。

正確なしきい値で表現できないように思う。

loopの中でfunctionを何度も呼ぶようなケースでは、
inline展開して、その親関数をCrankshaftでコンパイルしてOSRされがち。

再帰関数の場合はcrankshaftでrecompileされるのだが、
再帰でないケースは、Crankshftでコンパイルさせるのは難しいかも

(4) Crankshaftの中間表現とコンパイルパイプラインのデザインに関して
--------------------------------------------------------------------------------

(4-1) ::

  SSA形式といっても、色々あるので、どんな中間表現なのか。

中間表現の構造は、graphベースでphiはinstructionとして表現されていない。

内部のアルゴリズムも、C2のIdealみたいに動作する。

すべてがC2のIdealみたいに細かい粒度の命令ではなく、

StringAddやICが混在した状態で動作することを前提としており、

ところどころかなり粒度の粗いmacroが入っているところがおもしろい。

(4-2) ::

  OSR/Deoptimizeの仕組み。 Tableの仕組みやSafecodeに関して。

>> 未調査

(4-3) ::

  Profile情報の、JavaScript固有の活用方法

>> 大体わかったはず。

型情報はload/storeを起点にして収集し、型情報をforwadingして伝搬してく。

その後、型情報を利用してbox/unboxの除去を行ったり、冗長な型キャストを除去している

型推論がなくてもguardすればよいが、上記の冗長な命令の除去につながるのだと思う。

(5) 追加の宿題
--------------------------------------------------------------------------------

  typeinference-initialize

  hydrogenの本格的な構造

  lithiumの構造と、lithiumの最適化アルゴリズム

  調査結果をsphinxでまとめるのはいいとして、順番に構造化したい。
  sphinxのrstをファイルで分割して、うまく構造化するための方法を調査し、
  blogファイルをメインとして、調査を追加していくような使い方をしたい。
  トップダウンのケースにおいて、うまいまとめかたがあるのかどうか。

