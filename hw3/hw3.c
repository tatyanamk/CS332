#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

#define vIndex 0
#define lIndex 1
#define sIndex 2
#define s2Index 3
#define tIndex 4
#define eIndex 5  
#define numFlags 6  
#define PATH_SIZE 1024
#define TEMP_SIZE 25

struct command
{
    char **argv;
    char **flagArgs;
    char *path;
    int argc;
};

typedef struct command Command;
typedef void checkFlags(Command c);

void execute_command(const char *command, const char *file) {
    pid_t pid = fork();
    if (pid == 0) {
        
        execl("/bin/sh", "sh", "-c", command, file, NULL);
        exit(EXIT_FAILURE); 
    } else if (pid > 0) {
      
        wait(NULL); 
    } else {
       
        perror("fork");
        exit(EXIT_FAILURE);
    }
}

void doChecks(struct command c, checkFlags *f)
{
    f(c);
}

void getV(Command c)
{
    for (int i = 0; i < c.argc; i++)
    {
        if (strstr(c.argv[i], "-v") != NULL)
        {
            char *temp = "-v is active!";
            int length = strlen(temp);
            c.flagArgs[vIndex] = (char *)malloc(length * sizeof(char));
            strcpy(c.flagArgs[vIndex], temp);
        }
    }
}

void getL(Command c)
{
    for (int i = 0; i < c.argc; i++)
    {
        if (strstr(c.argv[i], "-L") != NULL)
        {
            if (c.argc <= i + 1 || c.argv[i + 1][0] == '-')
            {
                printf("INVALID ARGUMENT SYNTAX; Expecting '-L <max file size>'\n");
                exit(-1);
            }
            else
            {
                char *temp = c.argv[i + 1];
                c.flagArgs[lIndex] = (char *)malloc(strlen(temp) * sizeof(char));
                strcpy(c.flagArgs[lIndex], temp);
            }
        }
    }
}

void getS(Command c)
{
    for (int i = 0; i < c.argc; i++)
    {
        if (strcmp(c.argv[i], "-s") == 0)
        {
            if (c.argc < i + 2 || c.argv[i + 2][0] == '-')
            {
                printf("INVALID ARGUMENT SYNTAX; Expecting '-s <string pattern> <depth>'\n");
                exit(-1);
            }
            else
            {
                char *temp = c.argv[i + 1];
                int len = strlen(temp);
                c.flagArgs[sIndex] = (char *)malloc(len * sizeof(char));
                strcpy(c.flagArgs[sIndex], temp);
                temp = c.argv[i + 2];
                len = strlen(temp);
                c.flagArgs[s2Index] = (char *)malloc(len * sizeof(char));
                strcpy(c.flagArgs[s2Index], temp);
            }
        }
    }
}

void getT(Command c)
{
    for (int i = 0; i < c.argc; i++)
    {
        if (strstr(c.argv[i], "-t") != NULL)
        {
            if ((c.argc <= i + 1) || (strstr(c.argv[i + 1], "f") == NULL && strstr(c.argv[i + 1], "d") == NULL))
            {
                printf("INVALID ARGUMENT SYNTAX; Expecting '-t <file type (f,d)>'\n");
                exit(-1);
            }
            else
            {
                char *temp = c.argv[i + 1];
                c.flagArgs[tIndex] = (char *)malloc(sizeof(char));
                strcpy(c.flagArgs[tIndex], temp);
            }
        }
    }
}

void getE(Command c) {
    for (int i = 0; i < c.argc; i++) {
        if (strcmp(c.argv[i], "-e") == 0) {
            if (c.argc < i + 2 || c.argv[i + 1][0] == '-') {
                printf("INVALID ARGUMENT SYNTAX; Expecting '-e <unix-command with arguments>'\n");
                exit(-1);
            } else {
                char *temp = c.argv[i + 1];
                c.flagArgs[eIndex] = (char *)malloc(strlen(temp) * sizeof(char));
                strcpy(c.flagArgs[eIndex], temp);
            }
        }
    }
}

void *print_permissions(mode_t mode, char *retPerms)
{
    char perms[10] = "---------";
    mode_t flags[] = {S_IRUSR, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP, S_IXGRP, S_IROTH, S_IWOTH, S_IXOTH};

    char perm_chars[10] = "rwxrwxrwx";

    for (int i = 0; i < 9; ++i)
    {
        if (mode & flags[i])
        {
            perms[i] = perm_chars[i];
        }
    }

    strcpy(retPerms, perms);
}

