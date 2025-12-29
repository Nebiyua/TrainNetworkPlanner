#include <iostream>
#include <string>
#include <limits>
#include "Graph.h"

// ----------------------------
// CLI Controller (adjusted to Graph.h)
// ----------------------------
class TrainPlannerCLI {
public:
    explicit TrainPlannerCLI(Graph& graph) : graph_(graph) {}

    void run() {
        bool running = true;
        while (running) {
            printMenu();
            int choice = readInt("Choose an option: ", 0, 7);
            std::cout << "\n";

            switch (choice) {
                case 0: running = false; break;
                case 1: handleAddStation();   break;
                case 2: handleAddTrack();     break;
                case 3: handleListStations(); break;
                case 4: handleConnectivity(); break;
                case 5: handleFastestRoute(); break;
                case 6: handleLoadData();     break;
                case 7: handleSaveData();     break;
                default: std::cout << "Unknown option.\n"; break;
            }

            if (running) pause();
            std::cout << "\n";
        }

        std::cout << "Goodbye!\n";
    }

private:
    Graph& graph_;

    // ------------ UI helpers ------------
    void printMenu() const {
        std::cout << "=============================\n";
        std::cout << " Train Network Planner (CLI)\n";
        std::cout << "=============================\n";
        std::cout << "1) Add station\n";
        std::cout << "2) Add track\n";
        std::cout << "3) List stations\n";
        std::cout << "4) Connectivity check (path exists)\n";
        std::cout << "5) Fastest route (Dijkstra)\n";
        std::cout << "6) Load data\n";
        std::cout << "7) Save data\n";
        std::cout << "0) Exit\n";
    }

    static void clearLine() {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    int readInt(const std::string& prompt, int minVal, int maxVal) {
        while (true) {
            std::cout << prompt;
            int x;
            if (std::cin >> x) {
                clearLine();
                if (x >= minVal && x <= maxVal) return x;
            } else {
                std::cin.clear();
                clearLine();
            }
            std::cout << "Invalid input. Enter a number in [" << minVal << ", " << maxVal << "].\n";
        }
    }

    std::string readLine(const std::string& prompt) {
        std::cout << prompt;
        std::string s;
        std::getline(std::cin, s);
        return s;
    }

    void pause() const {
        std::cout << "\nPress ENTER to continue...";
        std::string dummy;
        std::getline(std::cin, dummy);
    }

    // ------------ Menu actions mapped to Graph.h ------------

    // Graph::addStation(string name, string code)
    void handleAddStation() {
        std::string name = readLine("Station name: ");
        std::string code = readLine("Station code: ");

        if (name.empty() || code.empty()) {
            std::cout << "Name and code cannot be empty.\n";
            return;
        }

        graph_.addStation(name, code);
        std::cout << "Station added (or updated if your BST handles duplicates).\n";
    }

    // Graph::addTrack(string fromStation, string toStation, int dist, int time)
    void handleAddTrack() {
        std::string from = readLine("From station (name or code, depending on your Graph design): ");
        std::string to   = readLine("To station (name or code): ");

        int dist = readInt("Distance (integer, e.g., km): ", 0, 1'000'000);
        int time = readInt("Travel time (integer, e.g., minutes): ", 0, 1'000'000);

        graph_.addTrack(from, to, dist, time);
        std::cout << "Track added.\n";
    }

    // Graph::listStations()
    void handleListStations() {
        graph_.listStations();
    }

    // Graph::isPathExisting(string start, string end)
    void handleConnectivity() {
        std::string start = readLine("Start station: ");
        std::string end   = readLine("End station: ");

        bool ok = graph_.isPathExisting(start, end);
        std::cout << (ok ? "Connected: a path exists.\n"
                         : "Not connected: no path exists.\n");
    }

    // Graph::getFastestRoute(string start, string end)  (prints route inside Graph)
    void handleFastestRoute() {
        std::string start = readLine("Start station: ");
        std::string end   = readLine("End station: ");

        graph_.getFastestRoute(start, end);
    }

    // Graph::loadData()
    void handleLoadData() {
        graph_.loadData();
        std::cout << "Data loaded.\n";
    }

    // Graph::saveData()
    void handleSaveData() {
        graph_.saveData();
        std::cout << "Data saved.\n";
    }
};

