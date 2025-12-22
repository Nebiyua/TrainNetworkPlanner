#include <iostream>
#include "../include/Graph.h"

using namespace std;

int main() {
    Graph myNetwork;

    cout << "=== Building the Network ===" << endl;
    
    // 1. Add Stations
    myNetwork.addStation("Addis Ababa", "ADD");
    myNetwork.addStation("Adama", "ADM");
    myNetwork.addStation("Dire Dawa", "DD");

    // 2. Add Tracks
    // Addis -> Adama
    myNetwork.addTrack("Addis Ababa", "Adama", 100, 60);
    // Adama -> Dire Dawa
    myNetwork.addTrack("Adama", "Dire Dawa", 350, 240);
    // Error Test
    myNetwork.addTrack("Addis Ababa", "London", 5000, 999);

    // 3. List
    myNetwork.listStations();

    return 0;
}