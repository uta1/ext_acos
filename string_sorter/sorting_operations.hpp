/*! \file */

#pragma once

#include <algorithm>

/*! ---------------------------------
* Checks argument for belonging to 
* the class of Cyrillic or Latin letters
*
* @param [in] arg       Checking character
*
* @returns True if belons and false else
* 
* -----------------------------------
*/
bool isLetter(wchar_t arg) {
    return (
            arg >= L'a' && arg <= L'z' ||
            arg >= L'A' && arg <= L'Z' ||
            arg >= L'а' && arg <= L'я' ||
            arg >= L'А' && arg <= L'Я'
    );
}

/*! ---------------------------------
* Checks if a-argument less than b-argument
* in lexicographical order or lexicographical 
* order counted from the end
*
* @param [in] a       First argument
* @param [in] b       Second argument
* @param [in] fromEnd Is responsible 
*                     for the direction of 
*                     character-by-symbol comparison
*
* @returns True if less and false else
* 
* @see  isLessDespitePunctuationFromBeginnig(const wchar_t* a, const wchar_t* b),
*       isLessDespitePunctuationFromEnd(const wchar_t* a, const wchar_t* b)
*
* -----------------------------------
*/
bool isLessDespitePunctuation(const wchar_t* a, const wchar_t* b, bool fromEnd = false) {
    int lenOfA = wcslen(a);
    int lenOfB = wcslen(b);
    
    for (
         int aIter = (fromEnd ? lenOfA - 1 : 0), bIter = (fromEnd ? lenOfB - 1 : 0); 
         fromEnd ? (aIter >= 0 && bIter >= 0) : (aIter < lenOfA && bIter < lenOfB);
         //
    ) {
        if (!isLetter(a[aIter])) {
            fromEnd ? --aIter : ++aIter;
            continue;
        }
        if (!isLetter(b[bIter])) {
            fromEnd ? --bIter : ++bIter;
            continue;
        }
        
        if (a[aIter] < b[bIter]) {
            return true;
        }
        if (a[aIter] > b[bIter]) {
            return false;
        }
        
        fromEnd ? --aIter : ++aIter;
        fromEnd ? --bIter : ++bIter;
    }
    
    return lenOfA < lenOfB;
}

/*! ---------------------------------
* Checks if a-argument less than b-argument
* in lexicographical order
*
* @param [in] a       First argument
* @param [in] b       Second argument
*
* @returns True if less and false else
* 
* @see isLessDespitePunctuationFromEnd(const wchar_t* a, const wchar_t* b), 
*      isLessDespitePunctuation(const wchar_t* a, const wchar_t* b, bool fromEnd = false)
* -----------------------------------
*/
bool isLessDespitePunctuationFromBeginnig(const wchar_t* a, const wchar_t* b) {
    return isLessDespitePunctuation(a, b, false);
}

/*! ---------------------------------
* Checks if a-argument less than b-argument
* in lexicographical order counted from the end
*
* @param [in] a       First argument
* @param [in] b       Second argument
*
* @returns True if less and false else
* 
* @see  isLessDespitePunctuationFromBeginnig(const wchar_t* a, const wchar_t* b),
*       isLessDespitePunctuation(const wchar_t* a, const wchar_t* b, bool fromEnd = false)
*
* -----------------------------------
*/
bool isLessDespitePunctuationFromEnd(const wchar_t* a, const wchar_t* b) {
    return isLessDespitePunctuation(a, b, true);
}

/*! ---------------------------------
* Prints text sorted in lexicographical order
*
* @param [in] numberOfLines Size of 
*                           linePointers array
* @param [in] linePointers  Pointer on
*                           array of strings
* 
* -----------------------------------
*/
void printSortedByBeginnig(int numberOfLines, wchar_t** linePointers) {
    wchar_t** copiedLinePointers = getCopy(numberOfLines, linePointers);
    
    std::sort(
              copiedLinePointers, 
              copiedLinePointers + numberOfLines, 
              isLessDespitePunctuationFromBeginnig
    );
    
    printText(numberOfLines, copiedLinePointers);

    delete copiedLinePointers;
}

/*! ---------------------------------
* Prints text sorted in lexicographical order
* counted from the end
*
* @param [in] numberOfLines Size of 
*                           linePointers array
* @param [in] linePointers  Pointer on
*                           array of strings
* 
* -----------------------------------
*/
void printSortedByEnd(int numberOfLines, wchar_t** linePointers) {
    wchar_t** copiedLinePointers = getCopy(numberOfLines, linePointers);
    
    std::sort(
              copiedLinePointers, 
              copiedLinePointers + numberOfLines, 
              isLessDespitePunctuationFromEnd
    );
    
    printText(numberOfLines, copiedLinePointers);

    delete copiedLinePointers;
}

