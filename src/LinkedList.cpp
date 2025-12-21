#include "../include/LinkedList.h"

LinkedList::LinkedList() {
    head = nullptr; 
}

void LinkedList::addTrack(int destId, int dist, int time) {

    Track* newTrack = new Track(destId, dist, time); 
    newTrack->next = head; 
    head = newTrack; 
}

void LinkedList::displayTracks() {
    Track* current = head; 
    
    while(current != nullptr) {

        cout << " -> To Station ID: " << current->destinationStationId 
             << " (" << current->weightDistance << "km, " 
             << current->weightTime << " min)" << endl;
             
        current = current->next; 
    }
}

Track* LinkedList::getHead() {
    return head;
}

LinkedList::~LinkedList() {
    Track* current = head;
    while (current != nullptr) {
        Track* nextNode = current->next;
        delete current;
        current = nextNode;
    }
}