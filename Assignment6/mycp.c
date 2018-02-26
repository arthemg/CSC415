#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define BUFFERSIZE 1024

int main(int argc, char *argv[])
{
    errno = 0;
    char buffer[BUFFERSIZE];
    int sourceFile;
    int destinationFile;
    ssize_t bytesRead;
    ssize_t bytesWritten;
    ssize_t totalBytes;

    if(argc != 3)
    {
        printf("Please provide 2 file names.\n");
        return 1;
    }

    sourceFile = open(argv[1], O_RDONLY);

    if(sourceFile == -1)
    {
        printf("\n Open Source File failed with error [%s]\n",strerror(errno));
        close(sourceFile);
        return 1;
    }

    destinationFile = open(argv[2], O_RDWR |O_EXCL | O_CREAT, S_IRWXU);

        if (destinationFile == -1)
        {
            printf("\n Open Destination File failed with error [%s]\n",strerror(errno));
            close(sourceFile);
            close(destinationFile);
            return 1;
        }


    do
    {
        bytesRead = read(sourceFile, buffer, BUFFERSIZE);

        if(bytesRead < 0)
        {
            printf("There was error reading the file.\n");
            close(sourceFile);
            return 1;
        }

        bytesWritten = write(destinationFile, buffer, bytesRead);

        if(bytesWritten < 0)
        {
            printf("There was error writing to the file.\n");
            close(sourceFile);
            close(destinationFile);
            return 1;
        }

        totalBytes += bytesWritten;

    }while (bytesRead != 0);

    printf("Successfully copied %zd bytes!", totalBytes);

    close(sourceFile);
    close(destinationFile);

    return 0;
}