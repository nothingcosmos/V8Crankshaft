typedef struct _object {
  union {
    struct {
      signed int   val:31;
      unsigned int tag:1;
    } smi;
    void *heapobject;
    struct {
      signed int   val:30;
      unsigned int tag:2;
    } failure;
  };
} object;

object fail = {0};

class env {
public:
  object FactIf(object n);
private:
  object (env::*property[])(object);
};

object env::FactIf(object n) {
  //  object p
  object p;
  //  if (n > 1) {
  if (n.smi.tag == 1) goto deoptimize;
  int n2;
  n2  = n.smi.val;
  if (n2 > 1) goto then_block;

else_block:
  //    p = 1;
  p.smi = reinterpret_cast<object>(1<<1);
  goto join_block;

then_block:
  //    p = n * FactIf(n - 1);
  if (n.smi.tag == 1) goto deoptimize;
  int n3;
  n3 = n2 - 1;

  if (this->property[0] == &env::FactIf) goto deoptimize;

  object arg;
  if ((n3 + n3) > n3) goto deoptimize;
  arg.smi = reinterpret_cast<object>(n3 + n3);

  object ret;
  ret = env::FactIf(arg);

  if (ret.smi.tag == 1) goto deoptimize;

  int ret2;
  ret2 = ret.smi.val;
  if ((ret2+ret2) > ret2) goto deoptimize;

  p.smi.tag = 0;
  p.smi.val = ret2;

join_block:
  //  return p;
  return p;
deoptimize:
  return fail;
}
