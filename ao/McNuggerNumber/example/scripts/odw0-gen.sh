g++ odw0-gentest.cpp -o odw0-gentest.e -O2 -static -lm

./odw0-gentest.e 100 4 20 1 > odw3.in

./odw0-gentest.e 501 10 10 2 > odw4.in

./odw0-gentest.e 500 10 50 3 > odw5.in

./odw0-gentest.e 500 100 50 4 > odw6.in

./odw0-gentest.e 40000 100 5 5 > odw7.in

./odw0-gentest.e 40001 100 50 6 > odw8.in

./odw0-gentest.e 40000 10000 50 7 > odw9.in

./odw0-gentest.e 500000 10 25 8 > odw10.in

./odw0-gentest.e 500001 20000 40 9 > odw11.in

./odw0-gentest.e 500000 50000 50 10 > odw12.in

./odw0-gentest.e 1000000 10 10 11 > odw13.in

./odw0-gentest.e 1000000 5000 50 12 > odw14.in

./odw0-gentest.e 1000000 100000 10 13 > odw15.in

./odw0-gentest.e 1000000 100000 60 14 > odw16.in
