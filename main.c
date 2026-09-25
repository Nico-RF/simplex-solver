#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#include "fractions.h"

/*

    The problem must be entered in standard form.
    It is assumed that every single variable is >= 0.

*/

void evaluateErrCode(int errCode);
void printTable(MatFraction* table, char maxMin, int numberVars, int numberRestrictions);
void printSolution(Fraction* baseMatrix, Fraction* functionCoeffs, int numberVars);

int main() {
    // Problem parameters
    int numberVars = 0, numberRestrictions = 0;
    char maxMin = 'M';
    char sign = '<';
    int negMin = -1;
    int negSign = -1;
    int decimalDigits = 7;

    // Iterators
    int i, j, k;
    
    // Matrix columns
    int z_col = 0, ld_col = 0, bv_col = 0, ratio_col = 0;
    
    // Auxiliar variables
    int errCode = 0;
    int iterationCounter = 1;
    int varIndex = 0;
    Fraction currentFraction;

    // Data structures
    Fraction *baseMatrix;
    MatFraction *simplexMatrix;
    Fraction *functionCoeffs;
    Fraction **matrix;

    // Search-aimed variables
    Fraction minR0;
    int columnMinR0;
    Fraction minRatio;
    int rowMinRatio;


    // Read the problem parameters
    printf("Enter the number of variables (not including slack and surplus variables): ");
    scanf("%d", &numberVars);

    printf("Enter the number of restrictions (except x_1>=0, x_2>=0...): ");
    scanf("%d", &numberRestrictions);

    z_col = 0;
    ld_col = numberVars+numberRestrictions+1;
    bv_col = ld_col+1;
    ratio_col = bv_col+1;
    baseMatrix = calloc(numberVars + numberRestrictions, sizeof (Fraction));
    if (baseMatrix == NULL) evaluateErrCode(-1);
    functionCoeffs = malloc(numberVars * sizeof(Fraction));
    if (functionCoeffs == NULL) evaluateErrCode(-1);

    simplexMatrix = createMatFraction(numberRestrictions + 1, 1 + numberVars + numberRestrictions + 3, &errCode);
    evaluateErrCode(errCode);
    matrix = simplexMatrix->m;

    fillRandomInt(simplexMatrix, 0, &errCode); // fills with zeros
    evaluateErrCode(errCode);

    printf("\nEnter the objective function: \n");
    printf("Do you want to maximize (M) or minimize (m) your function?: ");
    scanf("%*c%c", &maxMin);

    if (maxMin == 'm') negMin = 1;
    else negMin = -1;

    matrix[0][0] = (Fraction) {1, 1}; // as the z variable is part of the BV 

    for (i = 0; i < numberVars; i++) {
        printf("Introduce the coefficient x_%d: ", i+1);
        currentFraction = readFraction(decimalDigits);
        matrix[0][i+1] = mul(currentFraction, (Fraction) {negMin, 1});
        functionCoeffs[i] = currentFraction;
    }

    printf("\nIntroduce the restriction coefficients:");
    for (i = 0; i < numberRestrictions; i++) {
        printf("\n\nRestriction %d\n", i);
        for (j = 0; j < numberVars; j++) {
            printf("Coefficient x_%d: ", j+1);
            currentFraction = readFraction(decimalDigits);
            matrix[i+1][j+1] = currentFraction;
        }

        printf("Introduce the sign of the inequality (> is >= and < is <=): ");
        scanf("%*c%c", &sign);
        if (sign == '>') negSign = -1;
        else negSign = 1;

        matrix[i+1][numberVars+i+1] = (Fraction) {negSign, 1};

        printf("Introduce the right-hand side value (> 0): ");
        currentFraction = readFraction(decimalDigits);
        matrix[i+1][ld_col] = currentFraction;
    }

    matrix[0][bv_col] = (Fraction) {0, 1};
    matrix[0][ratio_col] = (Fraction) {-1, 1};

    for (i = 1; i <= numberRestrictions; i++) {
        matrix[i][bv_col] = (Fraction) {i+numberVars, 1};
        matrix[i][ratio_col] = (Fraction) {-1, 1};
    }

    // Print the initial table
    printTable(simplexMatrix, maxMin, numberVars, numberRestrictions);

    // get the minimum value
    minR0 = matrix[0][1];
    columnMinR0 = 1;

    for (i = 1; i <= numberVars; i++) {
        if (greater(minR0, matrix[0][i])) {
            minR0 = matrix[0][i];
            columnMinR0 = i;
        }
    }
    
    printf("\n");

    while (greater((Fraction){0, 1}, minR0)) { // solution can be improved

        minR0 = (Fraction) {9999999, 1};
        
        // --- ITERATION ---
        // Calculate the ratios
        printf("--- ITERATION NUMBER %d ---\n\n", iterationCounter);
        minRatio = (Fraction) {9999999, 1};
        rowMinRatio = -1;
        
        for (i = 1; i <= numberRestrictions; i++) {
            if (matrix[i][columnMinR0].numerator <= 0) continue;
            
            matrix[i][ratio_col] = divide(matrix[i][ld_col], matrix[i][columnMinR0]);
            
            if (greater(minRatio, matrix[i][ratio_col])) {
                minRatio = matrix[i][ratio_col];
                rowMinRatio = i;
            }
        }

        if (rowMinRatio == -1) {
            printf("\nEl problema introducido no tiene solución (no está acotado).\n");
            break;
        }
        
        // the variable rowMinRatio enters into the basis
        matrix[rowMinRatio][bv_col] = (Fraction) {columnMinR0, 1};

        // Divide by the coefficient at columnMinR0 the whole rowMinRatio
        currentFraction = matrix[rowMinRatio][columnMinR0];
        
        for (i = 1; i <= ld_col; i++) {
            matrix[rowMinRatio][i] = divide(matrix[rowMinRatio][i], currentFraction);
        }

        printf("--- AFTER DIVISION ---");
        printTable(simplexMatrix, maxMin, numberVars, numberRestrictions);

        // R_i' = R_i - R_i{columnMinR0}*R_{rowMinRatio}

        for (i = 0; i < 1 + numberRestrictions; i++) {
            if (i == rowMinRatio) continue;
            currentFraction = matrix[i][columnMinR0];
            for (j = 1; j <= ld_col; j++) {
                matrix[i][j] = sub(matrix[i][j], mul(currentFraction, matrix[rowMinRatio][j]));
            }
        }

        // Search new minimum element
        for (i = 1; i < ld_col; i++) {
            if (greater(minR0, matrix[0][i])) {
                minR0 = matrix[0][i];
                columnMinR0 = i;
            }
        }

        printf("--- AFTER GAUSS-JORDAN ---");
        printTable(simplexMatrix, maxMin, numberVars, numberRestrictions);

        // Update basis solution
        for (k = 0; k < numberVars + numberRestrictions; k++) {
            baseMatrix[k] = (Fraction){0, 1};
        }

        for (k = 1; k <= numberRestrictions; k++) {
            varIndex = matrix[k][bv_col].numerator - 1;
            baseMatrix[varIndex] = matrix[k][ld_col];
        }

        printf("Basic Feasable Solution %d: \n", iterationCounter);
        printSolution(baseMatrix, functionCoeffs, numberVars);
        printf("\n\n");

        iterationCounter++;
    }

    printf("--- SOLUTION ---\n");
    printSolution(baseMatrix, functionCoeffs, numberVars);

    free(simplexMatrix);
    free(baseMatrix);
    free(functionCoeffs);

    return 0;
}

