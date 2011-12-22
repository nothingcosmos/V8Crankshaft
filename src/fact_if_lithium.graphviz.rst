Lithium image
###############################################################################

FactIf::

  begin_compilation
    name "FactIf"
    method "FactIf"
    date 1324387693000
  end_compilation

.. graphviz::

  digraph factFor {

  B0 [shape=box, label=" begin_LIR \l 0 label () () () ()  <|@ \l 2 gap () () () ()  <|@ \l 4 parameter [stack:-2]=  <|@ \l 6 gap () ([stack:-2];) () ()  <|@ \l 8 parameter [stack:-1]=  <|@ \l 10 gap () ([stack:-1];) () ()  <|@ \l 12 context [eax|R]=  <|@ \l 14 gap ([stack:0] = [eax|R];) () () ()  <|@ \l 16 gap () () () ()  <|@ \l 18 goto B1 <|@ \l end_LIR \l "];
  B0 -> B1;

  B1 [shape=box, label=" begin_LIR \l 20 label () () () ()  <|@ \l 22 gap () ([esi|R] = [eax|R];) () ()  <|@ \l 24 stack-check = [esi|R] [id=3|[parameters=2|[arguments_stack_height=0|[stack:-2];[stack:-1];[constant:1]] {[esi|R];[stack:0];[eax|R]} @-1 <|@ \l 26 gap () ([eax|R] = [stack:-1];) () ()  <|@ \l 28 tagged-to-i [eax|R]= [eax|R] [id=3|[parameters=2|[arguments_stack_height=0|[stack:-2];[stack:-1];[constant:1]] <|@ \l 30 gap (v0(0) = [eax|R];) () () ()  <|@ \l 32 gap () () () ()  <|@ \l 34 cmp-id-and-branch if [eax|R] > [constant:43] then B4 else B2 <|@  \l end_LIR \l "];
  B1 -> B4;
  B1 -> B2;

  B2 [shape=box, label=" begin_LIR \l 36 label () () () ()  Dead block replaced with B3 <|@ \l 38 gap () () () ()  <|@ \l 40 gap () () () ()  <|@ \l 42 goto B3 <|@ \l end_LIR \l "];
  B2 -> B3;

  B3 [shape=box, label=" begin_LIR \l 44 label () () () ()  <|@ \l 46 gap () () () ()  <|@ \l 48 gap () ([ecx|R] = [constant:31];) () ()  <|@ \l 50 goto B6 <|@ \l end_LIR \l "];
  B3 -> B6;

  B4 [shape=box, label=" begin_LIR \l 52 label () () () ()  Dead block replaced with B5 <|@ \l 54 gap () () () ()  <|@ \l 56 gap () () () ()  <|@ \l 58 goto B5 <|@ \l end_LIR \l "];
  B4 -> B5;

  B5 [shape=box, label=" begin_LIR \l 60 label () () () ()  <|@ \l 62 gap () () ([ecx|R] = [stack:0];) ()  <|@ \l 64 global-object [ecx|R]= [ecx|R] <|@ \l 66 gap (v0(0) = [ecx|R];) ([eax|R];) () ()  <|@ \l 68 sub-i [eax|R]= [eax|R] [constant:43] <|@ \l 70 gap (v0(0) = [eax|R];) () () ()  <|@ \l 72 load-global-cell [edx|R]=  <|@ \l 74 gap (v0(0) = [edx|R];) () () ()  <|@ \l 76 check-function = [edx|R] [id=46|[parameters=2|[arguments_stack_height=0|[stack:-2];[stack:-1];[constant:1]] <|@ \l 78 gap () () () ()  <|@ \l 80 global-receiver [ecx|R]= [ecx|R] <|@ \l 82 gap (v0(0) = [ecx|R];) () () ()  <|@ \l 84 push-argument = [ecx|R] <|@ \l 86 gap () ([eax|R];) () ()  <|@ \l 88 number-tag-i [eax|R]= [eax|R] [id=46|[parameters=2|[arguments_stack_height=1|[stack:-2];[stack:-1];[constant:1]] {[eax|R]} @66 <|@ \l 90 gap (v0(0) = [eax|R];) () () ()  <|@ \l 92 push-argument = [eax|R] <|@ \l 94 gap () () () ()  <|@ \l 96 call-known-global [eax|R]#1 /  {} @57 <|@ \l 98 gap (v0(0) = [eax|R];) ([eax|R];) () ()  <|@ \l 100 lazy-bailout =  [id=28|[parameters=2|[arguments_stack_height=0|[stack:-2];[stack:-1];[constant:1];[stack:-1];[eax|R]] <|@ \l 102 gap () () ([ecx|R] = [stack:-1];) ()  <|@ \l 104 double-untag [xmm1|R]= [ecx|R] [id=28|[parameters=2|[arguments_stack_height=0|[stack:-2];[ecx|R];[constant:1];[ecx|R];[eax|R]] <|@ \l 106 gap (v0(0) = [xmm1|R];) () () ()  <|@ \l 108 double-untag [xmm2|R]= [eax|R] [id=28|[parameters=2|[arguments_stack_height=0|[stack:-2];[ecx|R];[constant:1];[ecx|R];[eax|R]] <|@ \l 110 gap (v0(0) = [xmm2|R];) ([xmm1|R];) () ()  <|@ \l 112 mul-d [xmm1|R]= [xmm1|R] [xmm2|R] <|@ \l 114 gap (v0(0) = [xmm1|R];) () () ()  <|@ \l 116 number-tag-d [ecx|R]= [xmm1|R] {} @55 <|@ \l 118 gap (v0(0) = [ecx|R];) () () ()  <|@ \l 120 gap () ([ecx|R];) () ()  <|@ \l 122 goto B6 <|@ \l end_LIR \l "];
  B5 -> B6;

  B6 [shape=box, label=" begin_LIR \l 124 label () (v0(0) = [ecx|R];) () ()  <|@ \l 126 gap () ([eax|R] = [ecx|R];) () ()  <|@ \l 128 return = [eax|R] <|@ \l 130 gap () () () ()  <|@ \l end_LIR \l "];
  B6 -> B5;
  B6 -> B3;
  }

