/* 
 * Created by: Nick Nesbit
 * Instructor: Hauschild
 * Date: 2/8/2018
 *
 * Info: Simple batch processing utility
 *          -Creates child processes that execute commands from standard input
 *          -n option required: used to specify number of processes to run simultaneously
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include "makeargv.h"

//Start of main program
int main (int argc, char* argv[]) {
   
   //Declare variables.
   pid_t childpid, donepid = 0;
   int i, c, pr_limit, pr_count, redo, status = 0;
   const int MAX_CANON = 100;
   char delim[] = " \t";
   char arg[MAX_CANON];
   char error[200];
   char** myargv;

   //If number of arguments is 2:
   if (argc == 2) {
      //Check for -h option.
      while ((c = getopt(argc, argv, "h")) != -1) {
         switch(c) {
         case 'h':
            //Check for redundancy.
            if (redo != 1) {
               //Print helpful information.
	       fprintf(stdout, "\nUsage: %s -n [INTEGER] < [EXECUTABLE(S)]\nExample: %s -n 4 < testing.data\n"
	                     "n is a required option to specify the number of process allowed to run at a time.\n"
		             "Executables are read by line from standard input.\n",
		             argv[0], argv[0]);
	       //Set redundancy.
	       redo = 1;
            }
	    else {
	       fprintf(stderr, "%s: Error: Redundancy detected: %c\n", argv[0], c);
	    }
	    break;
         }
      }
      //End program.
      return 0; 
   }

   //Else if number of arguments is 3:
   else if (argc == 3) {
      //Check for -n option.
      while ((c = getopt(argc, argv, "n:")) != -1) {
         switch(c) {
	 case 'n':
	    //Initialize varibales.
	    pr_limit = atoi(argv[2]);
	    pr_count = 0;
	    status = 0;

	    //While reading from standard input (main loop):
	    while ((fgets(arg, MAX_CANON, stdin)) != NULL) {
	       //If no more processes allowed to start wait for one to finish.
	       if (pr_count == pr_limit) {
	          fprintf(stdout, "%s: Process limit reached. Waiting...\n", argv[0]);
	          donepid = wait(&status);
	          pr_count--;
	          fprintf(stdout, "%s: Process %ld finished. %d child processes running.\n", argv[0], (long)donepid, pr_count);
               }

	       //Call the fork funtion, increment i.
	       childpid = fork();
	       i++;

	       //Check for error:
	       if (childpid == -1) {
                  snprintf(error, sizeof(error), "%s: Error", argv[0]);
	          perror(error);
	       } 
		
	       //Child code:
	       else if (childpid == 0) {
	          fprintf(stdout, "%s: Hello from child %d: %ld. Attempting to execute line: %s\n", argv[0], i, (long)getpid(), arg);
		  //Attempt to tokenize input line.
	          if (makeargv(arg, delim, &myargv) == -1) {
		     snprintf(error, sizeof(error), "%s: Error: %ld", argv[0], (long)getpid());
                     perror(error);
		  } 
		  //Attempt to tun the executable.
		  else if (execvp(myargv[0], &myargv[0]) == -1) {
		     snprintf(error, sizeof(error), "%s: Error: %ld", argv[0], (long)getpid());
                     perror(error);
                  }
                  //End child process.
                  exit(0);
               } 
		
	       //Parent code:
	       else {
	          //Increment number of children.
		  pr_count++;
		  fprintf(stdout, "%s: %ld started child process %d: %ld. %d child processes running.\n", 
                                argv[0], (long)getpid(), i, (long)childpid, pr_count);

	          //Check for finished children.
	          while ((donepid = waitpid(-1, &status, WNOHANG )) > 0) {
		     //Decrement number of children.
		     pr_count--;
		     fprintf(stdout, "%s: Process %ld finished. %d child processes running.\n", argv[0], (long)donepid, pr_count);
	          }
               }
            }      
            //End of main loop.
      
      	    //Wait for all children to finish.
      	    while ((donepid = wait(&status)) > 0) {
      	       pr_count--;
      	       fprintf(stdout, "%s: Process %ld finished.\n", argv[0], (long)donepid);
      	    }
            break;
         }
      }
      //Success.
      return 1;
   }

   //Argument error.
   fprintf(stderr, "%s: Error: Invalid number of arguments\n", argv[0]);
   return -1;
}
