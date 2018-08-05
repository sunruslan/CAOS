#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>
#include <math.h>
#include <tgmath.h>


unsigned long long sieve (unsigned long long n) {
    FILE *fout = fopen("sieve.txt", "w");
    unsigned long long size = (n - 1) / 2;
    char *isPrime = (char*)malloc((size+1)*sizeof(int));
    for (unsigned long long i = 0; i <= size; ++i) {
        isPrime[i] = 1;
    }
    for (unsigned long long i = 3; i*i <=n; i += 2) {
        if (isPrime[i / 2]) {
            for (unsigned long long j = i*i; j <= n; j += 2*i) {
                isPrime[j / 2] = 0;
            }
        }
    }
    unsigned long long found = n >= 2 ? 1 : 0;
    if (n >= 2) {
      unsigned long long  a = 2;
      fprintf(fout, "%lld\n", a);
    }
    for (unsigned long long i = 1; i <= size; ++i) {
        if (isPrime[i]) {
          fprintf(fout, "%lld\n", i * 2 + 1);
        }
        found += isPrime[i];
    }
    free(isPrime);
    fclose(fout);
    return found;
}
int main (int argc, char **argv) {
  printf("%lld\n", sieve(atoll(argv[1])));
  return 0;
}
