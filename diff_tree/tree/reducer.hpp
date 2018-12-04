#pragma once

#include "structures.h"
#include "basic_operations.hpp"
#include "latex_operations.hpp"

void makeConst(Node* arg, double c) {
    del(arg->lSon);
    del(arg->rSon);
    
    arg->oper = OperType::NUMBER;
    arg->value = c;
}

double calculate(double lOperand, double rOperand, OperType oper) {
    switch (oper) {
      case OperType::PLUS:
        return lOperand + rOperand;
        
      case OperType::MINUS:
        return lOperand - rOperand;
        
      case OperType::MUL:
        return lOperand * rOperand;
        
      case OperType::DIV:
        return lOperand / rOperand;
    }
}

void reduce(Node** arg, FILE* latexDumpFile = NULL, Node* root = NULL) {
    if (arg == NULL) {
        return;
    }
    
    if ((*arg)->lSon != NULL) {
        reduce(&((*arg)->lSon));
    }
    if ((*arg)->rSon != NULL) {
        reduce(&((*arg)->rSon));
    }
    
    if ((*arg)->oper != OperType::VAR && (*arg)->oper != OperType::NUMBER) {
        if ((*arg)->lSon->oper == OperType::NUMBER && (*arg)->rSon->oper == OperType::NUMBER) {
            (*arg)->value = calculate((*arg)->lSon->value, (*arg)->rSon->value, (*arg)->oper);
            (*arg)->oper = OperType::NUMBER;
            
            del((*arg)->lSon);
            del((*arg)->rSon);
            
            (*arg)->lSon = NULL;
            (*arg)->rSon = NULL;
            
            printLatex(latexDumpFile, root);
            return;
        }
    }
    
    if ((*arg)->oper == OperType::PLUS) {
        if ((*arg)->lSon->oper == OperType::NUMBER && abs((*arg)->lSon->value) < EPS) {            
            Node* temp = (*arg)->rSon;
            (*arg)->rSon = NULL;
            del(*arg);
            
            *arg = temp;
        }
    }
    
    if ((*arg)->oper == OperType::MINUS) {
        if ((*arg)->rSon->oper == OperType::NUMBER && abs((*arg)->rSon->value) < EPS) {            
            Node* temp = (*arg)->lSon;
            (*arg)->lSon = NULL;
            del(*arg);
            
            *arg = temp;
        }
    }

    if ((*arg)->oper == OperType::MUL) {
        if ((*arg)->lSon->oper == OperType::NUMBER && abs((*arg)->lSon->value) < EPS) {  
            makeConst(*arg, 0.0);          
        }
        
        if ((*arg)->rSon->oper == OperType::NUMBER && abs((*arg)->rSon->value) < EPS) {            
            makeConst(*arg, 0.0);
        }
    }
    
    printLatex(latexDumpFile, root);
}

