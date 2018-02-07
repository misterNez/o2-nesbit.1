#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char* argv[]) {
   int i, s, r = 0;;
   pid_t pid = getpid();

   if (argc != 3) {
      fprintf(stderr, "Usage: %s processes\n", argv[0]);
      return 1;
   }

   s = atoi(argv[1]);
   r = atoi(argv[2]);

   for (i = 0; i < r; i++) {
      sleep(s);
      fprintf(stderr, "Process ID: %ld\n", pid);
   }
   
   printf("\n");
   return 0;
}
