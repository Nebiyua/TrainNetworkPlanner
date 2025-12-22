#include <iostream>
#include "../include/Stack.h"
#include "../include/Queue.h"

using namespace std;

int main() {
    // --- TEST 1: The Stack (Undo History) ---
    cout << "=== Testing Stack (History) ===" << endl;
    Stack history;
    
    // Simulate user doing things
    cout << "User added Station A..." << endl;
    history.push("Added Station A");
    
    cout << "User added Station B..." << endl;
    history.push("Added Station B");
    
    cout << "User added Track A->B..." << endl;
    history.push("Added Track A->B");

    // Print current history
    history.printHistory();

    // Simulate Undo
    cout << "\n[User presses Undo]..." << endl;
    string lastAction = history.pop();
    cout << "Undid action: " << lastAction << endl;
    
    cout << "\nNew History:" << endl;
    history.printHistory();


    // --- TEST 2: The Queue (BFS Line) ---
    cout << "\n=== Testing Queue (Processing Line) ===" << endl;
    Queue line;
    
    cout << "Enqueuing Station IDs: 10, 20, 30" << endl;
    line.enqueue(10);
    line.enqueue(20);
    line.enqueue(30);

    cout << "Dequeuing (Should be 10): " << line.dequeue() << endl;
    cout << "Dequeuing (Should be 20): " << line.dequeue() << endl;
    
    if (line.isEmpty()) cout << "Queue is empty!" << endl;
    else cout << "Queue still has items." << endl;

    return 0;
}