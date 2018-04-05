OPTIMIZE = -O2

CC = gcc

CFLAGS = $(OPTIMIZE) -g3 -Wall -Wextra -march=native -mtune=native -mrdrnd



randcpuid.o:	

	$(CC) $(CFLAGS) -c randcpuid.c -o randcpuid.o

randmain.o:	

	$(CC) $(CFLAGS) -c randmain.c -o randmain.o

randlibhw.o:

	$(CC) $(CFLAGS) -fPIC -c randlibhw.c -o randlibhw.o

randlibsw.o:

	$(CC) $(CFLAGS) -fPIC -c randlibsw.c -o randlibsw.o

randlibhw.so: randlibhw.o	

	$(CC) $(CFLAGS) -shared -o randlibhw.so randlibhw.o

randlibsw.so: randlibsw.o

	$(CC) $(CFLAGS) -shared -o randlibsw.so randlibsw.o

randmain: randcpuid.o randmain.o

	$(CC) $(CFLAGS) randcpuid.o randmain.o -ldl -Wl,-rpath=$(PWD) -o randmain

