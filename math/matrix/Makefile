CFLAGS  = -std=c++11 -arch x86_64 -I. -I.. -I/opt/local/include -I/Users/hoshino/program_ym0/cardinal/lib/clang
LDFLAGS = -L.


o:
	g++ $(CFLAGS) -O2 -c -o matrix.o matrix.cpp

ar:
	ar rcs libmatrix.a matrix.o

lib:
	c++ $(CFLAGS) -O2 -dynamiclib -o libmatrix.dylib matrix.cpp

mytest:
	g++ $(CFLAGS) $(LDFLAGS) -g -o test test.cpp

