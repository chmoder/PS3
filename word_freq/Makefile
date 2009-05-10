wfc: list.o word.o main.o
	spu-g++ -Wall $^ -o $@

list.o: list.cpp list.h
	spu-g++ -Wall -c $< 

word.o: word.cpp word.h
	spu-g++ -Wall -c $<

main.o: main.cpp list.h
	spu-g++ -Wall -c $<

clean:
	-rm -f *.o wfc
