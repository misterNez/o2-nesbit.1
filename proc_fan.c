#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

int main (int argc, char* argv[]) {
   pid_t childpid, wpid = 0;
   int i, c, pr_limit, pr_count, status;

   if (argc == 2) {
      while ((c = getopt(argc, argv, "h")) != -1) {
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

	     for (i = 1; i <= pr_limit; i++ ) {
	        if (pr_count == pr_limit) {
		   wait(&status);
		   pr_count--;
		}

		childpid = fork();

		if (childpid == -1) {
		   perror("fork");
		}

		else if (childpid == 0) {
		   pr_count++;
		   printf("Hello from child %d.", i);
		   sleep(i+1);
		   exit(0);
		}

		else {
		   printf("Started process %d, %ld sleeping\n", i, (long)getpid());
		   while ((wpid = waitpid(-1, &status, WNOHANG)) > 0)
		      pr_count--;
		}
	     }
	     while ((wpid = wait(&status)) > 0);
	 }
      }

   return 0;
   }

   else
      fprintf(stderr, "%s: Error: Invalid number of arguments\n", argv[0]);

   return 0;
}
