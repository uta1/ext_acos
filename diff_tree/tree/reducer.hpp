#pragma once

#include <math.h>

#include "structures.h"
#include "basic_operations.hpp"
#include "latex_operations.hpp"

void makeConst(Node* arg, double c) {
    //del(arg->lSon);
    //del(arg->rSon);
    
    arg->oper = OperType::NUMBER;
    arg->value = c;
}

double calculate(const Node* arg) {
    double lOperand = 0;
    double rOperand = 0;
    
    if (arg->lSon != nullptr) {
        lOperand = arg->lSon->value;
    }
    if (arg->rSon != nullptr) {
        rOperand = arg->rSon->value;
    }
    
    switch (arg->oper) {
      case OperType::PLUS:
        return lOperand + rOperand;
        
      case OperType::MINUS:
        return lOperand - rOperand;
        
      case OperType::MUL:
        return lOperand * rOperand;
        
      case OperType::DIV:
        return lOperand / rOperand;
        
      case OperType::SIN:
        return sin(rOperand);
        
      case OperType::COS:
        return cos(rOperand);
    }
}

bool calculable(const Node* arg) {
    switch (getValence(arg->oper)) {
      case 0:
        return false;
        
      case 1:
        return arg->rSon->oper == OperType::NUMBER;
        
      case 2:
        return arg->lSon->oper == OperType::NUMBER && arg->rSon->oper == OperType::NUMBER;
    }
}

void rebornToSon(Node** father, Node** son) {
    Node* temp = *son;
    *son = nullptr;
    del(*father);
    
    *father = temp;
}

bool tryToCutFactor(Node** arg, bool isLSon) {
    Node* cuttingSon = nullptr;
    Node** aliveSon = nullptr;
    
    if (isLSon) {
        cuttingSon = (*arg)->lSon;
        aliveSon = &((*arg)->rSon);
    } else {
        cuttingSon = (*arg)->rSon;
        aliveSon = &((*arg)->lSon);
    }
    
    if (cuttingSon->oper == OperType::NUMBER) {
        if (fabs(cuttingSon->value) < EPS) {
            makeConst(*arg, 0.0); 
            return true;         
        }
        
        if (fabs(cuttingSon->value - 1.0) < EPS) {
            rebornToSon(arg, aliveSon);
            return true;
        }
    }
    
    return false;
}

bool tryToCutAddend(Node** arg, bool isLSon) {
    Node* cuttingSon = nullptr;
    Node** aliveSon = nullptr;
    
    if (isLSon) {
        cuttingSon = (*arg)->lSon;
        aliveSon = &((*arg)->rSon);
    } else {
        cuttingSon = (*arg)->rSon;
        aliveSon = &((*arg)->lSon);
    }

    if (cuttingSon->oper == OperType::NUMBER && fabs(cuttingSon->value) < EPS) {            
        rebornToSon(arg, aliveSon);
        return true;
    }
    
    return false;
}

void reduce(Node** arg, FILE* latexDumpFile = nullptr, Node* root = nullptr) {
    if (arg == nullptr || getValence((*arg)->oper) == 0) {
        return;
    }
    
    if (*arg == root) {
        printLatex(latexDumpFile, root, 0);
    }
       
    if ((*arg)->lSon != nullptr) {
        reduce(&((*arg)->lSon), latexDumpFile, root);
    }
    if ((*arg)->rSon != nullptr) {
        reduce(&((*arg)->rSon), latexDumpFile, root);
    }
        
    if (calculable(*arg)) {
        (*arg)->value = calculate(*arg);
        (*arg)->oper = OperType::NUMBER;
        
        del((*arg)->lSon);
        del((*arg)->rSon);
        
        (*arg)->lSon = nullptr;
        (*arg)->rSon = nullptr;
        
        printLatex(latexDumpFile, root, 1);
        return;
    }
    
    if ((*arg)->oper == OperType::PLUS) {
        if (tryToCutAddend(arg, true)) {
            return;
        }
        tryToCutAddend(arg, false);
    }
    
    if ((*arg)->oper == OperType::MINUS) {
        if ((*arg)->rSon->oper == OperType::NUMBER && fabs((*arg)->rSon->value) < EPS) {            
            Node* temp = (*arg)->lSon;
            (*arg)->lSon = nullptr;
            del(*arg);
            
            *arg = temp;
        }
    }

    if ((*arg)->oper == OperType::MUL) {
        if (tryToCutFactor(arg, true)) {
            return;
        }
        tryToCutFactor(arg, false);
    }
    
    printLatex(latexDumpFile, root, 1);
}

