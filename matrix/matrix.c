#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"
#include "fractions.h"

MatFraction *createMatFraction(int rows, int columns, int* errCode) {
    MatFraction *new;
    int i;

    if (rows <= 0 || columns <= 0) {
        *errCode = -1;
        return NULL;
    }

    new = malloc(sizeof (MatFraction));
    
    if (new == NULL) {
        // malloc failed
        *errCode = -2;
        return NULL;
    }

    new->rows = rows;
    new->columns = columns;

    new->m = malloc(rows * sizeof(Fraction *));
    if ((new->m) == NULL) {
        // malloc failed
        free(new);
        *errCode = -3;
        return NULL;
    }
    
    for (i = 0; i < rows; i++) {
        new->m[i] = malloc(columns * sizeof(Fraction));
        if ((new->m[i]) == NULL) {
            // malloc failed
            for (--i; i >= 0; i--) free(new->m[i]);
            free(new->m);
            free(new);
            *errCode = -4;
            return NULL;
        }
    }

    *errCode = 0;
    return new;
}

void freeMatFraction(MatFraction *mat) {
    int i;

    if (mat == NULL || mat->m == NULL) return;

    for (i = 0; i < mat->rows; i++) {
        free(mat->m[i]);
    }

    free(mat->m);

    free(mat);
}

MatFraction *addMatFraction(MatFraction *mat1, MatFraction *mat2, int* errCode) {
    MatFraction *new;
    int i, j;
    
    if (mat1 == NULL || mat2 == NULL || mat1->m == NULL || mat2->m == NULL) {
        *errCode = -5;
        return NULL;
    }

    if (mat1->rows != mat2->rows || mat1->columns != mat2->columns) {
        printf("%d==%d %d==%d\n", mat1->rows, mat2->rows, mat1->columns, mat2->columns);
        *errCode = -6;
        return NULL;
    }

    new = createMatFraction(mat1->rows, mat1->columns, errCode);

    if (*errCode < 0)
        return NULL;

    for (i = 0; i < mat1->rows; i++) {
        for (j = 0; j < mat1->columns; j++) {
            new->m[i][j] = add(mat1->m[i][j], mat2->m[i][j]);
        }
    }

    *errCode = 0;
    return new;
}

MatFraction *scalarProduct(MatFraction *mat, int scalar, int* errCode) {
    MatFraction *new;
    int i, j;

    if (mat == NULL || mat->m == NULL) {
        *errCode = -7;
        return NULL;
    }

    new = createMatFraction(mat->rows, mat->columns, errCode);

    if (*errCode < 0) 
        return NULL;

    for (i = 0; i < mat->rows; i++) {
        for (j = 0; j < mat->columns; j++) {
            Fraction f = {scalar, 1};
            new->m[i][j] = mul(f, mat->m[i][j]);
        }
    }
    
    *errCode = 0;
    return new;
}

MatFraction *subMatFraction(MatFraction *mat1, MatFraction *mat2, int* errCode) {
    MatFraction *new, *aux;

    aux = scalarProduct(mat2, -1, errCode);
    
    if (*errCode < 0)
        return NULL;
    
    // here, aux is correctly allocated
    new = addMatFraction(mat1, aux, errCode);

    if (*errCode < 0) {
        freeMatFraction(aux); 
        return NULL;
    }

    // here new is correctly set, and aux has to be freed
    freeMatFraction(aux);

    return new;
}

MatFraction *mulMatFraction(MatFraction *mat1, MatFraction *mat2, int* errCode) {
    MatFraction *new;
    Fraction res;
    int i, j, x, y;
    
    if (mat1 == NULL || mat2 == NULL || mat1->m == NULL || mat2->m == NULL) {
        *errCode = -5;
        return NULL;
    }

    if (mat1->columns != mat2->rows) {
        *errCode = -6;
        return NULL;
    }

    new = createMatFraction(mat1->rows, mat2->columns, errCode);

    if (*errCode < 0) 
        return NULL;

    for (i = 0; i < new->rows; i++) {
        for (j = 0; j < new->columns; j++) {
            res = (Fraction) {0, 1};

            for (x = 0; x < mat1->columns; x++) {
                res = add(res, mul(mat1->m[i][x], mat2->m[x][j]));
            }

            new->m[i][j] = res;
        }
    }

    *errCode = 0;
    return new;
}

void fillRandomInt(MatFraction *mat, int max, int* errCode) {
    Fraction new;
    int i, j;
    
    max++;

    if (mat == NULL || mat->m == NULL) {
        *errCode = -1;
        return;
    }

    for (i = 0; i < mat->rows; i++) {
        for (j = 0; j < mat->columns; j++) {
            new.numerator = rand() % max;
            new.denominator = 1;

            mat->m[i][j] = new;
        }
    }

    *errCode = 0;
}

