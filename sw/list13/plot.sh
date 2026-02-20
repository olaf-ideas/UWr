#!/bin/sh

stty -F /dev/ttyUSB0 9600 raw -echo

gnuplot -persist <<EOF
set xlabel "Sample"
set ylabel "dBFS"
set grid
plot "< stdbuf -oL cat $PORT" with lines
EOF

