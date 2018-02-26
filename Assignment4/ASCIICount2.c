#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>

#define THREAD_NUM 8
#define BUFFER_SIZE 65536
#define STANDARD_ASCII 128

char buffer[BUFFER_SIZE];
int asciiCount[STANDARD_ASCII];
pthread_mutex_t mutex;


typedef struct
{
    int index;
    int end;
} Section;

void *runner(void *param)
{
    //index for runner method
    int run_index;
    //storage for ascii chars
    char asciiChar;

    //Instantiate Section and cast *param to Section type
    Section section;
    section = *(Section*) param;

    run_index = section.index * section.end;

    //walk over all ascii chars and add them to array and add mutex lock/unlock
    for(int i = 0; (i < section.end) && (run_index < BUFFER_SIZE); i++)
    {

        //pthread_mutex_lock(&mutex);
        asciiChar = buffer[run_index + i];
        asciiCount[asciiChar]++;
        //pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
    int bufferSection;
    Section section[THREAD_NUM]; //divide groups into the number of threads
    pthread_t tid[THREAD_NUM]; //thread identifier for child threads
    //init mutex lock
    //pthread_mutex_init(&mutex, NULL);

    //Look up and read the file into the buffer, if file not found return error and exit
    // after done reading into buffer split the data into the number of threads.
    int dataFile = open(argv[1], O_RDONLY);

    if (dataFile <= -1)
    {
        fprintf(stderr, "Unable to open file!\n");
        return -1;
    }

    //read the file
    dataFile = read(dataFile, buffer, BUFFER_SIZE);
    //divide into the number of threads
    bufferSection = dataFile / THREAD_NUM;

    //close the file
    close(dataFile);

    //Create threads
    for (int i = 0; i < THREAD_NUM; i++)
    {
        section[i].index = i;
        section[i].end = bufferSection;
        pthread_create(&tid[i], NULL, &runner, &section[i]);
    }

    //join all the threads
    for (int i = 0; i < THREAD_NUM; i++)
    {
        pthread_join(tid[i], NULL);
    }

    //Count all the ascii chars and output results
    for (int i = 0; i < STANDARD_ASCII; i++)
    {
        printf("%i occurrences of ", asciiCount[i]);

        if (i < 33 || i == (STANDARD_ASCII-1))
        {
            printf("%#0x \n", i);
        }
        else
        {
            printf("%c \n", i);
        }
    }
}
