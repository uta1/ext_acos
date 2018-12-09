#pragma once

enum OperType{
    NUMBER = 101,
    VAR = 102,
    PLUS = 201,
    MINUS = 202,
    MUL = 301,
    DIV = 302,
    SIN = 401,
    COS = 402
};

const double EPS = 1e-9;

struct Node{
    OperType oper;
    double value;
    Node* lSon;
    Node* rSon;
};
