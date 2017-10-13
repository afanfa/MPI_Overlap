#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>

int main(void)
{
  int i,me,np,n,provided,j,*blk_len,*disp,name_len,tmp;
  double *x,*y,start_time,end_time;
  MPI_Request req;
  MPI_Status status;
  MPI_Datatype dt;
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

  blk_len = (int *)calloc(100,sizeof(int));
  disp = (int *)calloc(100,sizeof(int));

  for(i=0;i<100;i++)
    {
      blk_len[i] = 1;
      disp[i] = i+(9*i);
    }

  MPI_Type_indexed(100, blk_len, disp, MPI_DOUBLE, &dt);
  MPI_Type_commit(&dt);

  if(me == np-1)
    MPI_Irecv(y, 1, dt, 0, 0, MPI_COMM_WORLD, &req);
  
  MPI_Barrier(MPI_COMM_WORLD);

  if(me == 0)
    {
      start_time = MPI_Wtime();
      /* Sending x to last process */
      MPI_Isend(x, 1, dt, np-1, 0, MPI_COMM_WORLD, &req);
      //Simulate computation for 2 seconds
      usleep(2000000);
      MPI_Wait(&req, &status);
      end_time = MPI_Wtime();
      printf("Process %d spent %lf seconds processing and sending data (2 seconds for computation)\n",me,end_time-start_time);
    }
  else if(me == np-1)
    {
      start_time = MPI_Wtime();
      usleep(1000000);
      MPI_Wait(&req,&status);
      end_time = MPI_Wtime();
      printf("Process %d spent %lf seconds waiting (1 sec computing)\n",me,end_time-start_time);
      printf("Value received y[10]=%lf. Expected %lf\n",y[10],10.0);
    }

  MPI_Barrier(MPI_COMM_WORLD);

  for(i=0;i<n;i++)
    y[i] = 0.0;

  if(me == np-1)
    MPI_Irecv(y, 1, dt, 0, 0, MPI_COMM_WORLD, &req);

  if(me == 0)
    printf("Second transfer\n");

  MPI_Barrier(MPI_COMM_WORLD);

  if(me == 0)
    {
      start_time = MPI_Wtime();
      /* Sending x to last process */
      MPI_Isend(x, 1, dt, np-1, 0, MPI_COMM_WORLD, &req);
      //Simulate computation for 2 seconds
      usleep(2000000);
      MPI_Wait(&req, &status);
      end_time = MPI_Wtime();
      printf("Process %d spent %lf seconds processing and sending data (2 seconds for computation)\n",me,end_time-start_time);
    }
  else if(me == np-1)
    {
      start_time = MPI_Wtime();
      usleep(1000000);
      MPI_Wait(&req,&status);
      end_time = MPI_Wtime();
      printf("Process %d spent %lf seconds waiting (1 sec computing)\n",me,end_time-start_time);
      printf("Value received y[10]=%lf. Expected %lf\n",y[10],10.0);
    }
  
  MPI_Barrier(MPI_COMM_WORLD);

  MPI_Finalize();

  return 0;
}
