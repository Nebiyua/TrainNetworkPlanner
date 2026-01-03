#include "../include/Stack.h"
#include <iostream>

using namespace std;

Stack::Stack() : top(nullptr) {}

// Free all nodes on destruction
Stack::~Stack() {
    while (!isEmpty()) pop();
}

// Push a new action onto the stack
void Stack::push(string cmd) {
    StackNode* newNode = new StackNode{ cmd, top };
    top = newNode;
}

// Pop the top action from the stack
string Stack::pop() {
    if (isEmpty()) return "";

    StackNode* temp = top;
    string result = temp->action;
    top = top->next;
    delete temp;

    return result;
}

// Check if stack is empty
bool Stack::isEmpty() {
    return top == nullptr;
}

// Print stack from top to bottom
void Stack::printHistory() {
    cout << "--- Action History (Latest First) ---" << endl;
    for (StackNode* current = top; current; current = current->next) {
        cout << ">> " << current->action << endl;
    }
}
