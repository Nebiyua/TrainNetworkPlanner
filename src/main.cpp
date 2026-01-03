#include <iostream>
#include <limits>
#include "../include/Graph.h"

using namespace std;

void printBanner() {
    cout << "\n==============================================" << endl;
    cout << "   🚄 ETHIO-TRAIN NETWORK PLANNER (v1.0) 🚄   " << endl;
    cout << "==============================================" << endl;
    cout << "COMMANDS:" << endl;
    cout << "  [1] Add Station       [2] Add Track" << endl;
    cout << "  [3] List Network      [4] Check Connectivity" << endl;
    cout << "  [5] Find Fastest Route" << endl;
    cout << "  [6] Exit              [0] Show Menu Again" << endl;
    cout << "==============================================" << endl;
}

int main() {
    Graph network;
    int choice;
    
    // Optional: Load data if you implement File I/O later
    network.loadData();

    printBanner();

    while (true) {
        cout << "\nTrainPlanner > ";
        
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(); // Consume newline

        if (choice == 6) {
            network.saveData(); 
            cout << "Shutting down... Goodbye!" << endl;
            break;
        }

        string s1, s2, code;
        int dist, time;

        switch (choice) {
            case 0:
                printBanner();
                break;

            case 1:
                cout << "   Name: "; getline(cin, s1);
                cout << "   Code: "; cin >> code;
                network.addStation(s1, code);
                break;

            case 2:
                cout << "   From: "; getline(cin, s1);
                cout << "   To:   "; getline(cin, s2);
                cout << "   Dist: "; cin >> dist;
                cout << "   Time: "; cin >> time;
                network.addTrack(s1, s2, dist, time);
                break;

            case 3:
                network.listStations();
                break;

            case 4:
                cout << "   Start: "; getline(cin, s1);
                cout << "   End:   "; getline(cin, s2);
                if (network.isPathExisting(s1, s2)) cout << "   ✅ Connection Confirmed." << endl;
                else cout << "   ❌ No connection found." << endl;
                break;

            case 5:
                cout << "   Start: "; getline(cin, s1);
                cout << "   End:   "; getline(cin, s2);
                network.getFastestRoute(s1, s2);
                break;

            default:
                cout << "   Unknown command. Type 0 for help." << endl;
        }
    }

    return 0;
}