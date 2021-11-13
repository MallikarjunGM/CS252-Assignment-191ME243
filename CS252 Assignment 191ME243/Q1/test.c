#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include<fcntl.h>
//git
int main (int argc, char **argv)
{
  struct timeval start;
  struct timeval end;
  if( argc <= 1 )
{
   fprintf( stderr, "usage:   %s [args...]\n", argv[0] );

}
  /*{/*char sharedMemoryName[] = "Command Timer";
  int   sharedMemoryFD   = shm_open( sharedMemoryName, IPC_CREAT | O_RDWR, 0666 );
  ftruncate( sharedMemoryFD, sizeof(start) );
  struct timeval *sharedMemory = mmap( 0, sizeof(start), PROT_READ | PROT_WRITE, MAP_SHARED, sharedMemoryFD, 0 );

  pid_t pid = fork();

  if(pid==0)
  {
  gettimeofday(sharedMemory, 0 );
  execvp( argv[1], argv + 1 );
  }
  wait(NULL);
  start = *sharedMemory;
  gettimeofday(&end,NULL);
  float t;
  t = ((end.tv_sec * 1000 + end.tv_usec*0.001) -
  (start.tv_sec * 1000 + start.tv_usec*0.001));
  printf("\n %f Milliseconds\n",t);}*/


  typedef struct timeval timeval_t;
  // Shared memory setup
   // 1) create the shared memory region
   char sharedMemoryName[] = "Command Timer";
   int sharedMemoryFD   = shm_open( sharedMemoryName, O_CREAT | O_RDWR, 0666 );
   // 2) configure the size of the shared memory region
   ftruncate( sharedMemoryFD, sizeof(timeval_t) );
   // 3) memory map the shared memory region
   timeval_t * sharedMemory = mmap( 0, sizeof(timeval_t), PROT_READ | PROT_WRITE, MAP_SHARED, sharedMemoryFD, 0 );

   if( fork() == 0 )
{

   // Get and save the current time into shared memory,
   gettimeofday( sharedMemory, 0 );
      execvp( argv[1], argv + 1 );
    // parent process
   wait( 0 );
   // get the end time
   timeval_t end_time;
   gettimeofday( &end_time, 0 );
   // get the start time
   timeval_t   startTime;
   startTime = *sharedMemory;
   // The child placed the start time in shared memory
   shm_unlink(sharedMemoryName);

// calculate elapsed time
   timeval_t elapsed_time;
   timersub( &end_time, &startTime, &elapsed_time );

   // print microseconds right justified zero filled
   printf( "\nElapsed time: %d.%06d seconds\n", elapsed_time.tv_sec, elapsed_time.tv_usec );
  // printf( "IPC Method: %s\nCommand: ", IPC_METHOD);
   char ** arg = argv + 1;
   //while( *arg ) printf("%s ", *arg++);
   //printf("\n");
 }
}
