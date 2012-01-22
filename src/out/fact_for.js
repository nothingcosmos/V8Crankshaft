function FactFor(n) {
  var p = 1;
  for (var i=1; i<=n; i++) {
    p = p * i;
  }
  return p;
}

function Bench() {
  for (i=0; i<100000; i++) {
    ret = FactFor(i%100);
    print ('--- ' + i + ':' + ret + '---');
  }
}

Bench();