char *getSymLink(struct stat s, char *path)
{
    char *linkTo = (char *)malloc(PATH_SIZE * sizeof(char));

    if (S_ISLNK(s.st_mode))
    {
        ssize_t len = readlink(path, linkTo, s.st_size + 1);

        if (len == -1)
        {
            perror("readlink");
            free(linkTo);
        }
    }

    return linkTo;
}

char *getDetails(struct stat s, char *path, char *details)
{
    details = (char *)malloc(PATH_SIZE * sizeof(char));

    long bytes;
    if (S_ISDIR(s.st_mode))
    {
        bytes = 0;
    }
    else
    {
        bytes = s.st_size;
    }

    char *perms = (char *)malloc(10 * sizeof(char));
    perms = print_permissions(s.st_mode, perms);

    time_t at = s.st_atime;
    char *accessTime = ctime(&at);
    accessTime[strlen(accessTime) - 1] = '\0';

    sprintf(details, "%ld | %s | %s", bytes, perms, accessTime);

    free(perms);
    perms = NULL;

    return details;
}

void choosePrint(struct stat s, char *currentPath, char *details, int det)
{
    char *linkTo = NULL;
    int sym = 0;

    if (S_ISLNK(s.st_mode))
    {
        linkTo = getSymLink(s, currentPath);
        sym = 1;
    }

    char *lastBack = strrchr(currentPath, '/');
    char *lastDir = &lastBack[1];

    if (sym && det)
    {
        printf("%s (%s) %s\n", lastDir, linkTo, details);
    }
    else if (sym)
    {
        printf("%s (%s)\n", lastDir, linkTo);
    }
    else if (det)
    {
        printf("%s %s\n", lastDir, details);
    }
    else
    {
        printf("%s\n", lastDir);
    }
}

void search(char *path, int indent, char **flagArgs)
{
    struct dirent *dirent;
    DIR *parentDir;
    parentDir = opendir(path);

    if (parentDir == NULL)
    {
        printf("Error opening directory: '%s'\n", path);
        exit(-1);
    }

    while ((dirent = readdir(parentDir)) != NULL)
    {
        int nameLen = strlen((*dirent).d_name);
        char *fileName = (char *)malloc(nameLen * sizeof(char));
        strcpy(fileName, (*dirent).d_name);

        if (strcmp(fileName, ".") != 0 && strcmp(fileName, "..") != 0)
        {
            struct stat s;
            char *currentPath = (char *)malloc(PATH_SIZE * sizeof(char));
            strcpy(currentPath, path);
            strcat(currentPath, "/");
            strcat(currentPath, fileName);

            if (lstat(currentPath, &s) == -1)
            {
                perror("stat");
                exit(EXIT_FAILURE);
            }

            if (S_ISDIR(s.st_mode))
            {
                if (flagArgs[tIndex] != NULL)
                {
                    if (strcmp(flagArgs[tIndex], "f") == 0)
                    {
                        continue;
                    }
                }

                char *details = NULL;
                int det = 0;

                if (flagArgs[vIndex] != NULL)
                {
                    details = getDetails(s, currentPath, details);
                    det = 1;
                }

                int count = indent;
                while (count > 0)
                {
                    printf(" ");
                    count--;
                }

                choosePrint(s, currentPath, details, det);

                free(details);
                details = NULL;

                search(currentPath, ++indent, flagArgs);
                --indent;
            }
            else
            {
                char *details = NULL;
                char *linkTo = NULL;
                int flagCount = 0;
                int sym = 0;
                int det = 0;

                for (int i = 0; i < numFlags; i++)
                {
                    if (flagArgs[i] != NULL && i != s2Index)
                    {
                        flagCount++;
                    }
                }

                if (flagCount > 0)
                {
                    if (flagArgs[vIndex] != NULL)
                    {
                        details = getDetails(s, currentPath, details);
                        det = 1;
                    }

                    if (flagArgs[lIndex] != NULL)
                    {
                        if (s.st_size < atoi(flagArgs[lIndex]))
                            continue;
                    }

                    if (flagArgs[sIndex] != NULL)
                    {
                        if ((strstr(fileName, flagArgs[sIndex]) == NULL) || indent > atoi(flagArgs[s2Index]))
                            continue;
                    }

                    if (flagArgs[tIndex] != NULL)
                    {
                        if (strcmp(flagArgs[tIndex], "d") == 0)
                            continue;
                    }

                    int count = indent;
                    while (count > 0)
                    {
                        printf(" ");
                        count--;
                    }

                    choosePrint(s, currentPath, details, det);

                    if (flagArgs[eIndex] != NULL) {
                        execute_command(flagArgs[eIndex], currentPath);  
                    }

                    free(linkTo);
                    linkTo = NULL;
                }
                else
                {
                    int count = indent;
                    while (count > 0)
                    {
                        printf(" ");
                        count--;
                    }

                    choosePrint(s, currentPath, details, 0);
                    free(linkTo);
                    linkTo = NULL;
                }

                free(details);
                details = NULL;
            }

            free(currentPath);
            currentPath = NULL;
        }

        free(fileName);
        fileName = NULL;
    }

    closedir(parentDir);
}

