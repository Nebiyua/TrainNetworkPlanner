#include <iostream>
#include <limits>
#include <cstdlib> // For system()
#include "../include/Graph.h"

using namespace std;

// --- HELPER FUNCTIONS ---

void clearScreen() {
    // Check operating system to run the correct clear command
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pause() {
    cout << "\n\033[1;33m[Press Enter to return to Main Menu...]\033[0m";
    // Check if buffer is empty, if not ignore, then wait for get()
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ensure buffer is clear
    cin.get(); // Wait for keypress
}

void printBanner() {
    cout << "\033[1;36m==============================================\033[0m" << endl;
    cout << "\033[1;32m   🚄 ETHIO-TRAIN NETWORK PLANNER (v2.0) 🚄   \033[0m" << endl;
    cout << "\033[1;36m==============================================\033[0m" << endl;
    cout << "  [1] Add Station       " << endl;
    cout << "  [2] Add Track         " << endl;
    cout << "  [3] List Network      " << endl;
    cout << "  [4] Check Connectivity" << endl;
    cout << "  [5] Find Fastest Route" << endl;
    cout << "  [6] Save & Exit       " << endl;
    cout << "\033[1;36m==============================================\033[0m" << endl;
    cout << "\033[1;33mSelect Option > \033[0m";
}

int main() {
    Graph network;
    int choice;
    
    // Auto-load data (Quietly)
    // We can redirect cout to silence the "Loading..." messages if we wanted,
    // but for now, let's just clear the screen immediately after loading.
    network.loadData();
    
    while (true) {
        // 1. START FRESH
        clearScreen();
        printBanner();
        
        // 2. GET INPUT
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (choice == 6) {
            clearScreen();
            cout << "\n\033[1;32mSaving Data...\033[0m" << endl;
            network.saveData(); 
            cout << "Goodbye!" << endl;
            break;
        }

        string s1, s2, code;
        int dist, time;

        // 3. EXECUTE ON A FRESH SCREEN
        switch (choice) {
            case 1:
                clearScreen();
                cout << "\n--- \033[1;34mADD NEW STATION\033[0m ---" << endl;
                cout << "Enter Name: "; 
                cin.ignore(); getline(cin, s1);
                cout << "Enter Code: "; cin >> code;
                network.addStation(s1, code);
                pause(); // Wait so user can see "Success"
                break;

            case 2:
                clearScreen();
                cout << "\n--- \033[1;34mADD NEW TRACK\033[0m ---" << endl;
                cout << "From: "; cin.ignore(); getline(cin, s1);
                cout << "To:   "; getline(cin, s2);
                cout << "Dist: "; cin >> dist;
                cout << "Time: "; cin >> time;
                network.addTrack(s1, s2, dist, time);
                pause();
                break;

            case 3:
                clearScreen();
                // We don't need a header here, listStations has one
                network.listStations();
                pause();
                break;

            case 4:
                clearScreen();
                cout << "\n--- \033[1;34mCHECK CONNECTIVITY\033[0m ---" << endl;
                cout << "Start: "; cin.ignore(); getline(cin, s1);
                cout << "End:   "; getline(cin, s2);
                cout << "\nAnalyzing network..." << endl;
                if (network.isPathExisting(s1, s2)) 
                    cout << "✅ \033[1;32mYES! Connection exists.\033[0m" << endl;
                else 
                    cout << "❌ \033[1;31mNO! No path found.\033[0m" << endl;
                pause();
                break;

            case 5:
                clearScreen();
                cout << "\n--- \033[1;34mCALCULATE FASTEST ROUTE\033[0m ---" << endl;
                cout << "Start: "; cin.ignore(); getline(cin, s1);
                cout << "End:   "; getline(cin, s2);
                cout << "\nRunning Dijkstra Algorithm..." << endl;
                network.getFastestRoute(s1, s2);
                pause();
                break;

            default:
                // If invalid choice, just loop back
                break;
        }
    }

    return 0;
}