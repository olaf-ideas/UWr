#!/usr/bin/bash

# apt list --installed 2>/dev/null | grep -Eo '^[a-zA-Z09]*' | sort | uniq
# ls -1 /usr/share/doc
# find /usr/share/doc -maxdepth 1 -maxdepth 1 -exec test ! -f {}/changelog.Debian.gz \; -print

# a)
comm -23 \
  <(apt list --installed 2>/dev/null | grep -Eo '^[a-zA-Z09]*' | sort | uniq) \
  <(find /usr/share/doc -mindepth 1 -maxdepth 1 -printf "%f\n" | sort | uniq) 

# b)
comm -23 \
  <(find /usr/share/doc -mindepth 1 -maxdepth 1 -printf "%f\n" | sort | uniq) \
  <(apt list --installed 2>/dev/null | grep -Eo '^[a-zA-Z09]*' | sort | uniq)

# c)
comm -23 \ 
  <(apt list --installed 2>/dev/null | grep -Eo '^[a-zA-Z09]*' | sort | uniq) \
  <(find /usr/share/doc -mindepth 1 -maxdepth 1 -exec test ! -f {}/changelog.Debian.gz \; -printf "%f\n" | sort | uniq)


