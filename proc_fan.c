#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include "makeargv.h"

int main (int argc, char* argv[]) {
   pid_t childpid, wpid = 0;
   int i, c, pr_limit, pr_count, status = 0;
   const int MAX_CANON = 100;
   char delim[] = " \t";
   char arg[MAX_CANON];
   char** myargv;

   if (argc == 2) {
      if ((c = getopt(argc, argv, "h")) != -1) {
         switch(c) {
	 case 'h':
	     printf("Usage: %s -n [INTEGER]\n", argv[0]);
	 }
      }
   return 0; 
   }

   else if (argc == 3) {
      while((c = getopt(argc, argv, "n:")) != -1) {
         switch(c) {
	 case 'n':
	     pr_limit = atoi(argv[2]);
	     pr_count = 0;
	     status = 0;

	     while ((fgets(arg, MAX_CANON, stdin)) != NULL){
	        if (pr_count == pr_limit) {
		   printf("Process limt reached. Waiting...\n");
		   childpid = wait(&status);
		   pr_count--;
		   printf("Process %ld finished. %d child processes running.\n", (long)childpid, pr_count);
		}

		childpid = fork();
		printf("%ld running.\n", (long)getpid());
		i++;

		if (childpid == -1) {
		   perror("fork error");
		} 
		
		else if (childpid == 0) {
		   printf("Hello from child %d, process %ld. Line: %s\n", i, (long)getpid(), arg);
		   if (makeargv(arg, delim, &myargv) == -1) {
		      perror("Child failed to construct argument array");
		   } 
		   else {
		      execvp(myargv[0], &myargv[0]);
		      perror("Child failed to execute command");
		   }
		exit(0);
		} 
		
		else {
		   pr_count++;
		   printf("Started process %d, %ld. %d child processes running. Line: %s\n", i, (long)childpid, pr_count, arg);
	           while ((wpid = waitpid(-1, &status, WNOHANG )) > 0) {
		      pr_count--;
		      printf("Process %ld finished(1). %d child processes running.\n", (long)wpid, pr_count);
		   }
	        }
	     }
	     while ((wpid = wait(&status)) > 0) {
		pr_count--;
	        printf("Process %ld finished(2).\n", (long)wpid);
	     }
	 }
      }
   return 0;
   }
   fprintf(stderr, "%s: Error: Invalid number of arguments\n", argv[0]);
   return 0;
}
