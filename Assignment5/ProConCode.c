#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define BUFFER_SIZE 16

sem_t full;
sem_t empty;
pthread_mutex_t mutex;

struct Section
{
    int thread_index;
    int num_produced;
};

/**
 * Implementing a simple version of Stack to store the produced items
 */
typedef struct stack
{
    int *values;     
    int maxLength;                         
    int size;               
}stack;

stack stacks;


/**
 * adding new elements to the stack
 * @param stack
 * @param element
 */
void push(stack *stack, int value)
{
    if (stack->size != stack->maxLength)
    {
        stack->values[stack->size] = value;
        stack->size++;
    }
}

/**
 * poping last element off the stack
 */
int pop(stack *stack)
{
    int stackTop;

    if (stack->size != 0)
    {
        stack->size--;
        stackTop = stack->values[stack->size];
    }
    return stackTop;
}

/**
 * Priducer function to create items to be consumed later
 */

void *producer(void *param)
{
    struct Section *section = (struct Section *)param;

    for (int counter = 0; counter < section->num_produced; counter++)
    {
        int value = section->thread_index * section->num_produced + counter;
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        push(&stacks, value);
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
    pthread_exit(NULL);
}
void *consumer(void *param)
{
    int consume = *(int *) param;

    for(int i = 0; i < consume; i++)
    {
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        int value = pop(&stacks);
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        printf("Consumer consumed value : %i\n", value);
    }
    
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    struct Section *section;

    pthread_t *producer_thread;
    pthread_t *consumer_thread;

    int totProducers = atoi(argv[1]);
    int totConsumers = atoi(argv[2]);
    int totItemsProduce = atoi(argv[3]);

    int consumerSections = totItemsProduce / totConsumers;
    int producerSections = totItemsProduce / totProducers;
    
    stacks.values  = malloc(BUFFER_SIZE * sizeof(int));
    stacks.maxLength = BUFFER_SIZE;
    stacks.size = 0;

    //init semophores and mutex
    sem_init(&full, 0, BUFFER_SIZE);
    sem_init(&empty, 0, 0);

    pthread_mutex_init(&mutex, NULL);
	
    // allocate memory and spawn threads
    producer_thread = malloc(totProducers * sizeof(pthread_t));
    consumer_thread = malloc(totConsumers * sizeof(pthread_t));
    section = malloc(totProducers * sizeof(struct Section));

    for (int i = 0; i < totProducers; i++)
    {
        section[i].thread_index = i;
        section[i].num_produced = producerSections;
        pthread_create(&producer_thread[i], NULL, &producer, &section[i]);
    }

    for(int i = 0; i < totConsumers; i++)
    {
        pthread_create(&consumer_thread[i], NULL, &consumer, &consumerSections);
    }

    for (int i = 0; i < totProducers; i++)
    {
        pthread_join(producer_thread[i], NULL);

    }

    return 0;
}
