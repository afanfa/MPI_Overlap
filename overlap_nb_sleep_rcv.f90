program nb_overlap
  use mpi
  use iso_fortran_env, only : real64
  implicit none

  integer :: i,me,np,n,tmp
  integer :: req,ierr
  integer :: status(MPI_STATUS_SIZE)
  real(real64), allocatable :: x(:),y(:)
  real(real64) :: start_time, end_time

  n = 100000

  allocate(x(n),y(n))

  call MPI_Init(ierr)

  call MPI_COMM_RANK(MPI_COMM_WORLD,me,ierr)
  call MPI_COMM_SIZE(MPI_COMM_WORLD,np,ierr)

! Send/Recv to start communication
  if(me == 0) call MPI_Send(me,1,MPI_INT,np-1,0,MPI_COMM_WORLD,ierr)
  if(me == np-1) call MPI_Recv(tmp,1,MPI_INT,0,0,MPI_COMM_WORLD,status,ierr)

  do i=1,n
     x(i) = real(i,kind=real64)
  enddo

  y = 0.0d0

  if(me == np-1) then
     call MPI_Irecv(y, n, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, req, ierr)
  endif

  call MPI_Barrier(MPI_COMM_WORLD,ierr)

  if(me == 0) then
     start_time = MPI_Wtime()
     call MPI_Isend(x, n, MPI_DOUBLE, np-1, 0 ,MPI_COMM_WORLD, req, ierr)
     call sleep(2)
     call MPI_Wait(req,status,ierr)
     end_time = MPI_Wtime()
     write(*,*) "Process",me,"spent",end_time-start_time,"seconds processing and sending data (2 seconds for computation)"
  else if(me == np-1) then
     start_time = MPI_Wtime()
     call sleep(1)
     call MPI_Wait(req,status,ierr)
     end_time = MPI_Wtime()
     write(*,*) "Process",me,"spent",end_time-start_time,"seconds waiting (1 sec for computation)"
     write(*,*) "Value received y(10)=",y(10),". Expected",10.0
  endif

  call MPI_Barrier(MPI_COMM_WORLD,ierr)

  y = 0.0d0

  if(me == np-1) then
     call MPI_Irecv(y, n, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, req, ierr)
  endif

  if(me == 0) write(*,*) 'Second transfer'

  call MPI_Barrier(MPI_COMM_WORLD,ierr)

  if(me == 0) then
     start_time = MPI_Wtime()
     call MPI_Isend(x, n, MPI_DOUBLE, np-1, 0 ,MPI_COMM_WORLD, req, ierr)
     call sleep(2)
     call MPI_Wait(req,status,ierr)
     end_time = MPI_Wtime()
     write(*,*) "Process",me,"spent",end_time-start_time,"seconds processing and sending data (2 seconds for computation)"
  else if(me == np-1) then
     start_time = MPI_Wtime()
     call sleep(1)
     call MPI_Wait(req,status,ierr)
     end_time = MPI_Wtime()
     write(*,*) "Process",me,"spent",end_time-start_time,"seconds waiting (1 sec for computation)"
     write(*,*) "Value received y(10)=",y(10),". Expected",10.0
  endif

  call MPI_Barrier(MPI_COMM_WORLD,ierr)

  call MPI_Finalize(ierr)

end program nb_overlap
