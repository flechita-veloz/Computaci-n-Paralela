// gcc -g -Wall -o readWrite readWrite.c -lpthread
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

struct list_node_s {
   int    data;
   struct list_node_s* next;
};

int  Insert(int value, struct list_node_s** head_p);
void Print(struct list_node_s* head_p);
int  Member(int value, struct list_node_s* head_p);
int  Delete(int value, struct list_node_s** head_p);
void Free_list(struct list_node_s** head_p);
int  Is_empty(struct list_node_s* head_p);
char Get_command(void);
int  Get_value(void);

struct timespec start, finish;
double elapsed;

// Función para imprimir el tiempo de ejecución
void printTime(struct timespec start, struct timespec finish) {
    double elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Elapsed time: %.5f seconds\n", elapsed);
}

struct thread_data {
    struct list_node_s** head_pp;
    int ini;
    int fin;
};

// Función que será ejecutada por los hilos
void* thread_func(void* arg) {
    struct thread_data* data = (struct thread_data*)arg;
    for (int i = data->ini; i < data->fin; i++) {
        Insert(i, data->head_pp);
    }
    return NULL;
}

int main(void) {
   int n;
   printf("Ingrese el limite de inserts: ");
   scanf("%d", &n);
    struct list_node_s* head_p = NULL;
    pthread_t thread1, thread2;
    struct timespec start, finish;
   
    // Argumentos para pasar a los hilos
    struct thread_data data1, data2;

    // Inicializar argumentos para el primer hilo
    data1.head_pp = &head_p;
    data1.ini = 0;
    data1.fin = n;

    // Medir el tiempo de ejecución con 1 hilo
    printf("Running with 1 thread:\n");
    clock_gettime(CLOCK_MONOTONIC, &start);
    pthread_create(&thread1, NULL, thread_func, (void*)&data1);
    pthread_join(thread1, NULL);
    clock_gettime(CLOCK_MONOTONIC, &finish);
    printTime(start, finish);
   //  Print(head_p);
    Free_list(&head_p);

    // Inicializar argumentos para los dos hilos
    data1.head_pp = &head_p;
    data1.ini = 0;
    data1.fin = n / 2;

    data2.head_pp = &head_p;
    data2.ini = n / 2;
    data2.fin = n;
   //  Print(head_p);
    // Medir el tiempo de ejecución con 2 hilos
    printf("Running with 2 threads:\n");
    clock_gettime(CLOCK_MONOTONIC, &start);
    pthread_create(&thread1, NULL, thread_func, (void*)&data1);
    pthread_create(&thread2, NULL, thread_func, (void*)&data2);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    clock_gettime(CLOCK_MONOTONIC, &finish);
    printTime(start, finish);
   //  Print(head_p);
    Free_list(&head_p);

    return 0;
}

pthread_rwlock_t rwlock; // Read-write lock

int Insert(int value, struct list_node_s** head_pp) {
    pthread_rwlock_wrlock(&rwlock); // Lock for writing

    struct list_node_s* curr_p = *head_pp;
    struct list_node_s* pred_p = NULL;
    struct list_node_s* temp_p;

    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p == NULL || curr_p->data > value) {
        temp_p = malloc(sizeof(struct list_node_s));
        temp_p->data = value;
        temp_p->next = curr_p;
        if (pred_p == NULL)
            *head_pp = temp_p;
        else
            pred_p->next = temp_p;
        
        pthread_rwlock_unlock(&rwlock); // Unlock after writing
        return 1;
    } else {
        printf("%d is already in the list\n", value);
        pthread_rwlock_unlock(&rwlock); // Unlock after writing
        return 0;
    }
}


void Print(struct list_node_s* head_p) {
   struct list_node_s* curr_p;

   printf("list = ");

   curr_p = head_p;
   while (curr_p != NULL) {
      printf("%d ", curr_p->data);
      curr_p = curr_p->next;
   }
   printf("\n");
}

int  Member(int value, struct list_node_s* head_p) {
   struct list_node_s* curr_p;

   curr_p = head_p;
   while (curr_p != NULL && curr_p->data < value)
      curr_p = curr_p->next;

   if (curr_p == NULL || curr_p->data > value) {
      printf("%d is not in the list\n", value);
      return 0;
   } else {
      printf("%d is in the list\n", value);
      return 1;
   }
}

int Delete(int value, struct list_node_s** head_pp) {
   struct list_node_s* curr_p = *head_pp;
   struct list_node_s* pred_p = NULL;

   while (curr_p != NULL && curr_p->data < value) {
      pred_p = curr_p;
      curr_p = curr_p->next;
   }
   
   if (curr_p != NULL && curr_p->data == value) {
      if (pred_p == NULL) {
         *head_pp = curr_p->next;
         free(curr_p);
      } else { 
         pred_p->next = curr_p->next;
         free(curr_p);
      }
      return 1;
   } else {
      printf("%d is not in the list\n", value);
      return 0;
   }
}

void Free_list(struct list_node_s** head_pp) {
   struct list_node_s* curr_p;
   struct list_node_s* succ_p;

   if (Is_empty(*head_pp)) return;
   curr_p = *head_pp; 
   succ_p = curr_p->next;
   while (succ_p != NULL) {
      free(curr_p);
      curr_p = succ_p;
      succ_p = curr_p->next;
   }
   free(curr_p);
   *head_pp = NULL;
}

int  Is_empty(struct list_node_s* head_p) {
   if (head_p == NULL)
      return 1;
   else
      return 0;
}

char Get_command(void) {
   char c;

   printf("Please enter a command:  ");
   scanf(" %c", &c);
   return c;
}

int  Get_value(void) {
   int val;

   printf("Please enter a value:  ");
   scanf("%d", &val);
   return val;
}
