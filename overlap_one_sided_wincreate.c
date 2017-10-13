#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>

int main(void)
{
  int i,me,np,n,provided,tmp,name_len;
  double *x,*y,start_time,end_time,elapsed;
  MPI_Win win;
  MPI_Aint win_size;
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

  y = (double *)malloc(n*sizeof(double));
  x = (double *)malloc(n*sizeof(double));

  win_size = n * sizeof(double);

  MPI_Win_create(x, win_size, 1, MPI_INFO_NULL, MPI_COMM_WORLD, &win);
  //MPI_Win_allocate(win_size, 1, MPI_INFO_NULL, MPI_COMM_WORLD, &x, &win);
  MPI_Barrier(MPI_COMM_WORLD);

  for(i=0;i<n;i++)
    x[i] = (double) i;

  MPI_Barrier(MPI_COMM_WORLD);

  if(me == 0)
    {
      start_time = MPI_Wtime();
      //Simulate computation for 2 seconds
      usleep(2000000);
      end_time = MPI_Wtime();
      printf("Process %d spent %lf seconds computing\n",me,end_time-start_time);
    }
  else if(me == np-1)
    {
      start_time = MPI_Wtime();
      MPI_Win_lock(MPI_LOCK_SHARED, 0, 0, win);
      MPI_Get(y, n, MPI_DOUBLE, 0, 0, n, MPI_DOUBLE, win);
      MPI_Win_unlock(0,win);
      end_time = MPI_Wtime();
      elapsed = end_time - start_time;
      printf("Process %d spent %lf seconds getting the data\n",me,end_time-start_time);
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
