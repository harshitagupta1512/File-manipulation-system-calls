#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
//#include <time.h>

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
    // double time_spent = 0.0;
    //clock_t begin = clock();

    //argc = 1, number of command line inputs = 1
    //argv[1] = input file name or path(absolute or relative)

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
    char outputFileName[100] = "1_";

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
    }

    //Place the file descriptor of output file at the beginning
    lseek(outputFile, 0, SEEK_SET);

    long long int fileSize = 0;
    long long int constFileSize;

    //Place the file descriptor of input file at the end
    fileSize = lseek(inputFile, 0, SEEK_END) - 1;
    constFileSize = fileSize;
    //fileSize = total number of (readable) characters in the file

    int numberOfBytesPerRead = 1024;

    //We read the file in chunks as its size > 1GB and hence cannot be loaded into the RAM at once
    //Chunk Size - 1KB - 1024 characters/bytes per read
    //We load 1KB of data in the memory everytime we read the input file

    char contentRead[numberOfBytesPerRead + 1];
    int i = 1;
    char buffer[500];

    while (fileSize >= numberOfBytesPerRead)
    {
        lseek(inputFile, -i * numberOfBytesPerRead - 1, SEEK_END);
        read(inputFile, contentRead, numberOfBytesPerRead);

        contentRead[numberOfBytesPerRead] = '\0';

        strrev(contentRead);
        write(outputFile, contentRead, numberOfBytesPerRead);

        sprintf(buffer, "\rPercentage of file written = %0.2f", 100 * ((double)(constFileSize - fileSize) / (double)constFileSize));
        write(STDOUT_FILENO, buffer, strlen(buffer));
        fflush(stdout);

        i++;
        fileSize = fileSize - numberOfBytesPerRead;
    }

    lseek(inputFile, 0, SEEK_SET);
    char LastContentRead[fileSize + 1];

    read(inputFile, LastContentRead, fileSize);

    LastContentRead[fileSize] = '\0';
    strrev(LastContentRead);
    write(outputFile, LastContentRead, strlen(LastContentRead));

    sprintf(buffer, "\rPercentage of file written = %0.2f\n", 100.00);
    write(STDOUT_FILENO, buffer, strlen(buffer));
    fflush(stdout);

    close(inputFile);
    close(outputFile);

    //clock_t end = clock();
    //time_spent = (double)(end - begin);
    //printf("\nTime taken = %f\n", time_spent / CLOCKS_PER_SEC);
}