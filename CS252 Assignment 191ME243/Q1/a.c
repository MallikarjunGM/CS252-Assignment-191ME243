/*Mallikarjun G M 191ME243
CS252 Assignment
Question number 3.19
git?
A program is to created that takes a command as an argument from
the terminal and display the time it takes to Run the said command.
2 versions of this program should be written, with different method of
IPC.
a) Shared-memory
b) Pipes

This program is the shared memory version.

Part of this code was taken from a solution posted on chegg.com by an
unknown author.

Explainations for functions used for shared memory was taken from
OS Concepts by Silberschatz et.al
*/

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

int main (int argc, char **argv)
{
  //timeval is a structure include in time.h
  //It holds timevalue in seconds and microseconds
  struct timeval start;
  struct timeval end;

  {
    //the following lines create a shared-memory

    //'command timer' will be the name of the shared memory object
    char sharedMemoryName[] = "Command Timer";

    /*in shm_open(), the first parameter is the name of shared shared memory
    CREAT AND RDWR are used to respective create and open read and write
    permission
    last parameter specifies file access permisiion.
    it returns file descriptor as integer
    */
    int   sharedMemoryFD   = shm_open ( sharedMemoryName, IPC_CREAT | O_RDWR, 0666 );

    /*ftruncate is used to configure size of the object
    the mmap() function establishes a memory-mapped file containing
    the shared-memory object.It also returns a pointer to the memory-mapped file
    that is used for accessing the shared-memory object
    */
    ftruncate ( sharedMemoryFD, sizeof(start) );
    struct timeval *sharedMemory = mmap ( 0, sizeof(start), PROT_READ | PROT_WRITE, MAP_SHARED, sharedMemoryFD, 0 );

    //A child process is created and the process ID is stored in pid.
    pid_t pid = fork ();

    //The next block of code only runs when it is in child process.
    if (pid == 0)
      {
        /*gettimeofday() writes the instant of time when it is run to a
          timeval_t structure. In this case, since the child process must
          communicate to the parent the start time, it will write to
          the shared memory. The time is from UNIX Epoch time*/
        gettimeofday (sharedMemory, NULL );

        /*execvp function replaces the current process with a new processes
        specified by the arrgument. Used to construct a pathname that identifies
        the new process image file. In this case, the process argument passed
        will be the command which is argv from argument of main()*/
        execvp ( argv[1], argv + 1 );
      }
    /*wait() waits for the child process to be terminated before continuing*/
    wait (NULL);
    /*time at which child process ends is recorded and stored in 'end'*/
    gettimeofday (&end,NULL);
    /*contents of sharedMemory is moved to start*/
    start = *sharedMemory;
    float time_elapsed;

    /*following lines calculates difference in times between start and end
    and prints it in ____ Milliseconds*/
    time_elapsed = ((end.tv_sec * 1000 + end.tv_usec * 0.001) -
                    (start.tv_sec * 1000 + start.tv_usec * 0.001));
    printf ("\n %f Milliseconds\n",time_elapsed);
  }
}
