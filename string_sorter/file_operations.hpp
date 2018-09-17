/*! \file */

#pragma once

#include <sys/stat.h>
#include <fstream>

/*! ---------------------------------------
* Check validity of argument (must be file)
* 
* @param [in] path        Path to checking file
*
* @returns True if valid and false else
*
* -----------------------------------------
*/
bool isValidFile(const char* path) {
    struct stat fileStat;
    
    stat(path, &fileStat);
    if (S_ISREG(fileStat.st_mode)) {
        return true;
    }
    
    return false;
}

/*! ---------------------------------------
* Gets size if file
* 
* @param [in] path        Path to file
*
* @returns Size of file in bytes
*
* @note File path and file must be validated
*       by isValidFile(const char* path)
*
* -----------------------------------------
*/
int getFileSize(const char* path) {
    struct stat fileStat; 
    
    stat(path, &fileStat);
    
    return fileStat.st_size;
}

/*! ---------------------------------------
* Reads file in UNICODE
* 
* @param [in] path        Path to file
* @paran [in] fileSize    Size of file
*
* @returns Pointer on start of read string
*
* @note Fuctions allocates fileSize + 1 blocks
*       of wchar_t for the return value
*       BUT NOT DELETED: it's up to you
*
* -----------------------------------------
*/
wchar_t* readFile(const char* path, int fileSize) {
    wchar_t* result = (wchar_t*)malloc((fileSize + 1) * sizeof(wchar_t));
    std::wifstream is{path};
    
    is.read(result, fileSize);
    
    return result;
}
