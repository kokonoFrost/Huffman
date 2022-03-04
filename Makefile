obj = huffman.o bitfile.o

huffman : $(obj)
	c++ -o huffman $(obj)
huffman.o : bitfile.h
bitfile.o : bitfile.h

.PHONY : clean
clean:
	-rm huffman $(obj)