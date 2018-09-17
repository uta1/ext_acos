/*! \file */

#pragma once

/*! ---------------------------------
* Replaces all occurrences of UNICODE 
* character with another UNICODE character
*
* @param [in] text      Input string
* @param [in] from      What to replace
* @param [in] to        What replace by
* 
* @note Handles correctly only lines 
*       without the character '\0' 
*       (except the last position)
*
* -----------------------------------
*/
void replaceSymbol(wchar_t* text, wchar_t from, wchar_t to) {
    int length = wcslen(text);
    
    for (int i = 0; i < length; ++i) {
        if (text[i] == from) {
            text[i] = to;
        }
    }
}

/*! ---------------------------------
* Splits a string consisting of glued 
* substrings into substrings
*
* @param [in]  text         Input string
* @param [in]  totalLength  What to replace
* @param [out] linePointers Pointer on
*                           array of strings
*
* @returns Number of splitted lines (numberOfLines)
* 
* @note Fuctions allocates numberOfLines blocks
*       of wchar_t for linePointers 
*       BUT NOT DELETED: it's up to you
* 
* -----------------------------------
*/
int parseToLines(wchar_t* text, int totalLength, wchar_t** &linePointers) {
    int numberOfLines = 0;
    
    int beginnigOfCurrent = 0;
    
    for (int i = 0; i < totalLength; ++i) {
        if (text[i] == L'\0') {
            if (beginnigOfCurrent != i) {
                ++numberOfLines;
            }
            beginnigOfCurrent = i + 1;
        }
    }
    
    linePointers = (wchar_t**)malloc(numberOfLines * sizeof(wchar_t*));
   
    int j = 0;
    beginnigOfCurrent = 0;
    for (int i = 0; i < totalLength; ++i) {
        if (text[i] == L'\0') {
            if (beginnigOfCurrent != i) {
                linePointers[j++] = text + beginnigOfCurrent;
            }
            beginnigOfCurrent = i + 1;
        }
    }
    
    return numberOfLines;
}

/*! ---------------------------------
* Splits a text by path into strings
* separated by EOL removing empty lines
*
* @param [in]  path         Path to file
* @param [out] linePointers Pointer on
*                           array of strings
*
* @returns Number of splitted lines (numberOfLines)
* 
* @note After work numberOfLines blocks of wchar_t*
*       will be allocated for linePointers
*       BUT NOT DELETED: it's up to you
* 
* -----------------------------------
*/
int getLinesFromFile(const char* path, wchar_t** &linePointers) {
    int fileSize = getFileSize(path);
    wchar_t* text = readFile(path, fileSize);
    
    replaceSymbol(text, L'\n', L'\0');
    int numberOfLines = parseToLines(text, fileSize, linePointers);

    return numberOfLines;
}

/*! ---------------------------------
* Make copy of pointers to strings
*
* @param [in] numberOfLines Size of 
*                           linePointers array
* @param [in] linePointers  Pointer on
*                           array of strings
*
* @returns Pointer on copied array (result)
* 
* @note After work numberOfLines blocks of wchar_t*
*       will be allocated for result
*       BUT NOT DELETED: it's up to you
* 
* -----------------------------------
*/
wchar_t** getCopy(int numberOfLines, wchar_t** &linePointers) {
    wchar_t** result = (wchar_t**)malloc(numberOfLines * sizeof(wchar_t*));
    
    for (int i = 0; i < numberOfLines; ++i) {
        result[i] = linePointers[i];
    }
    
    return result;
}

/*! ---------------------------------
* Prints text 
*
* @param [in] numberOfLines Size of 
*                           linePointers array
* @param [in] linePointers  Pointer on
*                           array of strings
* 
* -----------------------------------
*/
void printText(int numberOfLines, wchar_t** linePointers) {
    for (int i = 0; i < numberOfLines; ++i) {
        wprintf(L"%ls\n", linePointers[i]);
    }
    wprintf(L"\n");
}
