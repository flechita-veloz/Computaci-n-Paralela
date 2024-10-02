#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <time.h>

#define MAX 100
#define THREAD_COUNT 2

sem_t sems[THREAD_COUNT]; // Semaphores
char* input_lines[] = {
    "Pease porridge hot.",
    "Pease porridge cold.",
    "Pease porridge in the pot",
    "Nine days old.",
    NULL 
};


void* Tokenize(void* rank) {
    long my_rank = (long) rank;
    char my_line[MAX];
    char* my_string;
    char* saveptr;
    int count;

    for (int i = 0; input_lines[i] != NULL; i++) {
        sem_wait(&sems[my_rank]); // Wait for my turn to read the line
        strcpy(my_line, input_lines[i]); // Read the line into my_line
        sem_post(&sems[(my_rank + 1) % THREAD_COUNT]); // Signal the next thread

        printf("Thread %ld > my line = %s\n", my_rank, my_line);

        // Tokenization
        count = 0;
        my_string = strtok_r(my_line, " \t\n", &saveptr);
        while (my_string != NULL) {
            count++;
            printf("Thread %ld > string %d = %s\n", my_rank, count, my_string);
            my_string = strtok_r(NULL, " \t\n", &saveptr);
        }
    }
    return NULL;
}

int main(void) {
    pthread_t threads[THREAD_COUNT];
    clock_t start, end;

    // Initialize semaphores
    for (int i = 0; i < THREAD_COUNT; i++) {
        sem_init(&sems[i], 0, (i == 0) ? 1 : 0); // Only the first thread can start
    }

    // Start timer
    start = clock();

    // Create threads
    for (long i = 0; i < THREAD_COUNT; i++) {
        pthread_create(&threads[i], NULL, Tokenize, (void*)i);
    }

    // Wait for threads to finish
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    // Stop timer
    end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", cpu_time_used);

    // Clean up semaphores
    for (int i = 0; i < THREAD_COUNT; i++) {
        sem_destroy(&sems[i]);
    }

    return 0;
}
