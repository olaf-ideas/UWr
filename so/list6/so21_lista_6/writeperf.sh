#!/bin/bash

OPTS="-l 1000 -t 1000"

runtest() {
  echo "Method: $1"
  time strace -c ./writeperf $OPTS $1 > test
  md5sum test
  rm test
  echo ""
}

runtest write
runtest fwrite
runtest fwrite-line
runtest fwrite-full
runtest writev


: '
# OPTS="-l 10000 -t 100"

olaf@laptop:~/uwr/UWr/so/list6/so21_lista_6$ ./writeperf.sh 
Method: write

real    0m13.410s
user    0m0.653s
sys     0m8.094s
aae7be2734ae1d93b0d9d6f8830f46e4  test

Method: fwrite

real    0m36.196s
user    0m1.377s
sys     0m13.439s
aae7be2734ae1d93b0d9d6f8830f46e4  test

Method: fwrite-line

real    0m29.224s
user    0m1.623s
sys     0m13.544s
aae7be2734ae1d93b0d9d6f8830f46e4  test

Method: fwrite-full

real    0m28.107s
user    0m1.554s
sys     0m11.071s
aae7be2734ae1d93b0d9d6f8830f46e4  test

Method: writev

real    0m21.717s
user    0m0.000s
sys     0m8.450s
aae7be2734ae1d93b0d9d6f8830f46e4  test
'