#include "../include/LinkedList.h"
#include <iostream>

LinkedList::LinkedList() : head(nullptr) {}

// Add a new track at the front of the list
void LinkedList::addTrack(int destId, int dist, int time) {
    Track* newTrack = new Track(destId, dist, time);
    newTrack->next = head;
    head = newTrack;
}

// Print all tracks from this station
void LinkedList::displayTracks() {
    for (Track* current = head; current; current = current->next) {
        cout << " -> To Station ID: " << current->destinationStationId
             << " (" << current->weightDistance << "km, "
             << current->weightTime << " min)" << endl;
    }
}

// Return the head of the list
Track* LinkedList::getHead() {
    return head;
}

// Delete all tracks to free memory
LinkedList::~LinkedList() {
    Track* current = head;
    while (current) {
        Track* nextNode = current->next;
        delete current;
        current = nextNode;
    }
}
