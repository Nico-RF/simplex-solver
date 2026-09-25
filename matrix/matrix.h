#ifndef MATRIX_H
#define MATRIX_H
#include "fractions.h"

typedef struct MatFraction {
    int rows, columns;
    Fraction ** m;
} MatFraction;

MatFraction *createMatFraction(int rows, int columns, int* errCode);
void freeMatFraction(MatFraction *mat);
MatFraction *addMatFraction(MatFraction *mat1, MatFraction *mat2, int* errCode);
MatFraction *scalarProduct(MatFraction *mat, int scalar, int* errCode);
MatFraction *subMatFraction(MatFraction *mat1, MatFraction *mat2, int* errCode);
MatFraction *mulMatFraction(MatFraction *mat1, MatFraction *mat2, int* errCode);
void fillRandomInt(MatFraction *mat, int max, int* errCode);
void printMatFraction(MatFraction *mat, int* errCode);
MatFraction* inverseMatrix(MatFraction* mat, int *errCode);
void swapRows(MatFraction *mat, int rowA, int rowB, int *errCode);
MatFraction* submatrix(MatFraction *mat, int initialRow, int initialColumn, int finalRow, int finalColumn, int *errCode);
MatFraction* getColumn(MatFraction* mat, int column, int *errCode);
#endif