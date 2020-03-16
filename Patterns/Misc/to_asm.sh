#! /bin/bash -e

g++ $@ -O2 -c -S -o - -masm=intel | c++-filt | grep -vE '\s+\.'