int main(int argc, char **argv)
{
    int indent = 0;
    int numArguments = 6;  
    char **modArgv = &argv[1];
    char **flagArgs = (char **)malloc(numArguments * sizeof(char *));
    Command currentCommand;
    currentCommand.path = NULL;
    currentCommand.argv = modArgv;
    currentCommand.argc = argc - 1;
    currentCommand.flagArgs = flagArgs;

    for (int i = 0; i < numFlags; i++)
    {
        flagArgs[i] = NULL;
    }

    doChecks(currentCommand, getV);
    doChecks(currentCommand, getL);
    doChecks(currentCommand, getS);
    doChecks(currentCommand, getT);
    doChecks(currentCommand, getE);  =

    if (argc == 1)
    {
        currentCommand.path = (char *)malloc(PATH_SIZE * sizeof(char));
        getcwd(currentCommand.path, PATH_SIZE);
    }
    else
    {
        for (int i = 0; i < currentCommand.argc; i++)
        {
            if (i != s2Index && (strstr(currentCommand.argv[i], "/") != NULL) && (strstr(currentCommand.argv[i], "..") != NULL) || (strstr(currentCommand.argv[i], ".") != NULL))
            {
                if (strstr(currentCommand.argv[i], "..") != NULL)
                {
                    currentCommand.path = (char *)malloc(PATH_SIZE * sizeof(char));
                    char *tempPath = (char *)malloc(PATH_SIZE * sizeof(char));
                    getcwd(tempPath, PATH_SIZE);
                    char *lastBack = strrchr(tempPath, '/');
                    if (lastBack != NULL)
                    {
                        *lastBack = '\0';
                    }
                    if (strcmp(currentCommand.argv[i], "../") == 0)
                    {
                        strcpy(currentCommand.path, tempPath);
                    }
                    else if (strstr(currentCommand.argv[i], "../") != NULL)
                    {
                        *currentCommand.argv[i]++;
                        *currentCommand.argv[i]++;
                        strcpy(currentCommand.path, tempPath);
                        strcat(currentCommand.path, currentCommand.argv[i]);
                    }
                    break;
                }
                else
                {
                    currentCommand.path = (char *)malloc(PATH_SIZE * sizeof(char));
                    strcpy(currentCommand.path, currentCommand.argv[i]);
                }
            }
        }

        if (currentCommand.path == NULL)
        {
            printf("ERROR: Directory could not be found, printed current directory\n");
            currentCommand.path = (char *)malloc(PATH_SIZE * sizeof(char));
            getcwd(currentCommand.path, PATH_SIZE);
        }
    }

    int pathLen = strlen(currentCommand.path);
    if (currentCommand.path[pathLen - 1] == '/')
    {
        currentCommand.path[pathLen - 1] = '\0';
    }

    char *lastBack = strrchr(currentCommand.path, '/');
    char *baseDir = &lastBack[1];
    printf("%s\n", baseDir);
    indent++;

    search(currentCommand.path, indent, currentCommand.flagArgs);

    for (int i = 0; i < numFlags; i++)
    {
        free(currentCommand.flagArgs[i]);
    }
    free(flagArgs);

    return 0;
}
