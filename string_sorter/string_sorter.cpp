/*! \file */

#include "file_operations.hpp"
#include "lines_operations.hpp"
#include "sorting_operations.hpp"
#include <cstdio>

/*! ---------------------------------
* Prints lines of text from input file
* in lexicographical order, lexicographical order
* counted fron the end, initial order
* 
* @note Use function from bash
*       specifying the path to the file
*       with the first argument
*
* -----------------------------------
*/
int main(int argc, char** argv) {
    std::locale::global(std::locale("en_US.UTF-8"));
    
    if (argc <= 1 || !isValidFile(argv[1])) {
        wprintf(L"Невальдный аргумент\n");
        return 0;
    }
    
    int numberOfLines = 0;
    wchar_t** linePointers = nullptr;
    
    numberOfLines = getLinesFromFile(argv[1], linePointers);
    
    wprintf(L"\x1b[1;31mЭнциклопеция в прямом алфавитном порядке:\x1b[0m\n");
    printSortedByBeginnig(numberOfLines, linePointers);
    wprintf(L"\x1b[1;31mЭнциклопеция в порядке с конца:\x1b[0m\n");
    printSortedByEnd(numberOfLines, linePointers);
    wprintf(L"\x1b[1;31mНеповторимый оригинал:\x1b[0m\n");
    printText(numberOfLines, linePointers);
    
    delete linePointers[0];
    delete linePointers;
    
    return 0;
}
