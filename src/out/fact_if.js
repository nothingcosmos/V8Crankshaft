function FactIf(n) {
  var p;
  if (n > 1) {
    p = n * FactIf(n - 1);
  } else {
    p = 1;
  }
  return p;
}

function Bench() {
  for (i=0; i<100000; i++) {
    ret = FactIf(i%100);
    print ('--- ' + i + ':'  + ret + '---');
  }
}

Bench();
