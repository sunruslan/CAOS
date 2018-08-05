#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include <string.h>

int main (int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage:\n\t./a.out file.txt num\n");
    exit(1);
  }
  FILE *fin = fopen(argv[1], "r");
  if (fin == NULL) {
    perror("fopen");
    exit(1);
  } 
  int c;
  int len = 0;
  int capacity = 10;
  int num_files = 0;
  int cap = 10;
  char **file_names = (char **)malloc(capacity * sizeof(char*));
  if (file_names == NULL) {
    perror("malloc");
    exit(1);
  }
  
  for (int i = 0; i < capacity; ++i) {
    file_names[i] = (char *)malloc(cap * sizeof(char));
    if (file_names[i] == NULL) {
      perror("malloc");
      exit(1);
    }
  }
  
  while ((c = fgetc(fin)) != EOF) {
    if (!isspace(c)) {
      file_names[num_files][len] = c;
      len += 1;
      if (len > cap - 3) {
        cap = cap * 3 / 2;
        file_names[num_files] = (char*) realloc(file_names[num_files], cap * sizeof(char));
        if (file_names[num_files] == NULL) {
          perror("realloc");
          exit(1);
        } 
      }
    } else {
      file_names[num_files][len] = '\0';
      num_files += 1;
      len = 0;
      cap = 10;
      if (num_files > capacity - 3) {
        int old_capacity = capacity;
        capacity = capacity * 3 / 2;
        file_names = (char **) realloc(file_names, capacity * sizeof(char *));
        if (file_names == NULL) {
          perror("realloc");
          exit(1);
        }
        for (int i = old_capacity; i < capacity; ++i) {
          file_names[i] = (char*) malloc(cap * sizeof(char));
        }
      }
    }
  } 
  if (len > 0) {
    file_names[num_files][len] = '\0';
    num_files += 1;
  }
  int num = atoi(argv[2]);
  int *fd = (int *)malloc(num_files * sizeof(int));
  for (int i = 0; i < num_files; ++i) {
    if (i >= num) {
      wait(NULL);
    }
    pid_t pid;
    char template[] = "/tmp/fileXXXXXX";
    fd[i] = mkstemp(template);
    switch (pid = fork()) {
      case -1: 
        perror("fork");
        exit(1);
      case 0:
        dup2(fd[i], 1);
        close(fd[i]);
        execlp("/usr/bin/wc", "wc", file_names[i], NULL);
    }
    unlink(template);
  }
  for (int i = 0; i < num; ++i) {
    wait(NULL);
  }
  int total_lines = 0;
  int total_words = 0;
  int total_symbols = 0;
  for (int i = 0; i < num_files; ++i) {
    lseek(fd[i], 0, SEEK_SET);
    FILE* f = fdopen(fd[i], "r");
    int l, w, s;
    fscanf(f, "%d", &l);
    fscanf(f, "%d", &w);
    fscanf(f, "%d", &s);
    total_lines += l;
    total_words += w;
    total_symbols += s;
    fclose(f);
  }
  printf(" %d  %d %d\n", total_lines, total_words, total_symbols);
  for (int i = 0; i < capacity; ++i) {
    free(file_names[i]);
  }
  free(file_names);
  fclose(fin);
  return 0;
}
