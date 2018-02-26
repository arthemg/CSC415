#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


static void processInput();
static void printMenu();
static void printHelloWorld();
static void printListFiles();

void printMenu()
{
    printf("1. Hello World!\n");
    printf("2. List files\n");
    printf("3. Exit\n");
    printf("Please Select->");
}

void printHelloWorld()
{
    pid_t pid;
    pid = fork();

    if(pid < 0)
    {
        fprintf(stderr, "Fork Failed");
    }
    else if (pid == 0)
    {
        printf("Hello World\n\n");
    }
    else
    {
        wait(NULL);
        processInput();
    }
}

void printListFiles()
{
    pid_t pid;
    pid = fork();


    if(pid < 0)
    {
        fprintf(stderr, "Fork Failed");
    }
    else if (pid == 0)
    {
        printf("\n");
        execlp("/bin/ls","ls",NULL);
    }
    else
    {
        wait(NULL);
        printf("\n");
        processInput();

    }
}

void processInput()
{
    printMenu();
    char line[100];
    int input;
    fgets(line, sizeof(line), stdin);
    sscanf(line, "%d", &input);

    switch (input)
    {
        case 1:
            printHelloWorld();
            break;

        case 2:
            printListFiles();
            break;
        case 3:
            exit(0);

        default:
            printf("The selection is incorrect, please choose from 3 options provided:\n\n");
            processInput();
    }


}


int main()
{
    processInput();
    return 0;
}

