#include "../include/Queue.h"

Queue::Queue() : front(nullptr), rear(nullptr) {}

// Add an element to the back of the queue
void Queue::enqueue(int id) {
    QueueNode* newNode = new QueueNode{ id, nullptr };

    if (isEmpty()) {
        front = rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }
}

// Remove and return the element at the front
int Queue::dequeue() {
    if (isEmpty()) return -1;

    QueueNode* temp = front;
    int data = temp->stationId;

    front = front->next;
    if (!front) rear = nullptr;

    delete temp;
    return data;
}

// Check if the queue is empty
bool Queue::isEmpty() {
    return front == nullptr;
}
