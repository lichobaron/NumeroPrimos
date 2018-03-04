all: primproc primsec primhilos clean
	
primproc: primproc.o
	gcc -o primproc primproc.o
primproc.o: primproc.c
	gcc -c primproc.c
primhilos: primhilos.o
	gcc -o primhilos primhilos.o -pthread
primhilos.o: primhilos.c
	gcc -c primhilos.c
primsec: primsec.o
	gcc -o primsec primproc.o
primsec.o: primsec.c
	gcc -c primsec.c
clean:
	rm -r *.o