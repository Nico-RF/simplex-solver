# *SIMPLEX SOLVER*: Solucionador de sistemas de ecuaciones con el método del símplex.
Este proyecto puede serle útil para las asignaturas de Álgebra Computacional (101106) y de Investigación Operativa I (108409) de los Grados en Ing. Informática y en Estadística, respectivamente, de la USAL. 

<br />

*BLANCO DE LA IGLESIA, DAVID*. [@Ugulberto](https://github.com/Ugulberto)

<br />

## Funcionalidad:
Se desea minimizar o maximizar una función, z(x_i), sujeta a restricciones o bien polinómicas, o bien de signo de las variables. 

Este proyecto resuelve tanto el método tabular como el matricial, ya que funciona introduciendo coeficientes.

Pasos a seguir:

1. Introduce el número de variables (sin incluir las de exceso ni las de holgura): (A)<br />
2. Introduce el número de restricciones (excepto las de x_1>=0, x_2>=0...):        (B)<br />
3. Introduce la función objetivo:<br />
    a) ¿Quieres maximizar (M) o minimizar (m) tu función?:<br />
    b) Introduce el coeficiente x_1:<br />
    c) Introduce el coeficiente x_2:<br />
    ...<br />
4. Introduce los coeficientes de restricción:<br />
    RO) Restricción 0:<br />
    &nbsp;&nbsp;&nbsp;&nbsp;a) Coeficiente x_1:<br />
    &nbsp;&nbsp;&nbsp;&nbsp;b) Coeficiente x_2:<br />
    &nbsp;&nbsp;&nbsp;&nbsp;...<br />
    &nbsp;&nbsp;&nbsp;&nbsp;A) Coeficiente x_A:<br />
    &nbsp;&nbsp;&nbsp;&nbsp;Introduce el signo de la desigualdad: (> es >= y < es <=)<br />
    &nbsp;&nbsp;&nbsp;&nbsp;Introduce el valor del miembro de la derecha (>0): <br />
    R1) Restricción 1:<br />
    &nbsp;&nbsp;&nbsp;&nbsp;a) Coeficiente x_1:<br />
    &nbsp;&nbsp;&nbsp;&nbsp;...<br />
    ...<br />
    RB) Restricción B:<br />
    ...<br />
<br />


<br />

## Ejemplo de salida:
    M       z       x_1     x_2     s_1     s_2     RHS     BV      Ratio
    -------------------------------------------------------------------------
    R0      1       -1      -2      0       0       0       z       N/A
    R1      0       1       1       1       0       3       s_1     N/A
    R2      0       1       1       0       1       4       s_2     N/A
    
    
    --- ITERATION NUMBER 1 ---
    
    --- AFTER DIVISION ---
    M       z       x_1     x_2     s_1     s_2     RHS     BV      Ratio
    -------------------------------------------------------------------------
    R0      1       -1      -2      0       0       0       z       N/A
    R1      0       1       1       1       0       3       x_2     3
    R2      0       1       1       0       1       4       s_2     4
    
    --- AFTER GAUSS-JORDAN ---
    M       z       x_1     x_2     s_1     s_2     RHS     BV      Ratio
    -------------------------------------------------------------------------
    R0      1       1       0       2       0       6       z       N/A
    R1      0       1       1       1       0       3       x_2     3
    R2      0       0       0       -1      1       1       s_2     4
    
    Basic Feasable Solution 1: 
    z(x_1,x_2) = F(0,3) = 6
    
    
    --- SOLUTION ---
    z(x_1,x_2) = F(0,3) = 6

<br />

## Arquitectura del proyecto
    simplex-solver/      # Root del proyecto
    │
    ├── fractions/       # Tratamiento de fracciones en las iteraciones
    │   ├── fractions.c   
    │   └── fractions.h   
    │
    ├── matrix/          # Tratamiento de matrices
    │   ├── matrix.c         
    │   ├── matrix.h         
    │   └── textMatrix.c    
    │
    ├── makefile         # Órdenes make para ejecutar el proyecto
    ├── main.c           # Archivo MAIN: menú principal y parrilleo de subrutinas
    └── readme.md        # Guía de lectura del proyecto

## Ejecución:
Disponemos de un archivo makefile para facilitar la ejecución del programa. 
El makefile servirá para ejecutar y borrar los archivos matrix.o, fractions.o, main.o y solver. Para que funcione deberemos poner por terminal:
> make

y posteriormente ejecutar

> ./solver

<br />


## Disclaimer:
1.° Se reservan todos los derechos de autoría de este proyecto bajo amparo del *Real Decreto Legislativo 1/1996, de 12 de abril, por el que se aprueba el texto refundido de la **Ley de Propiedad Intelectual**, regularizando, aclarando y armonizando las disposiciones legales vigentes sobre la materia.*
