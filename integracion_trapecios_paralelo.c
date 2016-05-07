/* integracion_trapecios_no_paralelo.c
 * INTEGRACION NUMERICA POR EL METODO DE LOS TRAPECIOS
 * ENTRADA: NINGUNA.
 * SALIDA: ESTIMACION DE LA INTEGRAL DESDE a HASTA b DE f(x)
 * USANDO EL METODO DE LOS TRAPECIOS CON n TRAPECIOS */
#include <stdio.h>
#include <math.h>
#include <mpi.h>

#define ENVIO_FINAL 1

int main(int argc, char** argv)
{
    float integral; // RESULTADO DE LA INTEGRAL
    float a = -1.0; // EXTREMO IZQUIERDO
    float b = 1.0; // EXTREMO DERECHO
    int n = 1000; // NUMERO DE TRAPECIOS
    float h; // LONGITUD DE LA BASE DEL TRAPECIO
    float x;
    int i;
    float f(float x); /* FUNCION QUE ESTAMOS INTEGRANDO */
    // Anhadido para paralelizar
    int mi_id, nprocs;
    int n_local;
    float a_local, b_local;
    float integral_local;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &mi_id);

    h = (b-a)/n;


    n_local = n / nprocs;
    a_local = a + (mi_id*h*n_local);
    b_local = a + ((mi_id+1)*h*n_local);

    integral_local = (f(a_local) + f(b_local))/2.0;
    x = a_local;
    for (i = 1; i < n_local; i++) {
        x = a_local + (i*h);
        integral_local += f(x);
    }

    if (mi_id == 0) {
      integral = integral_local*h;
      for (i = 1; i < nprocs; i++) {
        MPI_Recv(&integral_local, 1, MPI_FLOAT, i, ENVIO_FINAL, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        integral += integral_local*h;
      }
      //integral = integral*h;
    } else {
      MPI_Send(&integral_local, 1, MPI_FLOAT, 0, ENVIO_FINAL, MPI_COMM_WORLD);
    }

    if (mi_id == 0) {
      printf("ESTIMACION USANDO n=%d TRAPECIOS,\n", n);
      printf("DE LA INTEGRAL DESDE %f HASTA %f = %f\n", a, b, integral);
      printf("\nESTIMACION DE PI: %f\n", 2 * integral);
    }
    MPI_Finalize();
} /* MAIN */

/* FUNCION QUE ESTAMOS INTEGRANDO */
float f(float x) {
    float return_val;
    /* CALCULA f(x) Y DEVUELVE SU VALOR */
    return_val = sqrt( 1 - x*x );
    return return_val;
} /* f */
