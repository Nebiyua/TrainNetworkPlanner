#ifndef TRACK_H
#define TRACK_H

struct Track {
    int destinationStationId; // The ID of the station we are going to
    int weightDistance;       // KM
    int weightTime;           // Minutes
    Track* next;              // Pointer to the next track in the list

    Track(int dest, int dist, int time) {
        destinationStationId = dest;
        weightDistance = dist;
        weightTime = time;
        next = nullptr;
    }
};

#endif