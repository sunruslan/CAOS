#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void string(FILE* input) {
  int c;
  int i=0;
  int j=0;
  char *str = malloc(sizeof(char));
  while ((c=fgetc(input))!=EOF) {
    if (isprint(c)) {
      if (i > 3) {
        str = realloc(str, (i+1)*sizeof(char));
        str[i] = '\0';
        printf("%s", str);
        i=0;
      }
      str = realloc(str, (i+1)*sizeof(char));
      str[i] = c;
      ++i;
      ++j;
    } else {
      if (j > 3) {
        str = realloc(str, (i+1)*sizeof(char));
        str[i] = '\0';
        printf("%s\n", str);
      }
      str = realloc(str, sizeof(char));
      j = 0;
      i = 0;
    }
  }
  if (j > 3) {
    str = realloc(str, (i+1)*sizeof(char));
    str[i]='\0';
    printf("%s\n", str);  
  }
}


int main (int argc, char **argv) {
  if (argc == 1) {
    string(stdin);
  }  
  else {
    for (size_t k = 1; k < argc; ++k) {
      FILE *fin = fopen(argv[k], "r");
      if (fin == NULL){
        perror("fopen");
        continue;
      }
      string(fin);
      fclose(fin);
    }
  }
  return 0;
}
