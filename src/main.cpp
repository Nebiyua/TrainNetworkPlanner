#include <iostream>
#include "../include/LinkedList.h"

using namespace std;

int main() {
    cout << "--- Testing Linked List ---" << endl;
    
    LinkedList addisTracks;
    
    // Adding tracks (Simulating: Addis -> Adama, Addis -> Hawassa)
    // ID 2 = Adama, 100km, 60min
    addisTracks.addTrack(2, 100, 60); 
    
    // ID 3 = Hawassa, 270km, 180min
    addisTracks.addTrack(3, 270, 180);

    cout << "Tracks leaving Addis Ababa:" << endl;
    addisTracks.displayTracks();

    return 0;
}