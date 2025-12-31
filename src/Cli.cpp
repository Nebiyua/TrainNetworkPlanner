#include <iostream>
#include <string>
#include <limits>
#include "../include/Cli.h"
#include "../include/Graph.h"

TrainPlannerCLI::TrainPlannerCLI(Graph& graph) : graph_(graph) {}

void TrainPlannerCLI::run() {
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
            case 6: handleNetworkHealthOverview(); break;
            case 7: handleLoadData();     break;
            case 8: handleSaveData();     break;
            default: std::cout << "Unknown option.\n"; break;
        }

        if (running) pause();
        std::cout << "\n";
    }

    std::cout << "Goodbye!\n";
}

void TrainPlannerCLI::printMenu() const {
    std::cout << "=============================\n";
    std::cout << " Train Network Planner (CLI)\n";
    std::cout << "=============================\n";
    std::cout << "1) Add station\n";
    std::cout << "2) Add station\n";

    std::cout << "3) Add track\n";
    std::cout << "4) List stations\n";
    std::cout << "5) Connectivity check (path exists)\n";
    std::cout << "6) Fastest route (Dijkstra)\n";
    std::cout << "7) Network health overview\n";

    std::cout << "8) Load data\n";
    std::cout << "9) Save data\n";
    std::cout << "0) Exit\n";
}

void TrainPlannerCLI::clearLine() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int TrainPlannerCLI::readInt(const std::string& prompt, int minVal, int maxVal) {
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

std::string TrainPlannerCLI::readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string s;
    std::getline(std::cin, s);
    return s;
}

void TrainPlannerCLI::pause() const {
    std::cout << "\nPress ENTER to continue...";
    std::string dummy;
    std::getline(std::cin, dummy);
}

void TrainPlannerCLI::handleAddStation() {
    std::string name = readLine("Station name: ");
    std::string code = readLine("Station code: ");

    if (name.empty() || code.empty()) {
        std::cout << "Name and code cannot be empty.\n";
        return;
    }

    graph_.addStation(name, code);
    std::cout << "Station added\n";
}

void TrainPlannerCLI::handleDeleteStation() {
    std::string name = readLine("Station name: ");
   

    if (name.empty()) {
        std::cout << "Name cannot be empty.\n";
        return;
    }

    graph_.deleteStation(name);
    std::cout << "Station deleted .\n";
}

void TrainPlannerCLI::handleNetworkHealthOverview() {
    graph_.networkHealthOverview();
}

void TrainPlannerCLI::handleAddTrack() {
    std::string from = readLine("From station (name or code, depending on your Graph design): ");
    std::string to   = readLine("To station (name or code): ");

    int dist = readInt("Distance (integer, e.g., km): ", 0, 1'000'000);
    int time = readInt("Travel time (integer, e.g., minutes): ", 0, 1'000'000);

    graph_.addTrack(from, to, dist, time);
    std::cout << "Track added.\n";
}

void TrainPlannerCLI::handleListStations() {
    graph_.listStations();
}

void TrainPlannerCLI::handleConnectivity() {
    std::string start = readLine("Start station: ");
    std::string end   = readLine("End station: ");

    bool ok = graph_.isPathExisting(start, end);
    std::cout << (ok ? "Connected: a path exists.\n"
                     : "Not connected: no path exists.\n");
}

void TrainPlannerCLI::handleFastestRoute() {
    std::string start = readLine("Start station: ");
    std::string end   = readLine("End station: ");

    graph_.getFastestRoute(start, end);
}

void TrainPlannerCLI::handleLoadData() {
    graph_.loadData();
    std::cout << "Data loaded.\n";
}

void TrainPlannerCLI::handleSaveData() {
    graph_.saveData();
    std::cout << "Data saved.\n";
}

