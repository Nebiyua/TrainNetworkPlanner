#include <iostream>
#include <limits>
#include <cstdlib>    // system()
#include "../include/Graph.h"

using namespace std;

// Clear console screen
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Wait for Enter key
void pause() {
    cout << "\n[Press Enter to return to Main Menu...]";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// Print main menu
void printBanner() {
    cout << "\n==============================================\n";
    cout << "   ETHIO-TRAIN NETWORK PLANNER                \n";
    cout << "==============================================\n";
    cout << "  [1] Add Station       \n";
    cout << "  [2] Add Track         \n";
    cout << "  [3] List Network      \n";
    cout << "  [4] Check Connectivity\n";
    cout << "  [5] Find Fastest Route\n";
    cout << "  [6] Save & Exit       \n";
    cout << "==============================================\n";
    cout << "Select Option > ";
}

int main() {
    Graph network;
    int choice;

    network.loadData(); 

    while (true) {
        clearScreen();
        printBanner();

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (choice == 6) {
            clearScreen();
            cout << "\nSaving Data...\n";
            network.saveData();
            cout << "Shutdown complete. Goodbye!\n";
            break;
        }

        string s1, s2, code;
        int dist, time;

        switch (choice) {
            case 1: // Add Station
                clearScreen();
                cout << "\n--- ADD NEW STATION ---\n";
                cout << "(Enter '0' to cancel)\n";
                
                cout << "Name: "; 
                cin.ignore(); 
                getline(cin, s1);
                
                if (s1 == "0") break; // Cancel logic

                cout << "Code: "; 
                cin >> code;
                network.addStation(s1, code);
                pause();
                break;

            case 2: // Add Track
                clearScreen();
                cout << "\n--- ADD NEW TRACK ---\n";
                cout << "(Enter '0' to cancel)\n";

                cout << "From: "; 
                cin.ignore(); 
                getline(cin, s1);
                if (s1 == "0") break;

                cout << "To:   "; 
                getline(cin, s2);
                if (s2 == "0") break;

                cout << "Dist: "; cin >> dist;
                cout << "Time: "; cin >> time;
                network.addTrack(s1, s2, dist, time);
                pause();
                break;

            case 3: // List Stations
                clearScreen();
                // listStations function handles its own printing
                network.listStations();
                pause();
                break;

            case 4: // Check Connectivity
                clearScreen();
                cout << "\n--- CHECK CONNECTIVITY ---\n";
                cout << "(Enter '0' to cancel)\n";

                cout << "Start: "; 
                cin.ignore(); 
                getline(cin, s1);
                if (s1 == "0") break;

                cout << "End:   "; 
                getline(cin, s2);
                
                cout << "\nAnalyzing network...\n";
                if (network.isPathExisting(s1, s2)) {
                    cout << "Result: Connection CONFIRMED.\n";
                } else {
                    cout << "Result: NO connection found.\n";
                }
                pause();
                break;

            case 5: // Fastest Route
                clearScreen();
                cout << "\n--- CALCULATE FASTEST ROUTE ---\n";
                cout << "(Enter '0' to cancel)\n";

                cout << "Start: "; 
                cin.ignore(); 
                getline(cin, s1);
                if (s1 == "0") break;

                cout << "End:   "; 
                getline(cin, s2);
                
                cout << "\nRunning Dijkstra Algorithm...\n";
                network.getFastestRoute(s1, s2);
                pause();
                break;

            default: 
                break;
        }
    }

    return 0;
}