FLAGS = -Imatrix -Ifractions

solver: matrix.o fractions.o main.o
	gcc $(FLAGS) $^ -o solver -lm

debug: matrix.o fractions.o main.o
	gcc $(FLAGS) -g $^ -o solver

testMatrix: matrix.o testMatrix.o fractions.o
	gcc $(FLAGS) $^ -o testMatrix

main.o: main.c matrix/matrix.h fractions/fractions.h
	gcc $(FLAGS) -c main.c -o main.o

matrix.o: matrix/matrix.c matrix/matrix.h fractions/fractions.h
	gcc $(FLAGS) -c matrix/matrix.c -o matrix.o	

fractions.o: fractions/fractions.c fractions/fractions.h
	gcc $(FLAGS) -c fractions/fractions.c -o fractions.o

testMatrix.o: matrix/matrix.h matrix/testMatrix.c
	gcc $(FLAGS) -c matrix/testMatrix.c -o testMatrix.o

clean:
	rm -f ./*.o

.PHONY: clean