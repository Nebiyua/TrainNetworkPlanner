#ifndef QUEUE_H
#define QUEUE_H

struct QueueNode {
    int stationId;
    QueueNode* next;
};

class Queue {
private:
    QueueNode* front;
    QueueNode* rear;

public:
    Queue();

    void enqueue(int id);
    int dequeue();
    bool isEmpty();
};

#endif
