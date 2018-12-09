#pragma once

#include "../checking_definitions.h"
#include "structures.h"
#include "basic_operations.hpp"
#include "latex_operations.hpp"

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
    (*result)->lSon = nullptr;
    (*result)->rSon = nullptr;
    
    Node* lDiffed = nullptr;
    Node* rDiffed = nullptr;
    Node* cashedNode1 = nullptr;
    Node* cashedNode2 = nullptr;
    Node* nom = nullptr;
    Node* den = nullptr;
    
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
        cashedNode1 = copy(cur->lSon);
        cashedNode2 = copy(cur->rSon);
        
        mulOrDivNomDiff(result, lDiffed, rDiffed, cashedNode1, cashedNode2, OperType::PLUS);
        
        break;
        
      case OperType::DIV:
        diff(cur->lSon, &lDiffed);
        diff(cur->rSon, &rDiffed);
        cashedNode1 = copy(cur->lSon);
        cashedNode2 = copy(cur->rSon);
        
        nom = (Node*)calloc(1, sizeof(Node));
        ALLOC_CHECK(nom);
        
        mulOrDivNomDiff(&nom, lDiffed, rDiffed, cashedNode1, cashedNode2, OperType::MINUS);
        
        den = (Node*)calloc(1, sizeof(Node));
        ALLOC_CHECK(den);
        
        cashedNode2 = copy(cur->rSon);
        den->lSon = cashedNode2;
        cashedNode2 = copy(cur->rSon);
        den->rSon = cashedNode2;
        den->oper = OperType::MUL;
        
        (*result)->lSon = nom;
        (*result)->rSon = den;
        (*result)->oper = OperType::DIV;
        
        break;

      case OperType::SIN:
        cashedNode1 = copy(cur);
        diff(cur->rSon, &rDiffed);
    
        cashedNode1->oper = OperType::COS;
        
        (*result)->lSon = cashedNode1;
        (*result)->rSon = rDiffed;
        (*result)->oper = OperType::MUL;
        
        break;
        
      case OperType::COS:
        cashedNode1 = copy(cur);
        diff(cur->rSon, &rDiffed);
    
        cashedNode1->oper = OperType::SIN;
        
        (*result)->lSon = (Node*)calloc(1, sizeof(Node));
        ALLOC_CHECK((*result)->lSon);
        (*result)->lSon->oper = OperType::NUMBER;
        (*result)->lSon->value = -1.0;
        
        (*result)->rSon = (Node*)calloc(1, sizeof(Node));
        ALLOC_CHECK((*result)->rSon);
        (*result)->rSon->oper = OperType::MUL;
        (*result)->rSon->rSon = rDiffed;
        (*result)->rSon->lSon = cashedNode1;
        
        (*result)->oper = OperType::MUL;
        
        break;
    }
    
    return 0;
}
