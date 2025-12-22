#ifndef QUEUE_H
#define QUEUE_H

// A simple node for the Queue
struct QueueNode {
    int stationId;    // We only need to store the Station ID to track where to go next
    QueueNode* next;
};

class Queue {
private:
    QueueNode* front; // The head of the line (remove from here)
    QueueNode* rear;  // The back of the line (add to here)

public:
    Queue();
    // We should technically have a destructor, but for simplicity, we skip it for now.
    
    void enqueue(int id); // Add to back
    int dequeue();        // Remove from front
    bool isEmpty();       // Is the line empty?
};

#endif