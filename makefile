
CC = g++ -O2 -std=c++14

OBJ = *.o  cluster search test
DEPS = fileHandler.h
all:    cluster search test





cluster: proj2cluster.o  fileHandler.o	vHash.o hashFunctions.o cube_class.o cubefun.o bintree.o frechet.o config.o curve.o interval.o point.o simplification.o
	$(CC) -o cluster proj2cluster.o  fileHandler.o vHash.o hashFunctions.o cube_class.o cubefun.o bintree.o frechet.o config.o curve.o interval.o point.o simplification.o

search: proj2main.o  fileHandler.o	vHash.o hashFunctions.o cube_class.o cubefun.o bintree.o frechet.o config.o curve.o interval.o point.o simplification.o
	$(CC) -o search proj2main.o  fileHandler.o vHash.o hashFunctions.o cube_class.o cubefun.o bintree.o frechet.o config.o curve.o interval.o point.o simplification.o

test: testing.o fileHandler.o vHash.o hashFunctions.o cube_class.o cubefun.o bintree.o frechet.o config.o curve.o interval.o point.o simplification.o
	$(CC) -o test testing.o  fileHandler.o vHash.o hashFunctions.o cube_class.o cubefun.o bintree.o frechet.o config.o curve.o interval.o point.o simplification.o

*.o : *.cpp
		$(CC) *.cpp -c


clean:
	rm -f $(OBJ)
