#include "../include/Stack.h"
#include <iostream>

using namespace std;

Stack::Stack() {
    top = nullptr;
}

Stack::~Stack() {
    while (!isEmpty()) {
        pop();
    }
}

void Stack::push(string cmd) {
    StackNode* newNode = new StackNode;
    newNode->action = cmd;
    
    newNode->next = top;
    top = newNode;
}

string Stack::pop() {
    if (isEmpty()) {
        return "";
    }
    
    StackNode* temp = top;
    string result = temp->action;
    top = top->next;
    
    delete temp;
    
    return result;
}

bool Stack::isEmpty() {
    return (top == nullptr);
}

void Stack::printHistory() {
    StackNode* current = top;
    cout << "--- Action History (Latest First) ---" << endl;
    while (current != nullptr) {
        cout << ">> " << current->action << endl;
        current = current->next;
    }
}