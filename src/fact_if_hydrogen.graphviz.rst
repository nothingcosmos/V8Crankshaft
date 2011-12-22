hydrogen image
###############################################################################

FactIf::

  begin_compilation
    name "FactIf"
    method "FactIf"
    date 1324387693000
  end_compilation

.. graphviz::

  digraph factFor {

  B0 [shape=box, label=" 0 0 v0 BlockEntry  <|@ \l 0 1 t1 Constant 0x32e08091 <undefined> <|@ \l 0 1 t2 Parameter 0 <|@ \l 0 4 t3 Parameter 1 <|@ \l 0 5 t4 Context  <|@ \l 0 0 v5 Simulate id=2 var[0] = t2, var[1] = t3, var[2] = t4, var[3] = t1 <|@ \l 0 0 v6 Goto B1 <|@ \l "];
  B0 -> B1;

  B1 [shape=box, label=" 0 0 v7 BlockEntry  <|@ \l 0 0 v8 Simulate id=3 <|@ \l 0 0 v9 StackCheck  <|@ \l 0 2 i42 Change t3 t to i range[-2147483648,2147483647,m0=0] <|@ \l 0 2 i43 Constant 1 range[1,1,m0=0] <|@ \l 0 0 v11 CompareIDAndBranch GT i42 i43 goto (B4, B2) <|@ \l "];
  B1 -> B4;
  B1 -> B2;

  B2 [shape=box, label=" 0 0 v15 BlockEntry  <|@ \l 0 0 v16 Simulate id=47 <|@ \l 0 0 v17 Goto B3 <|@ \l "];
  B2 -> B3;

  B3 [shape=box, label=" 0 0 v30 BlockEntry  <|@ \l 0 2 t31 Constant 1 range[1,1,m0=0] type[smi] <|@ \l 0 0 v34 Simulate id=45 var[3] = t31 <|@ \l 0 0 v35 Goto B6 <|@ \l "];
  B3 -> B6;

  B4 [shape=box, label=" 0 0 v12 BlockEntry  <|@ \l 0 0 v13 Simulate id=46 <|@ \l 0 0 v14 Goto B5 <|@ \l "];
  B4 -> B5;

  B5 [shape=box, label=" 0 0 v18 BlockEntry  <|@ \l 0 1 t19 GlobalObject t4 <|@ \l 0 1 i21 Sub i42 i43 range[1,2147483646,m0=0] <|@ \l 0 1 t22 LoadGlobalCell [0x471090d1] <|@ \l 0 0 t23 CheckFunction t22 0x32e41645 <|@ \l 0 1 t24 GlobalReceiver t19 <|@ \l 0 0 t25 PushArgument t24 <|@ \l 0 1 t45 Change i21 i to t range[1,2147483646,m0=0] type[number] <|@ \l 0 0 t26 PushArgument t45 <|@ \l 0 2 t27 CallKnownGlobal o #2 changes[*] <|@ \l 0 0 v28 Simulate id=28 push t3, push t27 <|@ \l 0 1 d40 Change t3 t to d <|@ \l 0 1 d46 Change t27 t to d <|@ \l 0 2 d29 Mul d40 d46 ! <|@ \l 0 0 v32 Simulate id=45 pop 2 / var[3] = d29 <|@ \l 0 1 t47 Change d29 d to t type[heap-number] <|@ \l 0 0 v33 Goto B6 <|@ \l "];
  B5 -> B6;

  B6 [shape=box, label=" begin_HIR 0 0 v37 BlockEntry  <|@ \l 0 0 v38 Return t36 <|@ \l "];
  B6 -> B5;
  B6 -> B3;

  }

