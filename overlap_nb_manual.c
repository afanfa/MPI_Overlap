#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>

int main(void)
{
  int i,me,np,n,provided,flag,name_len,tmp;
  double *x,*y,start_time,end_time,elapsed;
  MPI_Request req;
  MPI_Status status;
  char processor_name[MPI_MAX_PROCESSOR_NAME];

  MPI_Init(NULL,NULL);

  n = 100000;

  MPI_Comm_rank(MPI_COMM_WORLD,&me);
  MPI_Comm_size(MPI_COMM_WORLD,&np);

  MPI_Get_processor_name(processor_name, &name_len);

  if(me == 0 || me == np-1)
    printf("Process %d has name %s\n",me,processor_name);

  // Send/Recv to start communication
  if(me == 0)
    MPI_Send(&me,1,MPI_INT,np-1,0,MPI_COMM_WORLD);

  if(me == np-1)
    MPI_Recv(&tmp,1,MPI_INT,0,0,MPI_COMM_WORLD,&status);
  
  x = (double *)malloc(n*sizeof(double));
  y = (double *)malloc(n*sizeof(double));

  for(i=0;i<n;i++)
    x[i] = (double) i;

  if(me == np-1)
    MPI_Irecv(y, n, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &req);
  
  MPI_Barrier(MPI_COMM_WORLD);

  if(me == 0)
    {
      start_time = MPI_Wtime();
      /* Sending x to last process */
      MPI_Isend(x, n, MPI_DOUBLE, np-1, 0, MPI_COMM_WORLD, &req);
      //Simulate computation for 2 seconds
      usleep(1000000);
      flag = 0;
      while(flag == 0)
	MPI_Test(&req, &flag, &status);
      usleep(1000000);
      MPI_Wait(&req, &status);
      end_time = MPI_Wtime();
      printf("Process %d spent %lf seconds processing and sending data (2 seconds for computation)\n",me,end_time-start_time);
    }
  else if(me == np-1)
    {
      start_time = MPI_Wtime();
      MPI_Wait(&req,&status);
      end_time = MPI_Wtime();
      elapsed = end_time - start_time;
      printf("Process %d spent %lf seconds waiting\n",me,end_time-start_time);
      printf("Value received y[10]=%lf. Expected %lf\n",y[10],10.0);
      MPI_Send(&elapsed,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
    }
  
  if(me == 0)
    {
      MPI_Recv(&elapsed,1,MPI_DOUBLE,np-1,0,MPI_COMM_WORLD,&status);
      if(abs((end_time-start_time)-elapsed) < 0.5)
	printf("NO OVERLAPPING\n");
      else
	printf("OVERLAPPING ACHIEVED\n");
    }

  MPI_Barrier(MPI_COMM_WORLD);

  MPI_Finalize();

  return 0;
}
