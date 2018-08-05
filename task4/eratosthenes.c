#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>
#include <math.h>
#include <tgmath.h>

const int block_size = 100000;
const int t = 10000000;

typedef struct{
    long long a, b;
    long long *primes;
    int size;
    char *new_primes;
    int len;
} pthrData;

void *threadFunc(void* thread_data) {
    pthrData *data = (pthrData*)thread_data;
    int len = data->len;
    data->new_primes = (char*)malloc(len* sizeof(char));
    for (int i = 0; i < len; ++i) {
        data->new_primes[i] = 1;
    }
    for (int i = 0; i < data->size && data->primes[i] * data->primes[i] < data->b; ++i) {
        long long a = (data->a % data->primes[i]) ? (data->a / data->primes[i] + 1) * data->primes[i]
                  : data->a;
        while (a < data->b) {
            data->new_primes[a-data->a] = 0;
            a += data->primes[i];
        }
    }
    return NULL;
}

void parallel_sieve_eratosthenes (long long n, int n_threads) {
    int capacity = 500;
    long long *primes = (long long*)malloc(capacity*sizeof(long long));
    int size = (block_size - 1) / 2 + 1;
    int count = 1;
    primes[0] = 2;
    char *isPrime = (char*)malloc(size*sizeof(char));
    for (int i = 0; i < size; ++i) {
        isPrime[i] = 1;
    }
    for (int i = 3; i*i <= block_size; i += 2) {
        if (isPrime[i / 2]) {
            for (int j = i*i; j <= block_size; j += 2*i) {
                isPrime[j / 2] = 0;
            }
        }
    }
    for (int i = 1; i < size; ++i) {
        if (isPrime[i]) {
            primes[count] = i*2+1;
            count += 1;
            if (count > capacity - 2) {
                capacity  = capacity * 3 / 2;
                primes = (long long*)realloc(primes, capacity*sizeof(long long));
            }
        }
    }
    free(isPrime);
    int res = 0;
    if (n > block_size) {
        int m_threads = 10;
        int m = (n_threads <= m_threads) ? n_threads : m_threads;
        int part_size = t / m + 1;
        pthread_t *threads = (pthread_t*) malloc(m * sizeof(pthread_t));
        pthrData *threadData = (pthrData*) malloc(m * sizeof(pthrData));
        for (long long i = block_size; i <= n; i += m*part_size) {
            long long a = i;
            
            for (int j = 0; j < m; ++j) {
                threadData[j].a = a + j * part_size;
                threadData[j].b = threadData[j].a + part_size;
                threadData[j].len = part_size;
                threadData[j].primes = primes;
                threadData[j].size = count;
                pthread_create(&(threads[j]), NULL, threadFunc, &threadData[j]);
             } 
             for (int j = 0; j < m; ++j) {
                 pthread_join(threads[j], NULL);
             }
            for (int j = 0; j < m; ++j) {
                for (int k = 0; k < part_size; ++k) {
                    if (threadData[j].new_primes[k]) {
                        primes[count] = threadData[j].a + k;
                        count += 1;
                        if (count > capacity - 2) {
                            capacity = capacity * 3 / 2;
                            primes = (long long*)realloc(primes, capacity * sizeof(long long));
                        }
                    }
                }
            }
        }
        for (int j = 0; j < m; ++j) {
          free(threadData[j].new_primes);
        }
        free(threads);
        free(threadData);
    }
    FILE *fin = fopen("test.txt", "w"); 
    for (int i = 0; i < count; ++i) {
        if (primes[i] > n) {
          break;
        }
        fprintf(fin, "%lld\n", primes[i]);
    }
    fclose(fin);
    free(primes);
}

int main (int argc, char **argv) {
    if (argc < 2 || argc > 3) {
        fprintf(stderr, "Usage: %s number_of_threads [max_size]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int n_threads = atoi(argv[1]);
    long long int n = (argc == 2 ? LLONG_MAX : atoll(argv[2]));
    parallel_sieve_eratosthenes (n, n_threads);
    return 0;
}
