program main

  use mpi
  implicit none

  character(len=20) :: message1,message2,message3
  integer :: myid, ierr, status(mpi_status_size), rc, numprocs

  call MPI_INIT(ierr)
  call MPI_COMM_RANK( MPI_COMM_WORLD, myid, ierr )
  call MPI_COMM_SIZE(MPI_COMM_WORLD,numprocs,ierr)
  write(*,*) '  process ', myid, ' of ', numprocs, ' is alive'

  call MPI_BARRIER( MPI_COMM_WORLD, ierr )

  if ( myid .eq. 0) then
      message1 = 'Hello, process 1'
      call MPI_SEND(message1,20,MPI_CHAR,1,99,MPI_COMM_WORLD,ierr)
      message2 = 'Hello, process 2'
      call MPI_SEND(message2,20,MPI_CHAR,2,99,MPI_COMM_WORLD,ierr)
      message3 = 'Hello, process 3'
      call MPI_SEND(message3,20,MPI_CHAR,3,99,MPI_COMM_WORLD,ierr)
  else if ( myid .eq. 1 ) then
      call MPI_RECV(message1,20,MPI_CHAR,0,99,MPI_COMM_WORLD,status,ierr)
      write(*,*) message1
  else if ( myid .eq. 2 ) then
      call MPI_RECV(message2,20,MPI_CHAR,0,99,MPI_COMM_WORLD,status,ierr)
      write(*,*) message2
  else if ( myid .eq. 3 ) then
      call MPI_RECV(message3,20,MPI_CHAR,0,99,MPI_COMM_WORLD,status,ierr)
      write(*,*) message3
  end if

  call MPI_FINALIZE(rc)

end