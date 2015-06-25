OUTPUTFILE := myls
STATICLIB := libmyls.a
SHARDERLIB := libmyls.so

#LD_LIBRARY_PATH:=/usr/local/lib
all: shared static
	

shared: libmylsshared myls 

libmylsshared: myls.o
	gcc -shared -o libmyls.so  myls.o
	mv -f libmyls.so /home/csuser/rks0007/CS590_Rakesh/midterm/lib
myls.o: 
	gcc -c -o myls.o myls.c

myls:  main 
	gcc -L../lib -o myls main.o -lmyls
	chmod a+x myls
	mv myls ../bin
main:
	gcc -Wall -c main.c -o main.o 

clean: 
	rm -f *.o ../lib/*.a ../lib/*.so ../bin/myls ../bin/myls_static





static: libmylsstatic mylsstatic

libmylsstatic: myls.o
	ar rcs libmyls.a myls.o
	mv -f libmyls.a /home/csuser/rks0007/CS590_Rakesh/midterm/lib	

mylsstatic: main
	gcc -Bstatic -L../lib -o myls_static main.o -lmyls
	mv myls_static ../bin

	

