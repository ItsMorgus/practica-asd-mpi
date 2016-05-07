/* integracion_trapecios_no_paralelo.c
 * INTEGRACION NUMERICA POR EL METODO DE LOS TRAPECIOS
 * ENTRADA: NINGUNA.
 * SALIDA: ESTIMACION DE LA INTEGRAL DESDE a HASTA b DE f(x)
 * USANDO EL METODO DE LOS TRAPECIOS CON n TRAPECIOS */
#include <stdio.h>
#include <math.h>

main(int argc, char** argv)
{
    float integral; // RESULTADO DE LA INTEGRAL
    float a = -1.0; // EXTREMO IZQUIERDO
    float b = 1.0; // EXTREMO DERECHO
    int n = 1000; // NUMERO DE TRAPECIOS
    float h; // LONGITUD DE LA BASE DEL TRAPECIO
    float x;
    float i;
    float f(float x); /* FUNCION QUE ESTAMOS INTEGRANDO */
    h = (b-a)/n;
    integral = (f(a) + f(b))/2.0;
    x = a;
    for (i = 1; i <= n-1; i++) {
        x = x + h;
        integral = integral + f(x);
    }
    integral = integral*h;
    printf("ESTIMACION USANDO n=%d TRAPECIOS,\n", n);
    printf("DE LA INTEGRAL DESDE %f HASTA %f = %f\n", a, b, integral);
    printf("\nESTIMACION DE PI: %f\n", 2 * integral);
} /* MAIN */

/* FUNCION QUE ESTAMOS INTEGRANDO */
float f(float x) {
    float return_val;
    /* CALCULA f(x) Y DEVUELVE SU VALOR */
    return_val = sqrt( 1 - x*x );
    return return_val;
} /* f */
