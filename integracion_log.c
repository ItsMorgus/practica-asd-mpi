/* integracion_trapecios_no_paralelo.c
 * INTEGRACION NUMERICA POR EL METODO DE LOS TRAPECIOS
 * ENTRADA: NINGUNA.
 * SALIDA: ESTIMACION DE LA INTEGRAL DESDE a HASTA b DE f(x)
 * USANDO EL METODO DE LOS TRAPECIOS CON n TRAPECIOS */
#include <stdio.h>
#include <math.h>
#include <mpi.h>

#define ENVIO_FINAL 1
#define REPETICIONES 100

int main(int argc, char** argv)
{
    double integral; // RESULTADO DE LA INTEGRAL
    double a = 1.0; // EXTREMO IZQUIERDO
    double b = 100000.0; // EXTREMO DERECHO
    int n = 2*2*2*2*3*3*5*7*11*11*13; // NUMERO DE TRAPECIOS
    double h; // LONGITUD DE LA BASE DEL TRAPECIO
    double x;
    int i, j;
    double f(double x); /* FUNCION QUE ESTAMOS INTEGRANDO */
    // Anhadido para paralelizar
    int mi_id, nprocs;
    int n_local;
    double a_local, b_local;
    double integral_local;
    double tiempos[REPETICIONES];
    double media_tiempos, s;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &mi_id);

    for (j = 0; j < REPETICIONES; j++) {
      tiempos[j] = MPI_Wtime();
      h = (b-a)/n;

      n_local = n / nprocs;
      a_local = a + (mi_id*h*n_local);
      b_local = a + ((mi_id+1)*h*n_local);

      integral_local = (f(a_local) + f(b_local))/2.0 * h;
      x = a_local;
      for (i = 1; i < n_local; i++) {
	x = a_local + ((double) i)*h;
	integral_local += f(x)*h;
      }

      if (mi_id == 0) {
	integral = integral_local;
	for (i = 1; i < nprocs; i++) {
	  MPI_Recv(&integral_local, 1, MPI_DOUBLE, i, ENVIO_FINAL, MPI_COMM_WORLD,
		   MPI_STATUS_IGNORE);
	  integral += integral_local;
	}
	//      integral = integral*h;
      } else {
	MPI_Send(&integral_local, 1, MPI_DOUBLE, 0, ENVIO_FINAL, MPI_COMM_WORLD);
      }

      tiempos[j] = MPI_Wtime() - tiempos[j];

      if (mi_id == 0) {
	if (j == 0) {
	  printf("Cada punto es una repeticion\n");
	}
	printf(".");
	fflush(stdout);
	if ((j+1) % 10 == 0) {
	  printf("\n");
	}
      }
    }
    if (mi_id == 0) {
      printf("ESTIMACION USANDO n=%d TRAPECIOS,\n", n);
      printf("DE LA INTEGRAL DESDE %f HASTA %f = %f\n", a, b, integral);
      s = 0.;
      for (j = 0; j < REPETICIONES; j++) {
	s += tiempos[j];
      }
      media_tiempos = s / (double) REPETICIONES;
      printf("%f\n", tiempos[13]);
      printf("Media de tiempo con %d repeticiones: %f\n", REPETICIONES, media_tiempos);
    }
    MPI_Finalize();
} /* MAIN */

/* FUNCION QUE ESTAMOS INTEGRANDO */
double f(double x) {
    double return_val;
    /* CALCULA f(x) Y DEVUELVE SU VALOR */
    return_val = log(x);
    return return_val;
} /* f */
