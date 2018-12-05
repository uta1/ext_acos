#pragma once

#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "checking_definitions.h"

int isValidFile(const char* path) {
    struct stat fileStat = {0};
    
    stat(path, &fileStat);
    if (S_ISREG(fileStat.st_mode)) {
        return 1;
    }
    
    return 0;
}

int getFileSize(const char* path) {
    struct stat fileStat = {0}; 
    
    stat(path, &fileStat);
    
    return fileStat.st_size;
}

int readFile(const char* path, int fileSize, char** result) {
    *result = (char*)calloc(fileSize + 1, sizeof(char));

    ALLOC_CHECK(result);
    
    int fd = 0;
    unsigned int bytesRead;

    if ((fd = open(path, O_RDONLY)) == -1) {
        fprintf(stderr, "\033[31mError:\033[0m Cannot open %s\n", path);
        return 1;
    }
    
    if ((bytesRead = read(fd, *result, fileSize)) == -1) {
        fprintf(stderr, "\033[31mError:\033[0m Cannot read %s\n", path);
        return 1;
    }
    
    return 0;
}

