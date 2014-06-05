libmem.so: mem.o
	gcc -g -shared -o libmem.so mem.o;\
	LD_LIBRARY_PATH=.;\
	export LD_LIBRARY_PATH;\

mem.o: mem.c mem.h
	gcc -g -c -fpic -o mem.o mem.c

myprogram: mymain.c  libmem.so
	gcc -g -lmem -L.-o myprogram  mymain.c -Wall -Werror

clean:
	rm -rf *.o libmem.so 

