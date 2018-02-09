#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char* argv[]) {
   int i, s, r = 0;
   pid_t pid = getpid();

   if (argc != 3) {
      fprintf(stderr, "%s: Error: %ld: Invalid number of arguments\n", argv[0], (long)pid);
      return(0);
   }

   s = atoi(argv[1]);
   r = atoi(argv[2]);

   for (i = 0; i < r; i++) {
      sleep(s);
      fprintf(stdout, "%s: Process ID: %ld\n", argv[0], (long)pid);
   } 
   
   printf("\n");
   return 0;
}
