#!/bin/bash
gcc -g -c -fpic mem.c -Wall -Werror
gcc -shared -o libmem.so mem.o
gcc -lmem -L. -o myprogram mymain.c -Wall -Werror
myprogram

