#!/usr/bin/sed -nEf

:loop
p

s/^(R*)RG/\1BB/; t loop;
s/^(R*)RB/\1GG/; t loop;
s/^(G*)GR/\1BB/; t loop;
s/^(G*)GB/\1RR/; t loop;
s/^(B*)BR/\1GG/; t loop;
s/^(B*)BG/\1RR/; t loop;

q
