#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void wc(int *symbols, int *words, int *lines, FILE *input) {
  int c;
  int new_word = 1;
  while ((c = fgetc(input)) != EOF) {
    if (isspace(c)) {
      if (c == '\n') {
        ++(*lines);
      }
      new_word = 1;
    } else if (new_word) {
      ++(*words);
      new_word = 0;
    }
    ++(*symbols); 
  }  
}

int main (int argc, char **argv) {
  int symbols = 0;
  int words = 0;
  int lines = 0;
  if (argc == 1) {
    wc(&symbols, &words, &lines, stdin);
    printf("\t%d\t%d\t%d\n", lines, words, symbols);
  } else {
    if (argc == 2) {
      FILE *fin = fopen(argv[1], "r");
      if (fin == NULL) {
        perror("fopen");
        exit(1);
      }
      wc(&symbols, &words, &lines, fin);
      fclose(fin);
      printf("%d %d %d %s\n", lines, words, symbols, argv[1]);
    } else {
      int total_symbols = 0;
      int total_words = 0;
      int total_lines = 0;
      for (int i = 1; i < argc; ++i) {
        symbols = 0;
        words = 0;
        lines = 0;
        FILE *fin = fopen(argv[i], "r");
        if (fin == NULL) {
          perror("fopen");
          continue;
        }
        wc(&symbols, &words, &lines, fin);
        fclose(fin);
        total_symbols += symbols;
        total_words += words;
        total_lines += lines;
        printf("%d %d %d %s\n", lines, words, symbols, argv[i]);
      }
      printf("%d %d %d total\n", total_lines, total_words, total_symbols);
    }
  }
  return 0;
}
