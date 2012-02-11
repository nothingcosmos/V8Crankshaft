debug
###############################################################################

gdb shell_g
run samples/count-xxx.js

Debug
===============================================================================

$ shell_g --help とすると、それっぽいオプションの一覧が出来てます。

適当にオプションを紹介します。 ::

  --trace_hydrogen  カレントのhydornge.cfgにASTやら中間表現を出力する

  --trace_codegen   コンパイルログをstdoutに出力する

  --print_ast       コンパイル対象のjsのASTをstdoutに出力する

  --print_code      コンパイル後のAsmをstdoutに出力する

shell_g --trace_hydrogen --trace_codegen --print_code


shell options
===============================================================================

--crankshaft false true/

--trace_hydrogen

  fileにCFGをダンプする
  カレントのhydrogen.cfg

--hydrogen_stats

  統計情報を実行後にstdoutへ出力

# --debug_code

--trace_codegen

  コンパイルログをstdoutに出力する。

--print_source

  コンパイル対象のjsのsourceをstdoutに出力する。

--print_ast

  コンパイル対象のjsのASTをstdoutに出力

--print_builtin_ast

  起動時にコンパイルするbuiltinのjs関数のASTをstdoutに出力

--print_json_ast

  コンパイル対象のjsのASTをjson形式でstdoutに出力する

--print_code

  コンパイル後のAsmをstdoutに出力する

--print_builtin_code


-------------------------------------------------------------------------------

###############################################################################

===============================================================================

-------------------------------------------------------------------------------