void printMatFraction(MatFraction *mat, int* errCode) {
    int i, j;
    if (mat == NULL || mat->m == NULL) {
        *errCode = -1;
        return;
    }

    for (i = 0; i < mat->rows; i++) {
        printf("%g", decValue(mat->m[i][0]));
        for (j = 1; j < mat->columns; j++) {
            printf(" %g", decValue(mat->m[i][j]));
        }
        printf("\n");
    }
    *errCode = 0;
}

MatFraction* inverseMatrix(MatFraction* mat, int *errCode) {
    MatFraction *new, *res;
    Fraction current;
    int found;
    int i, j, k;

    if (mat == NULL || mat->m == NULL) {
        *errCode = -5;
        return NULL;
    }

    if (mat->rows != mat->columns) {
        *errCode = -6;
        return NULL;
    }

    new = createMatFraction(mat->rows, 2 * mat->columns, errCode);
    if (*errCode < 0)
        return NULL;

    res = createMatFraction(mat->rows, mat->columns, errCode);
    if (*errCode < 0)
        return NULL;
    
    // copy the old matrix
    for (i = 0; i < mat->rows; i++) {
        for (j = 0; j < mat->columns; j++) {
            new->m[i][j] = mat->m[i][j];
        }
    }

    // identity matrix
    for (i = 0; i < mat->rows; i++) {
        for (j = mat->columns; j < 2 * mat->columns; j++) {
            if (i == j-mat->columns) new->m[i][j] = (Fraction) {1, 1};
            else new->m[i][j] = (Fraction) {0, 1};
        }
    }
    
    for (i = 0; i < mat->rows; i++) {
        current = new->m[i][i];

        if (current.numerator == 0) {
            found = 0;
            // This row does not fit this position.
            // Swap it with a row whose i-element is non 0.
            // If every one of these are 0, the matrix is impossible to invert.
            for (j = i; j < new->rows; j++) {
                if (new->m[j][i].numerator != 0) {
                    found = 1;
                    swapRows(new, i, j, errCode);
                    if (*errCode < 0) return NULL;
                    current = new->m[i][i];
                    break;
                }
            }
            if (!found) {
                *errCode = -7;
                return NULL;
            }
        }
        
        // place a 1 in the diagonal, changing all the file
        for (j = 0; j < 2 * mat->columns; j++) {
            new->m[i][j] = divide(new->m[i][j], current);
        }
        
        // place 0s in the rest of the elements of the column with valid file operations
        for (j = 0; j < mat->rows; j++) {
            if (j == i) continue;
            current = new->m[j][i];
            for (k = 0; k < 2 * mat->columns; k++) {
                new->m[j][k] = sub(new->m[j][k], mul(current, new->m[i][k]));
            }
        }
    }

    // in the last mat->rows columns of the matrix new, we have the inverse matrix
    for (i = 0; i < mat->rows; i++) {
        for (j = 0; j < mat->columns; j++) {
            res->m[i][j] = new->m[i][j+mat->columns];
        }
    }
    
    free(new);

    *errCode = 0;
    return res;
}

void swapRows(MatFraction *mat, int rowA, int rowB, int *errCode) {
    Fraction temp;
    int j;
    if (mat == NULL || mat->m == NULL) {
        *errCode = -5;
        return;
    }

    if (rowA < 0 || rowA >= mat->rows || rowB < 0 || rowB >= mat->rows) {
        *errCode = -6;
        return;
    }

    for (j = 0; j < mat->columns; j++) {
        temp = mat->m[rowB][j];
        mat->m[rowB][j] = mat->m[rowA][j];
        mat->m[rowA][j] = temp;
    }
    
    *errCode = 0;
}

MatFraction* submatrix(MatFraction *mat, int initialRow, int initialColumn, int finalRow, int finalColumn, int *errCode) {
    MatFraction* new;
    int i, j;

    if (mat == NULL || mat->m == NULL) {
        *errCode = -5;
        return NULL;
    }

    if (initialRow < 0 || initialRow >= mat->rows ||
        initialColumn < 0 || initialColumn >= mat->columns ||
        finalRow < 0 || finalRow >= mat->rows ||
        finalColumn < 0 || finalColumn >= mat->columns ||
        initialRow > finalRow ||
        initialColumn > finalColumn) 
    {
        *errCode = -6;
        return NULL;
    }


    new = createMatFraction(finalRow - initialRow + 1, finalColumn - initialColumn + 1, errCode);

    if (*errCode < 0) 
        return NULL;

    for (i = initialRow; i <= finalRow; i++) {
        for (j = initialColumn; j <= finalColumn; j++) {
            new->m[i-initialRow][j-initialColumn] = mat->m[i][j];
        }
    }

    *errCode = 0;
    return new;
}

MatFraction* getColumn(MatFraction* mat, int column, int *errCode) {
    MatFraction* new = submatrix(mat, 0, column, mat->rows, column, errCode);
    if (errCode < 0) return NULL;
    return new;
}