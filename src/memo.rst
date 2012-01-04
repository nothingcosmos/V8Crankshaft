memo
###############################################################################

build方法
===============================================================================

pushd v8

scons --help

release版でビルドする

$ scons

$ scons sample=shell

debug版でビルドする

$ scons mode=debug sample=shell verbose=on disassembler=on


ベンチの取り方
-------------------------------------------------------------------------------

cd benchmarks

../shell run.js


###############################################################################

Debug
===============================================================================

break compiler.cc:xxx
run sample/fact_for.js

360回くらいで、recompile走る。

-------------------------------------------------------------------------------

sample
-------------------------------------------------------------------------------
cd v8
shell
> for (var n in function () { return this } ()) print(n)


疑問点
###############################################################################

V8の生成するx86コードは、なぜintegerを2倍にして扱っているのだろうか。return 1は 0x2だし、3は0x6だし、print(5)すら、push 0x5してるんだけど。

-> 最下位1bitは、smi(smallinteger)か、heappointerかの判定で使用しているため.


typeFeedbackOracleで型情報をASTにフィードバックしたとして、
その型情報を何に使うんだろ

inline展開されたコードがCrankshaftでrecompileされた再に役立つのか？

それともフィードバック機能はおまけで、型推論的なことをやることが目的なのか？  

runtime type correctが見つからない

representation inference
-------------------------------------------------------------------------------

int32かdoubleへ変換され、box化が解除されること

first pass
  phiをたどる。phiのoperandがi32であるかどうか。phiの使用でi32へ変換されているかどうかをみる
  i32への変換可能性をみる

second pass
  phiに、悲観的に変換できないものとして、マークをつけていく

third pass
  phi math-abs binary bitwise arith operation
  タグづけ処理
  i32はタグなし



type inference and canonicalize
-------------------------------------------------------------------------------

type inference is forward control-flow propagation algorithm

type-inferenceは、型付けを確定させて、冗長な型チェックを除去する

phiがあるloop headerでiterationする


Rope / code
-------------------------------------------------------------------------------

アルゴリズム

メリット

文字列の結合が速い

部分文字列が速い substring

文字列のコピーが速い

葉が文字列、ノードがConcatという名前の二分木

一度作った葉ノードの書き換えは行わない

共有しまくるデータ構造であるため、省メモリ

substr, concatがO(log N)



StringAddStub

first second
eax   edx  <-- string
ebx   ecx  <-- string size





JVMも、stubGeneraterが同じ役割を行っているように見える

JVMも似たようなconjoion copyなるものが存在するため、
V8とJVMは等速になるのかなっていうイメージがあるが、そうではないのか？

immutableの差とか、アルゴリズムの差なのか？


JVMはStringAddみたいな中間表現が存在しないはずで、
StringAddをさらに細かいIdealに




