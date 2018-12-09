#pragma once

#include <stdio.h>

#include "structures.h"

int getValence(OperType oper) {
    switch (oper) {
      case OperType::PLUS:
        return 2;
    
      case OperType::MINUS:
        return 2;
    
      case OperType::MUL:
        return 2;
    
      case OperType::DIV:
        return 2;
        
      case OperType::SIN:
        return 1;
        
      case OperType::COS:
        return 1;
        
      case OperType::VAR:
        return 0;
     
      case OperType::NUMBER:
        return 0;
    }
}

void printData(const Node* node) {
    if (node == nullptr || node == NULL) {
        return;
    }

    if (node->oper == OperType::NUMBER) {
        printf("%lf", node->value);
    }

    if (node->oper == OperType::VAR) {
        printf("x");
    }

    if (node->oper == OperType::PLUS) {
        printf("+");
    }
    
    if (node->oper == OperType::MINUS) {
        printf("-");
    }

    if (node->oper == OperType::MUL) {
        printf("*");
    }
    
    if (node->oper == OperType::DIV) {
        printf("/");
    }
    
    if (node->oper == OperType::SIN) {
        printf("sin");
    }
    
    if (node->oper == OperType::COS) {
        printf("cos");
    }
}

int equalsView(const Node* node1, const Node* node2) {
    if (node1 == nullptr && node2 == nullptr) {
        return 1;
    }
    
    if (node1 == nullptr || node2 == nullptr) {
        return 0;
    }
    
    if (node1->oper != node2->oper) {
        return 0;
    }
    
    if (node1->oper == OperType::NUMBER) {
        return abs(node1->value - node2->value) < EPS;
    }
    
    return equalsView(node1->lSon, node2->lSon) && equalsView(node1->rSon, node2->rSon);
}

void print(const Node* node, int depth = 0);

void printAll(const Node* node, int depth) {
    printf("\n");

    for (int i = 0; i < depth; ++i) {
        printf(" ");
    }
    printf("(");
    if (node == nullptr || node == NULL) {
        return;
    }
    
    print(node->lSon, depth + 1);
    
    for (int i = 0; i < depth; ++i) {
        printf(" ");
    }
    printData(node);
    
    print(node->rSon, depth + 1);
    
    for (int i = 0; i < depth; ++i) {
        printf(" ");
    }
    fprintf(stdout, ")\n");
    
    return;
}

void print(const Node* node, int depth) {
    if (node == nullptr || node == NULL) {
        return;
    }    
    
    int operValence = getValence(node->oper);
    
    switch (operValence) {
      case 0:
        printAll(node, depth);
        break;
        
      case 1:
        printf("\n");

        for (int i = 0; i < depth; ++i) {
            printf(" ");
        }
        printf("(\n");
        
        for (int i = 0; i < depth; ++i) {
            printf(" ");
        }
        printData(node);
        
        print(node->rSon, depth + 1);
        
        for (int i = 0; i < depth; ++i) {
            printf(" ");
        }
        printf(")");
        printf("\n");
        
        break;
        
      case 2:    
        printAll(node, depth);
        break;
    }
    

}

void del(Node* node) {
    if (node == nullptr) {
        return;
    }
    
    del(node->lSon);
    del(node->rSon);
    
    free(node);
}

Node* copy(const Node* node) {
    if (node == nullptr) {
        return nullptr;
    }

    Node* result = (Node*)calloc(1, sizeof(Node));
    ALLOC_CHECK(result);
    
    result->oper = node->oper;
    result->value = node->value;
    
    result->lSon = copy(node->lSon);
    result->rSon = copy(node->rSon);
    
    return result;
}