/* 

2 2 M 4 1 8 2 < 16 5 2 < 12
2 2 M 4 1 8 2 16 5 2 12
3 4 M 60 30 20 8 6 1 48 4 2 1.5 20 2 1.5 0.5 8 0 1 0 5
3 4 M 60 30 20 8 6 1 < 48 4 2 1.5 < 20 2 1.5 0.5 < 8 0 1 0 < 5

*/

void evaluateErrCode(int errCode) {
    if (errCode == 0) return;
    exit(errCode);
}

void printTable(MatFraction* table, char maxMin, int numberVars, int numberRestrictions) {
    int i, j;
    int ld_col = numberVars + numberRestrictions + 1; // Columna RHS
    int bv_col = ld_col + 1;                          // Columna de variable básica
    int ratio_col = bv_col + 1;                       // Columna de la razón
    int varInd;

    // --- ENCABEZADO DE LA TABLA ---
    printf("\n%c\tz\t", maxMin);
    
    // Encabezados de decisiones (x_1, x_2, ...)
    for (i = 0; i < numberVars; i++) {
        printf("x_%d\t", i + 1);
    }

    // Encabezados de holguras (s_1, s_2, ...)
    for (i = 0; i < numberRestrictions; i++) {
        printf("s_%d\t", i + 1);
    }

    printf("RHS\tBV\tRatio\n");
    for (i = 0; i < 8 * (5 + numberRestrictions + numberVars) + 1; i++) 
        printf("-");
    printf("\n");

    // --- FILAS DE LA TABLA ---
    for (i = 0; i <= numberRestrictions; i++) {
        printf("R%d\t", i);

        // Imprimir SOLO los datos numéricos de la matriz (desde columna Z hasta RHS)
        for (j = 0; j <= ld_col; j++) {
            printf("%.4g\t", decValue(table->m[i][j]));
        }
        
        // --- COLUMNA BV (Variable Básica) ---
        if (i == 0) {
            printf("z\t"); // La fila 0 siempre representa a Z
        } else {
            varInd = table->m[i][bv_col].numerator;
            
            if (varInd >= 1 && varInd <= numberVars) {
                // Variables de decisión: x_1, x_2...
                printf("x_%d\t", varInd);
            } else if (varInd > numberVars) {
                // Variables de holgura: s_1, s_2... (Ajustamos el índice)
                printf("s_%d\t", varInd - numberVars);
            }
        }
        
        // --- COLUMNA RATIO ---
        if (i == 0 || table->m[i][ratio_col].numerator < 0) {
            printf("N/A\n");
        } else {
            printf("%.4g\n", decValue(table->m[i][ratio_col]));
        }
    }
    printf("\n");
}

void printSolution(Fraction* baseMatrix, Fraction* functionCoeffs, int numberVars) {
    int i;
    Fraction current, total = mul(baseMatrix[0], functionCoeffs[0]);

    printf("z(x_1");
    for (i = 1; i < numberVars; i++) {
        printf(",x_%d", i+1);
    }
    printf(") = F(%g", decValue(baseMatrix[0]));
    for (i = 1; i < numberVars; i++) {
        current = baseMatrix[i];
        total = add(total, mul(current, functionCoeffs[i]));
        printf(",%g", decValue(current));
    }
    printf(") = %g\n", decValue(total));
}