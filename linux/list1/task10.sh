#!/usr/bin/bash

#sed 's/^ \+//;:a;N;$!ba;s/\n/ /g; s/ \+/ /g; s/\.[ \t\n]/.\n/g;s/[ \t]\+/ /g' input.txt
sed 's/^ \+//;:a;N;$!ba;s/\n/ /g; s/ \+/ /g; s/\.[ \t\n]/.\n/g;s/[ \t]\+/ /g' input.txt
