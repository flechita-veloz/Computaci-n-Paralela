// gcc -fopenmp second.c -o second
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void odd_even_sort(int *a, int n, int thread_count) {
    int phase, i, tmp;
    #pragma omp parallel num_threads(thread_count) \
        default(none) shared(a, n) private(i, tmp, phase)
    for (phase = 0; phase < n; phase++) {
        if (phase % 2 == 0) {
            #pragma omp for
            for (i = 1; i < n; i += 2) {
                if (a[i - 1] > a[i]) {
                    tmp = a[i - 1];
                    a[i - 1] = a[i];
                    a[i] = tmp;
                }
            }
        } else {
            #pragma omp for
            for (i = 1; i < n - 1; i += 2) {
                if (a[i] > a[i + 1]) {
                    tmp = a[i + 1];
                    a[i + 1] = a[i];
                    a[i] = tmp;
                }
            }
        }
    }
}

int main() {
    int n = 10000;  // Tamaño del arreglo
    int thread_count = 4;
    int *a = malloc(n * sizeof(int));

    // Inicializar el arreglo con valores aleatorios
    for (int i = 0; i < n; i++) {
        a[i] = rand() % 10000;
    }

    double start_time = omp_get_wtime();
    odd_even_sort(a, n, thread_count);
    double end_time = omp_get_wtime();

    printf("Tiempo (Segunda implementación): %f segundos\n", end_time - start_time);

    free(a);
    return 0;
}
