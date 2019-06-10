CC=g++ 
CFLAGS= -O3 -Wall -fPIC -std=c++0x
all:FMQ clean

FMQ:div main.o compress.o FMQ.o perfectHash.o fm.a
	g++ $(CFLAGS) main.o compress.o FMQ.o perfectHash.o fm.a -o FMQ

fm.a:ABS_WT.o Balance_WT.o Huffman_WT.o Hutacker_WT.o FM.o BitMap.o UseCount.o WT_Handle.o InArray.o loadkit.o savekit.o divsufsort.o sssort.o trsort.o utils.o
	ar rc fm.a ABS_WT.o Balance_WT.o Huffman_WT.o Hutacker_WT.o FM.o BitMap.o UseCount.o WT_Handle.o  InArray.o loadkit.o savekit.o divsufsort.o sssort.o trsort.o utils.o

%.o:%.cpp *.h
	$(CC) -c  $(CFLAGS) $< -o $@

main.o:main.cpp FM.h compress.o FMQ.o perfectHash.o
	g++ -c $(CFLAGS) main.cpp

clean:
	rm *.a *.o ./divsufsort/*.a ./divsufsort/*.o

div:
	make -C ./divsufsort/; cp divsufsort/libdivsufsort.a .;ar x libdivsufsort.a;rm libdivsufsort.a 
