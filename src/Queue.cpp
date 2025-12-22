#include "../include/Queue.h"
#include <iostream>

using namespace std;

Queue::Queue() {
    front = nullptr;
    rear = nullptr;
}

void Queue::enqueue(int id) {
    QueueNode* newNode = new QueueNode;
    newNode->stationId = id;
    newNode->next = nullptr;

    if (isEmpty()) {
        front = newNode;
        rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }
}

int Queue::dequeue() {
    if (isEmpty()) return -1;

    QueueNode* temp = front;
    int data = temp->stationId;

    front = front->next;

    if (front == nullptr) {
        rear = nullptr;
    }

    delete temp;
    return data;
}

bool Queue::isEmpty() {
    return (front == nullptr);
}