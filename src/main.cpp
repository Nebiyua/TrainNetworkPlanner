#include <iostream>
#include <limits>
#include <cstdlib>    
#include "../include/Graph.h"

using namespace std;

// Clear the console
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
    cout << "   ETHIO-TRAIN NETWORK PLANNER (v3.0 Final)   \n";
    cout << "==============================================\n";
    cout << "  [1] Add Station       [2] Add Track\n";
    cout << "  [3] List Network      [4] Check Connectivity\n";
    cout << "  [5] Find Shortest Path (Time/Dist)\n";
    cout << "  [6] Direct Check (1-Hop)\n";
    cout << "  [7] Network Health Stats\n";
    cout << "  [8] Save & Exit\n";
    cout << "==============================================\n";
    cout << "Select Option > ";
}

int main() {
    Graph network;
    int choice;

    network.loadData(); // Load previous network if available

    while (true) {
        clearScreen();
        printBanner();

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (choice == 8) { // Exit
            clearScreen();
            cout << "\nSaving Data...\n";
            network.saveData();
            cout << "Shutdown complete. Goodbye!\n";
            break;
        }

        string s1, s2, code;
        int dist, time;
        int subChoice;

        switch (choice) {
            case 1: // Add Station
                clearScreen();
                cout << "\n--- ADD NEW STATION ---\n(Enter '0' to cancel)\n";
                cin.ignore(); getline(cin, s1);
                if (s1 == "0") break;
                cout << "Code: "; cin >> code;
                network.addStation(s1, code);
                pause();
                break;

            case 2: // Add Track
                clearScreen();
                cout << "\n--- ADD NEW TRACK ---\n(Enter '0' to cancel)\n";
                cin.ignore(); cout << "From: "; getline(cin, s1);
                if (s1 == "0") break;
                cout << "To:   "; getline(cin, s2);
                if (s2 == "0") break;
                cout << "Dist (km): "; cin >> dist;
                cout << "Time (min): "; cin >> time;
                network.addTrack(s1, s2, dist, time);
                pause();
                break;

            case 3: // List Stations
                clearScreen();
                network.listStations();
                pause();
                break;

            case 4: // General Connectivity (BFS)
                clearScreen();
                cout << "\n--- CHECK GENERAL CONNECTIVITY ---\n";
                cin.ignore(); cout << "Start: "; getline(cin, s1);
                cout << "End:   "; getline(cin, s2);
                cout << "\nAnalyzing network...\n";
                cout << (network.isPathExisting(s1, s2) ? 
                         "✅ YES! A path exists.\n" : 
                         "❌ NO! Unreachable.\n");
                pause();
                break;

            case 5: // Shortest Path (Dijkstra)
                clearScreen();
                cout << "\n--- CALCULATE SHORTEST PATH ---\n";
                cin.ignore(); cout << "Start: "; getline(cin, s1);
                cout << "End:   "; getline(cin, s2);
                cout << "\nOptimize for:\n[1] Time (Fastest)\n[2] Distance (Shortest)\n> ";
                cin >> subChoice;
                network.getShortestPath(s1, s2, (subChoice == 1)); 
                pause();
                break;

            case 6: // Direct Track Check
                clearScreen();
                cout << "\n--- DIRECT CONNECTION CHECK ---\n";
                cin.ignore(); cout << "Start: "; getline(cin, s1);
                cout << "End:   "; getline(cin, s2);
                cout << (network.isDirectlyConnected(s1, s2) ? 
                         "✅ YES! Direct track available.\n" : 
                         "❌ NO! No direct track.\n");
                pause();
                break;

            case 7: // Network Health Stats
                clearScreen();
                network.displayNetworkStats();
                pause();
                break;

            default:
                break;
        }
    }
    return 0;
}
