#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

int main (int argc, char* argv[]) {
   pid_t childpid, wpid = 0;
   int i, c, pr_limit, pr_count, status;

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

	     for (i = 1; i <= 5; i++ ) {
	        if (pr_count == pr_limit) {
		   childpid = wait(&status);
		   pr_count--;
		   printf("Process limit reached. Process %ld finished. %d child processes running.\n", (long)childpid, pr_count);
		}

		childpid = fork();
		printf("%ld running.\n", (long)getpid());

		if (childpid == -1) {
		   perror("fork");
		}

		else if (childpid == 0) {
		   //pr_count++;
		   printf("Hello from child %d, process %ld.\n", i, (long)getpid());
		   sleep(i+1);
		   exit(0);
		}

		else {
		   int z = 0;
		   pr_count++;
		   printf("Started process %d, %ld. %d child processes running.\n", i, (long)childpid, pr_count);
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

   else
      fprintf(stderr, "%s: Error: Invalid number of arguments\n", argv[0]);

   return 0;
}
