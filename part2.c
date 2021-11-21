#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <stdlib.h>

char *strrev(char *string)
{
    //Function to reverse a string
    char temp;
    int length = strlen(string) - 1;
    int l = length + 1;
    for (int i = 0; i < l / 2; i++)
    {
        temp = string[i];
        string[i] = string[length];
        string[length--] = temp;
    }
    return string;
}
int main(int argc, char *argv[])
{
    //double time_spent = 0.0;
    //clock_t begin = clock();

    //argc = 3, number of command line inputs = 3
    //argv[1] = input file name or path(absolute or relative)
    //argv[2] = Number of parts in which the file is divided
    //argv[3] = The part that needs to be reversed

    int x;
    x = *argv[2] - '0';
    int y;
    y = *argv[3] - '0';

    if (!(y >= 1 && y <= x))
    {
        char buff[500];
        sprintf(buff, "The part number to be reversed is greater than the number of parts the file is divided into\n");
        write(STDOUT_FILENO, buff, strlen(buff));
        return (0);
    }

    int inputFile;
    inputFile = open(argv[1], O_RDONLY);

    //input file is opened in read only mode

    if (inputFile == -1)
    {
        //error handling
        perror("Error encountered in opening the input file : ");
        return (0);
    }

    int outputFile;
    char outputFileName[100] = "2_";

    char *inputFileName;
    (inputFileName = strrchr(argv[1], '/')) ? ++inputFileName : (inputFileName = argv[1]);

    strcat(outputFileName, inputFileName);

    //The directory is given read, write and execute permissions for the user
    mkdir("Assignment", 0744);
    chdir("Assignment");

    outputFile = open(outputFileName, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    //The output file is given read and write permissions for the user
    //O_CREAT - open the output file, create it if it doesn't exist

    if (outputFile == -1)
    {
        //error handling
        perror("Error encountered in creating/opening the output file");
        return (0);
    }

    //Place the file descriptor of output file at the beginning
    lseek(outputFile, 0, SEEK_SET);

    long long int fileSize = 0;
    long long int constFileSize;

    fileSize = lseek(inputFile, 0, SEEK_END) - 1;
    constFileSize = fileSize;

    int numberOfBytesPerRead = 1024;

    //We read the file in chunks as its size > 1GB and hence cannot be loaded into the RAM at once
    //Chunk Size - 1KB - 1024 characters/bytes per read
    //We load 1KB of data in the memory everytime we read the input file

    char *contentRead = (char *)malloc(numberOfBytesPerRead * sizeof(char));
    int i = 0;

    //The file of size fileSize is divided into argv[2] parts of size fileSize/argv[2] each
    //The first part will be from character 0 - size-1
    //Second part will be from size to 2*size - 1
    //nth part will be from (n-1)*size to n*size - 1

    long long int sizeOfPartToBeRead = fileSize / x;
    long long int size = sizeOfPartToBeRead; //remains constant

    while (sizeOfPartToBeRead >= numberOfBytesPerRead)
    {
        lseek(inputFile, (y * size) - 1 - (i * numberOfBytesPerRead), SEEK_SET);
        read(inputFile, contentRead, numberOfBytesPerRead);

        strrev(contentRead);
        write(outputFile, contentRead, numberOfBytesPerRead);

        char buffer[50];
        sprintf(buffer, "\rPercentage of file written = %0.2f", 100 * ((double)(size - sizeOfPartToBeRead) / (double)size));
        write(STDOUT_FILENO, buffer, strlen(buffer));
        fflush(stdout);

        i++;
        sizeOfPartToBeRead -= numberOfBytesPerRead;
    }

    lseek(inputFile, (y - 1) * size, SEEK_SET);
    char *LastContentRead = (char *)malloc(sizeOfPartToBeRead * sizeof(char));
    read(inputFile, LastContentRead, sizeOfPartToBeRead);

    strrev(LastContentRead);
    write(outputFile, LastContentRead, strlen(LastContentRead));
    char buffer[50];
    sprintf(buffer, "\rPercentage of file written = %0.2f\n", 100.00);
    write(STDOUT_FILENO, buffer, strlen(buffer));
    fflush(stdout);

    close(inputFile);
    close(outputFile);

    //clock_t end = clock();
    //time_spent = (double)(end - begin);
    //printf("\nTime taken = %f\n", time_spent / CLOCKS_PER_SEC);
}