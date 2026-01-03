#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "Track.h"
#include <iostream>

using namespace std;

class LinkedList {
private:
    Track* head;

public:
    LinkedList();
    ~LinkedList();

    void addTrack(int destId, int dist, int time);
    void displayTracks();
    Track* getHead();
};

#endif
