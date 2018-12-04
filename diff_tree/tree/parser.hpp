#pragma once

#include <stdio.h>

#include "../lib/file_operations.h"
#include "structures.h"
#include "basic_operations.hpp"

int parseFromString(Node** node, const char* expr, int len = -1) {
    if (len == -1) {
        len = strlen(expr);
    }
    
    if (len < 3 || expr[0] != '(') {
        return 1;
    }
    
    *node = (Node*)calloc(1, sizeof(Node));
    ALLOC_CHECK(*node);
    (*node)->lSon = NULL;
    (*node)->rSon = NULL;
    
    int pos = 1;
    
    int bracesBalance = 1;
    
    int posOfOperSign = -1;
    
    for (pos = 1; bracesBalance != 0; ++pos) {
        if (pos == len) {
            return 1;
        }
        
        if (expr[pos] == '(') {
            ++bracesBalance;
        }
        if (expr[pos] == ')') {
            --bracesBalance;
        }
        
        if (bracesBalance == 1 && posOfOperSign == -1 && expr[pos] == ')') {
            posOfOperSign = pos + 1;
        }
    }
    
    if (posOfOperSign == -1) {
        if (len == 3) {
            if (expr[1] != 'x') {
                return 1;
            }
            
            (*node)->oper = OperType::VAR;
            
            return 0;
        }
        if (len < 5) {
            return 1;
        }
        (*node)->oper = OperType::NUMBER;
        sscanf(expr + 1, "%lf", &((*node)->value));
        
        return 0;
    }
    
    if (
        parseFromString(&((*node)->lSon), expr + 1, posOfOperSign - 1) != 0 || 
        parseFromString(&((*node)->rSon), expr + posOfOperSign + 1, len - posOfOperSign - 2) != 0
    ) {
        return 1;
    }
    
    (*node)->oper = charToOperType(expr[posOfOperSign]);
    
    return 0;
}

int readFromFile(const char* path, Node** root) {
    if (isValidFile(path) == 0) {
        fprintf(stderr, "\033[31mError:\033[0m Argument \"path\" in invalid: not a regular file\n");
        return 1;
    }
    
    int fileSize = getFileSize(path);
    char* text = NULL;
    if (readFile(path, fileSize, &text) != 0) {
        return 1;
    }
    text[fileSize - 1] = '\0';
    
    if (parseFromString(root, text) != 0) {
        fprintf(stderr, "\033[31mParsing error:\033[0m Incorrect expression\n");
        return 1;
    }
    
    free(text);
    return 0;
}
