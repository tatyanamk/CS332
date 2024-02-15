#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

void traverseDir(const char *dirname);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <dirname>\n", argv[0]);
        exit(-1);
    }

    traverseDir(argv[1]);

    return 0;
}

void traverseDir(const char *dirname) {
    DIR *dir;
    struct dirent *entry;
    struct stat file_info;
    char path[1024];

    if (!(dir = opendir(dirname))) {
        fprintf(stderr, "Cannot open directory '%s'\n", dirname);
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(path, sizeof(path), "%s/%s", dirname, entry->d_name);

        if (stat(path, &file_info) < 0) {
            fprintf(stderr, "Cannot stat file '%s'\n", path);
            continue;
        }

        if (S_ISDIR(file_info.st_mode)) {
           
            printf("Directory Name: %s\n", path);
            traverseDir(path);
        } else {
          
            printf("File: %s\n", path);
        }
    }

    closedir(dir);
}
