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
  MPI_Request req[n];

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

  j = 0;
  
  start_time = MPI_Wtime();

  for(i=0;i<n*n;i+=n)
    {
      if(me == 0)
	fread(&x[i],sizeof(double),n,fp[0]);
      MPI_Ibcast(&x[i],n,MPI_DOUBLE,0,MPI_COMM_WORLD,&req[j]);
      j += 1;
    }

  MPI_Waitall(n,req,MPI_STATUS_IGNORE);

  end_time = MPI_Wtime();

  time1 = end_time - start_time;

  if(me == 0)
    printf("Communication and read time: %lf\n",time1);

  j = 0;

  start_time = MPI_Wtime();

  for(i=0;i<n*n;i+=n)
    {
      if(me == 0)
	fread(&y[i],sizeof(double),n,fp[1]);
      MPI_Ibcast(&y[i],n,MPI_DOUBLE,0,MPI_COMM_WORLD,&req[j]);
      j += 1;
    }

  MPI_Waitall(n,req,MPI_STATUS_IGNORE); 

  end_time = MPI_Wtime();

  time2 = end_time - start_time;

  j = 0;

  start_time = MPI_Wtime();

  for(i=0;i<n*n;i+=n)
    {
      if(me == 0)
	fread(&z[i],sizeof(double),n,fp[2]);
      MPI_Ibcast(&z[i],n,MPI_DOUBLE,0,MPI_COMM_WORLD,&req[j]);
      j += 1;
    }

  MPI_Waitall(n,req,MPI_STATUS_IGNORE);

  end_time = MPI_Wtime();

  time2 = end_time - start_time;

  if(me == 0)
    printf("Total time: %lf\n",time1 + time2+time3);

  MPI_Finalize();

  return 0;
}
