#pragma once

#include <stdio.h>

#include "structures.h"
#include "basic_operations.hpp"

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
  
      case OperType::SIN:
        fprintf(fd, "\\sin{(");
        break;

      case OperType::COS:
        fprintf(fd, "\\cos{(");
        break;
    }
}

void printLatexFinalize(FILE* fd, const Node* node) {
    if (getValence(node->oper) != 0) {
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

void printLatex(FILE* fd, const Node* node, int mode = -1) {
    if (fd == nullptr) {
        return;
    }

    if (mode >= 0) {
        if (mode == 0) {
            fprintf(fd, "OK, let's go:\n");
        } else {
            switch (rand() % 3) {
              case 0:
                fprintf(fd, "\n$\\newline\n\\newline$\ntransform...\n");
                break;
                
              case 1:
                fprintf(fd, "\n$\\newline\n\\newline$\nnext it's easy:\n");
                break;
                
              case 2:
                fprintf(fd, "\n$\\newline\n\\newline$\nwhich is obviously equals to:\n");
                break;
            }
        }
    }
    
    fprintf(fd, "\n$");
    
    printLatexBody(fd, node);
    
    fprintf(fd, "$\n");
}
