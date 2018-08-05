#include <stdio.h>
#include <stdlib.h>

int main (int argc, char **argv) {
  int **pfd = malloc((argc-1)*sizeof(int*));
  if (pfd == NULL) {
    perror("malloc");
    exit(1); 
  }
  for (int i = 0; i < argc - 1; ++i) {
    pfd[i] = malloc(2*sizeof(int));
    if (pfd[i] == NULL) {
      perror("malloc");
      exit(1);
    }
  }
  int f;
  int in = 0; 
  for (int i = 0; i < argc - 2; ++i) {
    pipe(pfd[i]);
  }
  for (int i = 1; i < argc; i++) {
    switch (f = fork()) {
      case -1:
        perror("fork");
        exit(1);
        break;
      case 0:
        if (i == 1) {
          dup2(pfd[i-1][1], 1);
        } else {
          dup2(pfd[i-1][1], 1);
          dup2(pfd[i-2][0], 0);
        }
        for (int j = 0; j < argc - 2; ++j) {
          close(pfd[j][0]);
          close(pfd[j][1]);
        }
        if (execlp(argv[i], argv[i], NULL) == -1) {
          perror("execlp");
          exit(1);
        }
    }
  }
  for (int i = 0; i < argc - 2; ++i) {
    close(pfd[i][0]);
    close(pfd[i][1]);
  }
  for (int i = 1; i < argc; ++i) {
    wait(NULL);
  }
  return 0;
}
