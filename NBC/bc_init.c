#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>

int main(void)
{
  int i, j, n = 9000, me, np;
  double time1,time2,time3, s_read_time,e_read_time;
  double *x, *y, *z, start_time, end_time,s_comm_time,e_comm_time;
  MPI_Status s;
  FILE * fp[3];

  MPI_Init(NULL, NULL);

  MPI_Comm_rank(MPI_COMM_WORLD, &me);
  MPI_Comm_size(MPI_COMM_WORLD, &np);

  x = (double *)malloc(n*n*sizeof(double));
  y = (double *)malloc(n*n*sizeof(double));
  z = (double *)malloc(n*n*sizeof(double));

  if(me == 0)
    {
      fp[0] = fopen("./matrix_1.dat","rb");
      fp[1] = fopen("./matrix_2.dat","rb");
      fp[2] = fopen("./matrix_3.dat","rb");
    }

  start_time = MPI_Wtime();

  if(me == 0)
    {
      s_read_time = MPI_Wtime();
      for(i=0;i<n*n;i+=n)
	fread(&x[i],sizeof(double),n,fp[0]);
      e_read_time = MPI_Wtime();
    }
  s_comm_time = MPI_Wtime();
  MPI_Bcast(x,n*n,MPI_DOUBLE,0,MPI_COMM_WORLD);
  e_comm_time = MPI_Wtime();
  MPI_Barrier(MPI_COMM_WORLD); 

  end_time = MPI_Wtime();

  if(me == 0)
    printf("Communication time: %lf - Read time: %lf\n",e_comm_time - s_comm_time, e_read_time - s_read_time);

  time1 = end_time - start_time;

  start_time = MPI_Wtime();

  if(me == 0)
    {
      for(i=0;i<n*n;i+=n)
	fread(&y[i],sizeof(double),n,fp[1]);
    }

  MPI_Bcast(y,n*n,MPI_DOUBLE,0,MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD); 

  end_time = MPI_Wtime();

  time2 = end_time - start_time;

  start_time = MPI_Wtime();

  if(me == 0)
    {
      for(i=0;i<n*n;i+=n)
	fread(&z[i],sizeof(double),n,fp[2]);
    }

  MPI_Bcast(z,n*n,MPI_DOUBLE,0,MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD); 

  end_time = MPI_Wtime();

  time2 = end_time - start_time;

  if(me == 0)
    printf("Total time: %lf\n",time1 + time2+time3);

  MPI_Finalize();

  return 0;
}
