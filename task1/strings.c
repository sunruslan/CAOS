#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

static int myCompare (const void * a, const void * b) {
  return strcmp (*(const char **) a, *(const char **) b);
}

int main ( int argc, char *argv[] ) {
  if ( argc !=  3 ) {
    fprintf(stderr, "syntax : \n\t./strings input_file.txt output_file.txt\n");
    exit(1);
  }
  FILE* fin = fopen(argv[1], "r");
  if ( fin == NULL ) {
    fprintf(stderr, "\tFile %s is not readable or does not exist\n", argv[1]);
    exit(1);
  }
  else {
    int i=0, j=0;
    int word_capacity=10, capacity=10;
    char **words=malloc(capacity*sizeof(char*));    
    if (words == NULL){
      perror("malloc");
      exit(1);
    }
    int c, pred;
    words[j]=malloc(word_capacity*sizeof(char)); 
    if (words[j] == NULL){
      perror("malloc");
      exit(1);
    }
    while ((c=fgetc(fin)) != EOF) {
      if (!isspace(c)) {
        if (c != ';' && c != '&' && c != '|') {
          if (pred == '|' || pred == '&') {
            if (i > 0) {
              words[j][i] = '\0';
              ++j;
              words[j] = malloc(2*sizeof(char));
              if (words[j] == NULL) {
                perror("malloc");
                exit(1);
              }
            }
            char *tmp = malloc(2*sizeof(char));
            if (tmp == NULL) {
              perror("malloc");
              exit(1);
            }
            tmp[0] = pred;
            tmp[1] = '\0';
            strcpy(words[j], tmp);
            ++j;
            i=0;
            word_capacity = 10;
            words[j] = malloc(word_capacity*sizeof(char));
            if (words[j] == NULL) {
              perror("malloc");
              exit(1);
            }    
          }
          words[j][i] = c;
          pred = c; 
          ++i;
        }
        if (c == ';') {
          if (i > 0) {
            words[j][i] = '\0';
            ++j;
            words[j] = malloc(2*sizeof(char));
            if (words[j] == NULL) {
              perror("malloc");
              exit(1);
            }
          } 
          strcpy(words[j], ";");
          ++j;
          i=0;
          word_capacity = 10;
          words[j] = malloc(word_capacity*sizeof(char));
          if (words[j] == NULL) {
            perror("malloc");
            exit(1);
          }
          pred = ';';
        }
        if (c == '|') {
          int new_pred = '|';
          if (pred == '|') {
            if (i > 0) {
              words[j][i] = '\0';
              ++j;
              words[j] = malloc(3*sizeof(char));
              if (words[j] == NULL) {
                perror("malloc");
                exit(1);
              }
            } 
            strcpy(words[j], "||");
            ++j;
            i=0;
            word_capacity = 10;
            words[j] = malloc(word_capacity*sizeof(char));
            if (words[j] == NULL) {
              perror("malloc");
              exit(1);
            }
            new_pred = -1;
          } else if (pred == '&') {
            if (i > 0) {
              words[j][i] = '\0';
              ++j;
              words[j] = malloc(2*sizeof(char));
              if (words[j] == NULL) {
                perror("malloc");
                exit(1);
              }
            }
            char *tmp = malloc(2*sizeof(char));
            if (tmp == NULL) {
              perror("malloc");
              exit(1);
            }
            tmp[0] = pred;
            tmp[1] = '\0';
            strcpy(words[j], tmp);
            ++j;
            i=0;
            word_capacity = 10;
            words[j] = malloc(word_capacity*sizeof(char));
            if (words[j] == NULL) {
              perror("malloc");
              exit(1);
            } 
          }
          pred = new_pred;
        }
        if (c == '&') {
          int new_pred = '&';
          if (pred == '&') {
            if (i > 0) {
              words[j][i] = '\0';
              ++j;
              words[j] = malloc(3*sizeof(char));
              if (words[j] == NULL) {
                perror("malloc");
                exit(1);
              }
            } 
            strcpy(words[j], "&&");
            ++j;
            i=0;
            word_capacity = 10;
            words[j] = malloc(word_capacity*sizeof(char));
            if (words[j] == NULL) {
              perror("malloc");
              exit(1);
            }
            new_pred = -1;
          } else if (pred == '|') {
            if (i > 0) {
              words[j][i] = '\0';
              ++j;
              words[j] = malloc(2*sizeof(char));
              if (words[j] == NULL) {
                perror("malloc");
                exit(1);
              }
            }
            char *tmp = malloc(2*sizeof(char));
            if (tmp == NULL) {
              perror("malloc");
              exit(1);
            }
            tmp[0] = pred;
            tmp[1] = '\0';
            strcpy(words[j], tmp);
            ++j;
            i=0;
            word_capacity = 10;
            words[j] = malloc(word_capacity*sizeof(char));
            if (words[j] == NULL) {
              perror("malloc");
              exit(1);
            } 
          }
          pred = new_pred;
        }
      }
      else{
        if (pred == '|' || pred == '&') {
          if (i > 0) {
            words[j][i] = '\0';
            ++j;
            words[j] = malloc(2*sizeof(char));
            if (words[j] == NULL) {
              perror("malloc");
              exit(1);
            }
          }
          char *tmp = malloc(2*sizeof(char));
          if (tmp == NULL) {
            perror("malloc");
            exit(1);
          }
          tmp[0] = pred;
          tmp[1] = '\0';
          strcpy(words[j], tmp);
          ++j;
          i=0;
          word_capacity = 10;
          words[j] = malloc(word_capacity*sizeof(char));
          if (words[j] == NULL) {
            perror("malloc");
            exit(1);
          }    
        }
        if (i > 0){
          words[j][i]='\0';
          ++j;
          i=0;
          word_capacity = 10;
          words[j] = malloc(word_capacity*sizeof(char));
          if (words[j] == NULL) {
            perror("malloc");
            exit(1);
          }
        }
      }
      if (i > word_capacity - 4) {
        word_capacity = word_capacity * 3 / 2;
        words[j] = realloc(words[j], word_capacity*sizeof(char));
        if (words[j] == NULL) {
          perror("malloc");
          exit(1);
        }
      }
      if (j > capacity - 4) {
        capacity = capacity * 3 / 2;
        words = realloc(words, capacity*sizeof(char*));
        if (words == NULL) {
          perror("malloc");
          exit(1);
        }
      }
    }
    if (!strcmp(words[j], "")){
      --j;
    }
    fclose(fin);
    
    qsort(words, j+1, sizeof(const char *), myCompare);
    
    FILE *fout = fopen(argv[2], "w");
    if (fout == NULL) {
      perror("fopen");
      exit(1);
    }
    for (size_t k = 0; k < j+1; ++k){
      fprintf(fout, "\"%s\"\n", words[k]);
    }
    fclose(fout);
    
    for (size_t k = 0; k < j+2; ++k){
      free(words[k]);
    } 
    free(words);
  }
  return 0;
}
