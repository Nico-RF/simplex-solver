#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "matrix.h"

void evaluateErrCode(int *errCode) {
    if (*errCode == 0) return;
    printf("Exited with error code %d.\n", *errCode);
    exit(*errCode);
}

int main() {
    srand(time(NULL));
    MatFraction *m1, *m2, *m3, *mMul, *mInv, *id;
    int errCode;

    m1 = createMatFraction(2, 3, &errCode); 
    evaluateErrCode(&errCode);
    m2 = createMatFraction(2, 3, &errCode);
    evaluateErrCode(&errCode);
    mMul = createMatFraction(3, 2, &errCode);
    evaluateErrCode(&errCode);
    mInv = createMatFraction(3, 3, &errCode);
    evaluateErrCode(&errCode);

    fillRandomInt(m1, 3, &errCode);
    evaluateErrCode(&errCode);

    fillRandomInt(m2, 3, &errCode);
    evaluateErrCode(&errCode);

    fillRandomInt(mMul, 3, &errCode);
    evaluateErrCode(&errCode);

    fillRandomInt(mInv, 3, &errCode);
    evaluateErrCode(&errCode);

    printMatFraction(m1, &errCode);
    evaluateErrCode(&errCode);

    printf("\n");

    printMatFraction(m2, &errCode);
    evaluateErrCode(&errCode);

    printf("\n");

    printMatFraction(mMul, &errCode);
    evaluateErrCode(&errCode);

    printf("\n");

    printMatFraction(mInv, &errCode);
    evaluateErrCode(&errCode);

    printf("\n");

    m3 = addMatFraction(m1, m2, &errCode);
    evaluateErrCode(&errCode);

    printf("Resultado de la suma: \n");
    printMatFraction(m3, &errCode);
    evaluateErrCode(&errCode);

    printf("\n");

    printf("Resultado de la resta: \n");
    m3 = subMatFraction(m1, m2, &errCode);
    evaluateErrCode(&errCode);

    printMatFraction(m3, &errCode);
    evaluateErrCode(&errCode);

    printf("\nResultado de la multiplicación: \n");
    m3 = mulMatFraction(m1, mMul, &errCode);
    evaluateErrCode(&errCode);

    printMatFraction(m3, &errCode);
    evaluateErrCode(&errCode);

    printf("\nMatriz inversa:\n");
    m3 = inverseMatrix(mInv, &errCode);
    evaluateErrCode(&errCode);

    printMatFraction(m3, &errCode);
    evaluateErrCode(&errCode);

    printf("\nProducto por matriz inversa:\n");

    id = mulMatFraction(mInv, m3, &errCode);
    evaluateErrCode(&errCode);

    printMatFraction(id, &errCode);
    evaluateErrCode(&errCode);

    freeMatFraction(m1);
    freeMatFraction(m2);
    freeMatFraction(m3);
    freeMatFraction(mMul);
    freeMatFraction(mInv);
    freeMatFraction(id);

    return 0;
}