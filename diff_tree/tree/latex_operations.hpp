#pragma once

#include <stdio.h>

#include "structures.h"

void printLatexIntro(FILE* fd, const Node* node) {
    switch (node->oper) {
      case OperType::PLUS:
        fprintf(fd, "{(");
        break;
        
      case OperType::MINUS:
        fprintf(fd, "{(");
        break;
        
      case OperType::MUL:
        fprintf(fd, "{(");
        break;
      
      case OperType::DIV:
        fprintf(fd, "\\frac{(");
        break;
    }
}

void printLatexInter(FILE* fd, const Node* node) {
    switch (node->oper) {
      case OperType::NUMBER:
        fprintf(fd, "%lf", node->value);
        break;
        
      case OperType::VAR:
        fprintf(fd, "x");
        break;
        
      case OperType::PLUS:
        fprintf(fd, ")}+{(");
        break;
        
      case OperType::MINUS:
        fprintf(fd, ")}-{(");
        break;
        
      case OperType::MUL:
        fprintf(fd, ")} \\cdot {(");
        break;
      
      case OperType::DIV:
        fprintf(fd, ")}{(");
        break;
    }
}

void printLatexFinalize(FILE* fd, const Node* node) {
    if (node->oper != OperType::NUMBER && node->oper != OperType::VAR) {
        fprintf(fd, ")}");
    }
}

void printLatexBody(FILE* fd, const Node* node) {
    if (node == NULL) {
        return;
    }
    
    printLatexIntro(fd, node);
    printLatexBody(fd, node->lSon);
    printLatexInter(fd, node);
    printLatexBody(fd, node->rSon);
    printLatexFinalize(fd, node);
}

void printLatex(FILE* fd, const Node* node) {
    if (fd == NULL) {
        return;
    }

    fprintf(fd, "\n$");
    
    printLatexBody(fd, node);
    
    fprintf(fd, "$\n");
}
