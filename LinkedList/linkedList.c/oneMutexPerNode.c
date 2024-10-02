// gcc -g -Wall -o oneMutexPerNode oneMutexPerNode.c -lpthread
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

struct list_node_s {
    int data;
    struct list_node_s* next;
    pthread_mutex_t node_mutex; // Mutex for each node
};

int  Insert(int value, struct list_node_s** head_p);
void Print(struct list_node_s* head_p);
int  Member(int value, struct list_node_s* head_p);
int  Delete(int value, struct list_node_s** head_p);
void Free_list(struct list_node_s** head_p);
int  Is_empty(struct list_node_s* head_p);
char Get_command(void);
int  Get_value(void);

struct thread_arg {
    int start;
    int end;
    struct list_node_s** head_p;
};

// Function for thread operations
void* thread_func(void* arg) {
    struct thread_arg* args = (struct thread_arg*)arg;
    for (int i = args->start; i < args->end; i++) {
        Insert(i, args->head_p);
    }
    return NULL;
}

int main(void) {
    int n;
    printf("Ingrese el limite de inserts: ");
    scanf("%d", &n);
    struct list_node_s* head_p = NULL;

    // Execute with 1 thread
    clock_t start_time = clock();
    pthread_t thread1;
    struct thread_arg args1 = {0, n, &head_p};
    pthread_create(&thread1, NULL, thread_func, (void*)&args1);
    pthread_join(thread1, NULL);
    clock_t end_time = clock();
    double time_taken_1_thread = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Time taken with 1 thread: %f seconds\n", time_taken_1_thread);
    Free_list(&head_p);

    // Execute with 2 threads
    start_time = clock();
    pthread_t thread2, thread3;
    struct thread_arg args2 = {0, n / 2, &head_p};
    struct thread_arg args3 = {n / 2, n, &head_p};
    pthread_create(&thread2, NULL, thread_func, (void*)&args2);
    pthread_create(&thread3, NULL, thread_func, (void*)&args3);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    end_time = clock();
    double time_taken_2_threads = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Time taken with 2 threads: %f seconds\n", time_taken_2_threads);

    Free_list(&head_p);
    
    return 0;
}

int Insert(int value, struct list_node_s** head_pp) {
    struct list_node_s* curr_p = *head_pp;
    struct list_node_s* pred_p = NULL;
    struct list_node_s* temp_p;

    if (curr_p != NULL) pthread_mutex_lock(&curr_p->node_mutex); // Lock first node

    while (curr_p != NULL && curr_p->data < value) {
        if (pred_p != NULL) pthread_mutex_unlock(&pred_p->node_mutex); // Unlock previous node
        pred_p = curr_p;
        curr_p = curr_p->next;
        if (curr_p != NULL) pthread_mutex_lock(&curr_p->node_mutex); // Lock current node
    }

    if (curr_p == NULL || curr_p->data > value) {
        temp_p = malloc(sizeof(struct list_node_s));
        temp_p->data = value;
        temp_p->next = curr_p;
        pthread_mutex_init(&temp_p->node_mutex, NULL); // Initialize mutex for new node
        if (pred_p == NULL)
            *head_pp = temp_p;
        else
            pred_p->next = temp_p;
        
        if (curr_p != NULL) pthread_mutex_unlock(&curr_p->node_mutex);
        if (pred_p != NULL) pthread_mutex_unlock(&pred_p->node_mutex);
        return 1;
    } else {
        printf("%d is already in the list\n", value);
        if (curr_p != NULL) pthread_mutex_unlock(&curr_p->node_mutex);
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
    return head_p == NULL;
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
