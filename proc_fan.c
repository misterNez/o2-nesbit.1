#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include "makeargv.h"

//Start of main program
int main (int argc, char* argv[]) {
   
   //Declare varibales.
   pid_t childpid, donepid = 0;
   int i, c, pr_limit, pr_count, redo, status = 0;
   const int MAX_CANON = 100;
   char delim[] = " \t";
   char arg[MAX_CANON];
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
	       printf("\nUsage: %s -n [INTEGER] < [EXECUTABLE(S)]\nExample: %s -n 4 < testing.data\n"
	                "n is a required option to specify the number of process allowed to run at a time.\n",
		        "Executables are read by line from standard input.\n",
		        argv[0], argv[0]);
	       //Set redundancy.
	       redo = 1;
            }
	    else {
	       fprintf(stderr, "%s: Error: Redundancy detected: %c", argv[0], c);
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
      if ((c = getopt(argc, argv, "n:")) != -1) {
         switch(c) {
	    case 'n':
	    //Initialize varibales.
	    pr_limit = atoi(argv[2]);
	    pr_count = 0;
	    status = 0;

	    //While reading from standard input:
	    while ((fgets(arg, MAX_CANON, stdin)) != NULL) {
	       //If no more processes allowed to start wait for one to finish.
	       if (pr_count == pr_limit) {
	          printf("Process limit reached. Waiting...\n");
	          donepid = wait(&status);
	          pr_count--;
	          printf("Process %ld finished. %d child processes running.\n", (long)donepid, pr_count);
               }

	       //Call the fork funtion, print current PID, increment i.
	       childpid = fork();
	       printf("%ld running.\n", (long)getpid());
	       i++;

	       //Check for error:
	       if (childpid == -1) {
		  fprintf(stderr, "%s: Error: ", argv[0]);
	          perror("Fork error");
	       } 
		
	       //Child code:
	       else if (childpid == 0) {
	          printf("Hello from child %d: %ld. Line: %s\n", i, (long)getpid(), arg);
		  //Attempt to tokenize input line.
	          if (makeargv(arg, delim, &myargv) == -1) {
		     fprintf(stderr, "%s: Error: %ld: ", argv[0], (long)getpid());
		     perror("Child failed to construct argument array");
		  } 
		  //Attempt to tun the executable.
		  else if (execvp(myargv[0], &myargv[0]) == -1) {
		     fprintf(stderr, "%s: Error: %ld: ", argv[0], (long)getpid());
		     perror("Child failed to execute command");
                  }
                  exit(0);
               } 
		
	       //Parent code:
	       else {
	          //Increment number of children.
		  pr_count++;
		  printf("%ld started child process %d: %ld.\n%d child processes running.\n", (long)getpid(), i, (long)childpid, pr_count);
		  //Check for finished children:
	          while ((donepid = waitpid(-1, &status, WNOHANG )) > 0) {
		     //Decrement number of children.
		     pr_count--;
		     printf("Process %ld finished. %d child processes running.\n", (long)donepid, pr_count);
	          }
	       }
            }
	    //Wait for all children to finish.
	    while ((donepid = wait(&status)) > 0) {
	       pr_count--;
	       printf("Process %ld finished.\n", (long)donepid);
	    }
            break;
         }
      }
      //End of program.
      return 1;
   }
   //Report command line error.
   fprintf(stderr, "%s: Error: Invalid number of arguments", argv[0]);
   return -1;
}
