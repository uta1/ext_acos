#pragma once

#include <stdio.h>

#include "../lib/file_operations.h"
#include "structures.h"
#include "basic_operations.hpp"

int getOperLen(const char* expr, int len, int* operLen) {
    *operLen = 0;
    
    for (; *operLen < len; ++(*operLen)) {
        if (expr[*operLen] == '(') {
            return 0;
        }
    }
    
    return 1;
}

int isPref(const char* expr, const char* pref, int exprLen = -1) {
    int prefLen = strlen(pref);
    
    if (exprLen != -1 && exprLen < prefLen) {
        return 0;
    }
    
    for (int i = 0; i < prefLen; ++i) {
        if (expr[i] != pref[i]) {
            return 0;
        }
    }
    
    return 1;
}

OperType getOperType(const char* expr, int operLen) {
    if (isPref(expr, "+\0") == 1) {
        return OperType::PLUS;
    }
    if (isPref(expr, "-\0") == 1) {
        return OperType::MINUS;
    }
    if (isPref(expr, "*\0") == 1) {
        return OperType::MUL;
    }
    if (isPref(expr, "/\0") == 1) {
        return OperType::DIV;
    }
    if (isPref(expr, "sin\0") == 1) {
        return OperType::SIN;
    }
    if (isPref(expr, "cos\0") == 1) {
        return OperType::COS;
    }
}

int findUnaryOper(const char* expr, int len, int* posOfOperStart) {
    if (isPref(expr + 1, "sin\0", len) == 1) {
        *posOfOperStart = 1;
    }
    if (isPref(expr + 1, "cos\0", len) == 1) {
        *posOfOperStart = 1;
    }
    
    return 0;
}

int findBinOper(const char* expr, int len, int* posOfOperStart) {
    int pos = 1;
    
    int bracesBalance = 1;
    
    *posOfOperStart = -1;
    
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
        
        if (bracesBalance == 1 && *posOfOperStart == -1 && expr[pos] == ')') {
            *posOfOperStart = pos + 1;
        }
    }
    
    return 0;
}

int parseFromString(Node** node, const char* expr, int len = -1) {
    if (len == -1) {
        len = strlen(expr);
    }
    
    if (len < 3 || expr[0] != '(') {    
        return 1;
    }
    
    *node = (Node*)calloc(1, sizeof(Node));
    ALLOC_CHECK(*node);
    (*node)->lSon = nullptr;
    (*node)->rSon = nullptr;
    
    int posOfOperStart = -1;
    int operValence = -1;
    
    if (findUnaryOper(expr, len, &posOfOperStart) == 1) {
        printf("!");
        return 1;
    }
    if (posOfOperStart >= 0) {
        operValence = 1;
        posOfOperStart = 1;
    } else {
        if (findBinOper(expr, len, &posOfOperStart) == 1) {
            return 1;
        }
        
        if (posOfOperStart == -1) {
            operValence = 0;
        } else {
            operValence = 2;
        }
    }
    
    if (operValence == 0) {
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

    int operLen = 0;
    if (getOperLen(expr + posOfOperStart, len - posOfOperStart, &operLen) != 0) {
        return 1;
    }
    
    (*node)->oper = getOperType(expr + posOfOperStart, operLen);
    
    if (operValence == 2) {
        if (
            parseFromString(&((*node)->lSon), expr + 1, posOfOperStart - 1) != 0 || 
            parseFromString(&((*node)->rSon), expr + posOfOperStart + operLen, len - posOfOperStart - operLen - 1) != 0
        ) {
            return 1;
        }
    } else {
        if (
            parseFromString(&((*node)->rSon), expr + posOfOperStart + operLen, len - posOfOperStart - operLen - 1) != 0
        ) {
            return 1;
        }
    }
    
    return 0;
}

int readFromFile(const char* path, Node** root) {
    if (isValidFile(path) == 0) {
        fprintf(stderr, "\033[31mError:\033[0m Argument \"path\" in invalid: not a regular file\n");
        return 1;
    }
    
    int fileSize = getFileSize(path);
    char* text = nullptr;
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
