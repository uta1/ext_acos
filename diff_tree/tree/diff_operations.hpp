#pragma once

#include "../checking_definitions.h"
#include "structures.h"
#include "basic_operations.hpp"

int diff(const Node* cur, Node** result);

void linDiff(const Node* cur, Node* curRes, int operType) {
    curRes->oper = (OperType)operType;
    
    diff(cur->lSon, &(curRes->lSon));
    diff(cur->rSon, &(curRes->rSon));
}

void mulOrDivNomDiff(Node** result, Node* lDiffed, Node* rDiffed, Node* lCopy, Node* rCopy, OperType oper) {
    (*result)->lSon = (Node*)calloc(1, sizeof(Node));
    ALLOC_CHECK((*result)->lSon);
    (*result)->lSon->oper = OperType::MUL;
    (*result)->lSon->lSon = lDiffed;
    (*result)->lSon->rSon = rCopy;
    
    (*result)->rSon = (Node*)calloc(1, sizeof(Node));
    ALLOC_CHECK((*result)->rSon);
    (*result)->rSon->oper = OperType::MUL;
    (*result)->rSon->lSon = lCopy;
    (*result)->rSon->rSon = rDiffed;
    
    (*result)->oper = oper;
}

int diff(const Node* cur, Node** result) {
    if (*result == cur) {
        return 1;
    }
    
    *result = (Node*)calloc(1, sizeof(Node));
    ALLOC_CHECK(*result);
    (*result)->lSon = NULL;
    (*result)->rSon = NULL;
    
    Node* lDiffed = NULL;
    Node* rDiffed = NULL;
    Node* lCopy = NULL;
    Node* rCopy = NULL;
    Node* nom = NULL;
    Node* den = NULL;
    
    switch (cur->oper) {
      case OperType::NUMBER:
        (*result)->oper = OperType::NUMBER;
        (*result)->value = 0.0;
        break;
        
      case OperType::VAR:
        (*result)->oper = OperType::NUMBER;
        (*result)->value = 1.0;
        break;
        
      case OperType::PLUS:
        linDiff(cur, *result, (int)OperType::PLUS);
        break;
      
      case OperType::MINUS:
        linDiff(cur, *result, (int)OperType::MINUS);
        break;
        
      case OperType::MUL:
        diff(cur->lSon, &lDiffed);
        diff(cur->rSon, &rDiffed);
        lCopy = copy(cur->lSon);
        rCopy = copy(cur->rSon);
        
        mulOrDivNomDiff(result, lDiffed, rDiffed, lCopy, rCopy, OperType::PLUS);
        
        break;
        
      case OperType::DIV:
        diff(cur->lSon, &lDiffed);
        diff(cur->rSon, &rDiffed);
        lCopy = copy(cur->lSon);
        rCopy = copy(cur->rSon);
        
        nom = (Node*)calloc(1, sizeof(Node));
        ALLOC_CHECK(nom);
        
        mulOrDivNomDiff(&nom, lDiffed, rDiffed, lCopy, rCopy, OperType::MINUS);
        
        den = (Node*)calloc(1, sizeof(Node));
        ALLOC_CHECK(den);
        
        rCopy = copy(cur->rSon);
        den->lSon = rCopy;
        rCopy = copy(cur->rSon);
        den->rSon = rCopy;
        den->oper = OperType::MUL;
        
        (*result)->lSon = nom;
        (*result)->rSon = den;
        (*result)->oper = OperType::DIV;
        
        break;
    }
    
    return 0;
}
