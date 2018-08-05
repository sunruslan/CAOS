#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

int tail (FILE *input) {
  size_t count = 0;

  char **lines = malloc(sizeof(char*)*10);
  if (lines == NULL){
    fprintf(stdout, "Allocation error.\n"); 
    return -1;
  }
  for (int i = 0; i < 10; ++i) {
    lines[i] = NULL;
  }
 
  int size = 1;
  char *line = NULL;
  size_t line_index = 0;
  while (getline(&line, &size, input) != -1) {
    count += 1;
    free(lines[line_index]);
    lines[line_index] = (char*)malloc(size * sizeof(char));
    if (lines == NULL) {
      fprintf(stdout, "Allocation error.\n");
      return -1;
    }
    strcpy(lines[line_index], line);
    line_index = (line_index + 1) % 10;
    free(line);
    line = NULL;
  }
  
  if (count > 10){
    line_index = (line_index + 9) % 10;
    for (size_t j = 0; j < 10; ++j) {
      line_index = (line_index + 1) % 10;
      fprintf(stdout, "%s", lines[line_index]);
    }
  }
  else {
    for (size_t j = 0; j < count; ++j) {
      fprintf(stdout, "%s", lines[j]);
    } 
  }

  for (size_t i = 0; i < 10; ++i) {
    free(lines[i]);
  }
  free(lines);
  return 1;
}

int main (int argc, char ** argv) {
  if (argc == 1) {
    tail(stdin);
  } 
  else {
    for (size_t i = 1; i < argc; ++i) {
      if (argc > 2) {
        fprintf(stdout, "\n==> %s <==\n", argv[i]);
      }
      FILE *fin = fopen(argv[i], "r");
      if (fin != NULL) {
        tail(fin);
        fclose(fin);
      }
      else {
        fprintf(stdin, "ERROR! File \"%s\" does not exist or access is denied", argv[i]);
      }
    }
  }
}
