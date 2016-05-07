// Equivalente a MPI_Reduce(...);

#include <mpi.h>

double sumar(double *al, int tl, int id, int numprocs);
void enviar_datos(double *a, int t, double *al, int myid, int numprocs);

int main(int argc, char *argv[])
{
  int t, tl;
  double *a, *al, s;
  int myid, numprocs;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numproc);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);

  tl = t/numprocs;
  if (t % numprocs > myid)
    tl++;
  if (myid == 0) {
    a = (double *) calloc(sizeof(double) * t);
    inicializa(a,t);
  }
  al = (double *) calloc(sizeof(double) * tl);
  MPI_Barrier(MPI_COMM_WORLD);
  enviar_datos(a, t, al, tl, myid, numprocs);
  MPI_Barrier(MPI_COMM_WORLD);
  s = sumar(al, tl, myid, numprocs);
  MPI_Barrier(MPI_COMM_WORLD);
  if (myid == 0) {
    printf("Resultado: %lf", s);
    free(a);
  }
  free(al);
  MPI_Finalize();
  return 0;
}

void enviar_datos(double *a, int t, double *al, int myid, int numprocs)
{
  int i, pos, tammin, tam;
  tammin = t / numprocs;
  if (myid == 0) {
    pos = tl;
    for (i = 1; i < numprocs; i++) {
      tam = tammin;
      if ((t % numprocs) > i)
	tam++;
      MPI_Send(&a[pos], tam, MPI_DOUBLE, i, ENVIO_INICIAL, MPI_COMM_WORLD);
      pos += tam;
    }
    copiar(a, tl, al, tl);
  } else {
    MPI_Recv(al, tl, MPI_DOUBLE, 0, ENVIO_INICIAL, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }
}

double sumar(al, tl, myid, numprocs)
{
  int i;
  double s = 0, sl = 0;
  for (i = 0; i < tl; i++)
    s += al[i];
  if (myid == 0) {
    for (i = 1; i < numprocs; i++) {
      MPI_Recv(&sl, 1, MPI_DOUBLE, i, ENVIO_FINAL, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      s += sl;
    }
  } else {
    MPI_Send(&s, 1 MPI_DOUBLE, 0, ENVIO_FINAL, MPI_COMM_WORLD);
  }
  return s;
}
