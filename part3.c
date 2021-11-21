#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

int checkPermissions(char *filename)
{
    struct stat fs;
    int r;
    r = stat(filename, &fs);

    if (r == -1)
    {
        perror("Error Encountered");
        return (0);
    }

    if (filename[0] == 'A')
        filename = "directory";
    else if (filename[0] == '1')
        filename = "output_file_1";
    else if (filename[0] == '2')
        filename = "output_file_2";

    char buffer[100];
    sprintf(buffer, "User has read permission on %s:", filename);
    write(STDOUT_FILENO, buffer, strlen(buffer));

    if (fs.st_mode & S_IRUSR)
    {
        sprintf(buffer, "Yes\n");
        write(STDOUT_FILENO, buffer, strlen(buffer));
    }
    else
    {
        sprintf(buffer, "No\n");
        write(STDOUT_FILENO, buffer, strlen(buffer));
    }

    sprintf(buffer, "User has write permission on %s:", filename);
    write(STDOUT_FILENO, buffer, strlen(buffer));

    if (fs.st_mode & S_IWUSR)
    {
        sprintf(buffer, "Yes\n");
        write(STDOUT_FILENO, buffer, strlen(buffer));
    }
    else
    {
        sprintf(buffer, "No\n");
        write(STDOUT_FILENO, buffer, strlen(buffer));
    }

    sprintf(buffer, "User has execute permission on %s:", filename);
    write(STDOUT_FILENO, buffer, strlen(buffer));

    if (fs.st_mode & S_IXUSR)
    {
        sprintf(buffer, "Yes\n\n");
        write(STDOUT_FILENO, buffer, strlen(buffer));
    }
    else
    {
        sprintf(buffer, "No\n\n");
        write(STDOUT_FILENO, buffer, strlen(buffer));
    }

    sprintf(buffer, "Group has read permission on %s:", filename);
    write(STDOUT_FILENO, buffer, strlen(buffer));

    if (fs.st_mode & S_IRGRP)
    {
        sprintf(buffer, "Yes\n");
        write(STDOUT_FILENO, buffer, strlen(buffer));
    }
    else
    {
        sprintf(buffer, "No\n");
        write(STDOUT_FILENO, buffer, strlen(buffer));
    }

    sprintf(buffer, "Group has write permission on %s:", filename);
    write(STDOUT_FILENO, buffer, strlen(buffer));

    if (fs.st_mode & S_IWGRP)
    {
        sprintf(buffer, "Yes\n");
        write(STDOUT_FILENO, buffer, strlen(buffer));
    }
    else
    {
        sprintf(buffer, "No\n");
        write(STDOUT_FILENO, buffer, strlen(buffer));
    }

    sprintf(buffer, "Group has execute permission on %s:", filename);
    write(STDOUT_FILENO, buffer, strlen(buffer));

    if (fs.st_mode & S_IXGRP)
    {
        sprintf(buffer, "Yes\n\n");
        write(STDOUT_FILENO, buffer, strlen(buffer));
    }
    else
    {
        sprintf(buffer, "No\n\n");
        write(STDOUT_FILENO, buffer, strlen(buffer));
    }

    sprintf(buffer, "Others has read permission on %s:", filename);
    write(STDOUT_FILENO, buffer, strlen(buffer));

    if (fs.st_mode & S_IROTH)
    {
        sprintf(buffer, "Yes\n");
        write(STDOUT_FILENO, buffer, strlen(buffer));
    }
    else
    {
        sprintf(buffer, "No\n");
        write(STDOUT_FILENO, buffer, strlen(buffer));
    }

    sprintf(buffer, "Others has write permission on %s:", filename);
    write(STDOUT_FILENO, buffer, strlen(buffer));

    if (fs.st_mode & S_IWOTH)
    {
        sprintf(buffer, "Yes\n");
        write(STDOUT_FILENO, buffer, strlen(buffer));
    }
    else
    {
        sprintf(buffer, "No\n");
        write(STDOUT_FILENO, buffer, strlen(buffer));
    }
    sprintf(buffer, "Others has execute permission on %s:", filename);
    write(STDOUT_FILENO, buffer, strlen(buffer));

    if (fs.st_mode & S_IXOTH)
    {
        sprintf(buffer, "Yes\n\n");
        write(STDOUT_FILENO, buffer, strlen(buffer));
    }
    else
    {
        sprintf(buffer, "No\n\n");
        write(STDOUT_FILENO, buffer, strlen(buffer));
    }

    return 1;
}

int main(int argc, char *argv[])
{
    char dirName[] = "Assignment";
    int y;
    y = checkPermissions(dirName);
    if (y == 0)
    {
        return (0);
    }

    int x;
    x = chdir("Assignment");

    if (x == -1)
    {
        perror("Error encountered");
        return (0);
    }

    char *inputFileName;
    (inputFileName = strrchr(argv[1], '/')) ? ++inputFileName : (inputFileName = argv[1]);

    char outputFileName1[100] = "1_";
    strcat(outputFileName1, inputFileName);

    checkPermissions(outputFileName1);

    char outputFileName2[100] = "2_";
    strcat(outputFileName2, inputFileName);

    checkPermissions(outputFileName2);
}