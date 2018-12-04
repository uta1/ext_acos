#pragma once

#include <stdio.h>

#include "structures.h"

OperType charToOperType(char arg) {
    switch (arg) {
      case '+':
        return OperType::PLUS;
       
      case '-':
        return OperType::MINUS;
       
      case '*':
        return OperType::MUL;
        
      case '/':
        return OperType::DIV;
    }
}

void printData(const Node* node) {
    if (node == NULL) {
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
}

int equalsView(const Node* node1, const Node* node2) {
    if (node1 == NULL && node2 == NULL) {
        return 1;
    }
    
    if (node1 == NULL || node2 == NULL) {
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

void print(const Node* node, int delpth = 0) {
    if (node == NULL) {
        return;
    }    
    
    printf("\n");
    
    for (int i = 0; i < delpth; ++i) {
        printf(" ");
    }
    printf("(");
    
    print(node->lSon, delpth + 1);
    
    for (int i = 0; i < delpth; ++i) {
        printf(" ");
    }
    printData(node);
    
    print(node->rSon, delpth + 1);
    
    for (int i = 0; i < delpth; ++i) {
        printf(" ");
    }
    printf(")");
    printf("\n");
}

void del(Node* node) {
    if (node == NULL) {
        return;
    }
    
    del(node->lSon);
    del(node->rSon);
    
    free(node);
}

Node* copy(const Node* node) {
    if (node == NULL) {
        return NULL;
    }

    Node* result = (Node*)calloc(1, sizeof(Node));
    ALLOC_CHECK(result);
    
    result->oper = node->oper;
    result->value = node->value;
    
    result->lSon = copy(node->lSon);
    result->rSon = copy(node->rSon);
    
    return result;
}
