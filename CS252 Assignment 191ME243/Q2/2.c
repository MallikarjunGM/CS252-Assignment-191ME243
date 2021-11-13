/*Mallikarjun G M 191ME243
CS252 Assignment
Question number 3.21

A child process must be created to generate Collatz
sequence for a given positive integer
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>


int main ()
{
  pid_t parent_pid = getpid ();
  /*Process ID of the parent is stored in the variable parent_id. This is
  to later verify if the collatz sequence is being generated in child process*/
  long int n;
  long int m;
  /*n is used for storing the integer which is input by user, m is used later
  while printing the sequence*/
  printf ("\nCollatz Conjecture: \n");
  printf ("<Parent Process ID = %i>\n",parent_pid);
  printf ("Enter an Integer greater than 0\n");
  scanf ("%ld",&n);
  while (n <= 0)
    {
      printf ("\n%ld is not valid,\n",n);
      printf ("Enter an Integer GREATER than 0\n");
      scanf ("%ld",&n);
    }
  //The above loop ensures that the integer input is positive
  m = n;

  //Creation and application of child process starts here
  pid_t child_pid;
  child_pid = fork ();
  //child process with ID child_pid is created

  if (child_pid == 0)
  /*to ensure that the collatz sequence is generated only
  by the child process*/
    {
      printf ("<Currently in Child Process>\n");
      printf ("Process ID = %i\n\n",getpid ());
      /*This shows current process ID is NOT of main process*/


      while (1)
      //Generates collatz sequence until number is 1
        {
          if (m == 1)
            {
              //printf("%ld\n",m);
              printf ("<Current Process ID = %i\n",getpid ());
              printf ("<Child process completed>\n");
              break;
            }//Break condition for the loop is when sequence reaches 1

          else if (m % 2 == 0)
            {
              n = m;
              m = m / 2;
              printf ("%ld = %ld/2\n",m,n);
            }
          else
            {
              n = m;
              m = 3 * m + 1;
              printf ("%ld = %ld * 3 +1\n",m,n);
            }//Above 'Else' conditions are from definition of Collatz series
        }
    }

  if (getpid () == parent_pid)
  //This runs only in parent process
    {
      wait (NULL);
      /*wait(NULL) makes the parent wait for all of its child processes
      to terminate*/
      printf ("Back to parent process <Process id = %i>\n",parent_pid);
      printf ("Parent process done\n");
    }
}
