#ifndef TRACK_H
#define TRACK_H

struct Track {
    int destinationStationId;
    int weightDistance; //Km
    int weightTime; //Minutes
    Track* next;

    Track(int dest, int dist, int time)
        : destinationStationId(dest),
          weightDistance(dist),
          weightTime(time),
          next(nullptr) {}
};

#endif
