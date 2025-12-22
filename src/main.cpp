#include <iostream>
#include "../include/Graph.h"

using namespace std;

int main() {
    Graph myNetwork;
    myNetwork.addStation("A", "A");
    myNetwork.addStation("B", "B");
    myNetwork.addStation("C", "C");

    // A -> B (60 mins), B -> C (60 mins) = Total 120
    myNetwork.addTrack("A", "B", 100, 60);
    myNetwork.addTrack("B", "C", 100, 60);
    
    // A -> C Direct (100 mins) = Faster!
    myNetwork.addTrack("A", "C", 200, 100);

    cout << "Checking connectivity A -> C: " << myNetwork.isPathExisting("A", "C") << endl;
    
    cout << "Calculating Fastest Route A -> C..." << endl;
    myNetwork.getFastestRoute("A", "C"); // Should pick the Direct 100min route, not the 120min one

    return 0;
}