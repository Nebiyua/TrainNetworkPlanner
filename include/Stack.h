#ifndef STACK_H
#define STACK_H

#include <string>
#include <iostream>
using namespace std;

struct StackNode {
    string action;
    StackNode* next;
};

class Stack {
private:
    StackNode* top;

public:
    Stack();
    ~Stack();
    
    void push(string action);
    string pop();
    bool isEmpty();
    void printHistory();
};

#endif