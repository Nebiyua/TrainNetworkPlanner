#include "Cli.h"
#include "Graph.h"
#include <iostream>
#include <string>
#include <limits>
#include <cstring>


#include <conio.h> // For _getch() on Windows
#define CLEAR_SCREEN "\033[2J\033[H"
#define REVERSE_ON "\033[7m"
#define REVERSE_OFF "\033[27m"
#define COLOR_GREEN "\033[32m"
#define COLOR_RESET "\033[0m"

static const char* menuItems[] = {
    "Add Station",
    "Add Track",
    "Delete Station",
    "Network Health Overview",
    "List Stations",
    "Connectivity Check",
    "Fastest Route",
    "Load Data",
    "Save Data",
    "Exit"
};
static const int menuSize = sizeof(menuItems) / sizeof(menuItems[0]);

TrainPlannerCLI::TrainPlannerCLI(Graph& graph) : graph_(graph), selectedMenuIndex_(0), running_(true) {}

void TrainPlannerCLI::run() {
    while (running_) {
        drawUI();
        int ch = _getch();
        handleKeyPress(ch);
    }
    clearScreen();
    std::cout << "Goodbye!\n";
}

void TrainPlannerCLI::drawUI() const {
#ifdef _WIN32
    system("cls");
#else
    std::cout << CLEAR_SCREEN;
#endif
    std::cout << "---------------- MENU ------------------------\n";
    std::cout << "  Menu           Details\n";
    std::cout << "----------------------------------------------\n";
    for (int i = 0; i < menuSize; ++i) {
        std::cout << "  ";
        // Menu column (12 chars)
        std::string menuStr = menuItems[i];
        if (i == selectedMenuIndex_) menuStr = std::string(REVERSE_ON) + menuStr + std::string(REVERSE_OFF);
        int menuLen = (int)menuStr.length();
        std::cout << menuStr;
        for (int j = menuLen; j < 12; ++j) std::cout << " ";
        std::cout << "   ";
        // Details column (29 chars)
        std::string details = (i == selectedMenuIndex_) ? ("Details for " + std::string(menuItems[i])) : "";
        int detailsLen = (int)details.length();
        std::cout << details;
        for (int j = detailsLen; j < 29; ++j) std::cout << " ";
        std::cout << "\n";
        // Optional: horizontal line between items
        // if (i < menuSize - 1) std::cout << "----------------------------------------------\n";
    }
    std::cout << "----------------------------------------------\n";
    std::cout << "Use UP/DOWN arrows to navigate, Enter to select, q to quit.\n";
}

void TrainPlannerCLI::handleKeyPress(int key) {
    if (key == 224) { // Arrow keys
        key = _getch();
        if (key == 72 && selectedMenuIndex_ > 0) selectedMenuIndex_--; // Up
        if (key == 80 && selectedMenuIndex_ < menuSize - 1) selectedMenuIndex_++; // Down
    } else if (key == '\r') { // Enter
        switch (selectedMenuIndex_) {
            case 0: handleAddStation(); break;
            case 1: handleAddTrack(); break;
            case 2: handleDeleteStation(); break;
            case 3: handleNetworkHealthOverview(); break;
            case 4: handleListStations(); break;
            case 5: handleConnectivity(); break;
            case 6: handleFastestRoute(); break;
            case 7: handleLoadData(); break;
            case 8: handleSaveData(); break;
            case 9: running_ = false; break;
            default: handleHelp(); break;
        }
    } else if (key == 'q' || key == 'Q') {
        running_ = false;
    }
}

void TrainPlannerCLI::clearScreen() const {
    std::cout << CLEAR_SCREEN;
}

// ...existing code for readInt, readLine, pause, and menu actions...

int TrainPlannerCLI::readInt(const std::string& prompt, int minVal, int maxVal) {
    while (true) {
        std::cout << prompt;
        int x;
        if (std::cin >> x) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (x >= minVal && x <= maxVal) return x;
        } else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
    std::cout << "Station added.\n";
}

void TrainPlannerCLI::handleAddTrack() {
    std::string from = readLine("From station (name or code): ");
    std::string to   = readLine("To station (name or code): ");
    int dist = readInt("Distance (integer, e.g., km): ", 0, 1000000);
    int time = readInt("Travel time (integer, e.g., minutes): ", 0, 1000000);
    graph_.addTrack(from, to, dist, time);
    std::cout << "Track added.\n";
}

void TrainPlannerCLI::handleDeleteStation() {
    std::string name = readLine("Station name: ");
    if (name.empty()) {
        std::cout << "Name cannot be empty.\n";
        return;
    }
    graph_.deleteStation(name);
    std::cout << "Station deleted.\n";
}

void TrainPlannerCLI::handleNetworkHealthOverview() {
    graph_.networkHealthOverview();
}

void TrainPlannerCLI::handleListStations() {
    graph_.listStations();
}

void TrainPlannerCLI::handleConnectivity() {
    std::string start = readLine("Start station: ");
    std::string end   = readLine("End station: ");
    bool ok = graph_.isPathExisting(start, end);
    std::cout << (ok ? "Connected: a path exists.\n" : "Not connected: no path exists.\n");
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

void TrainPlannerCLI::handleHelp() const {
    std::cout << "Unknown option. Please select a valid menu item.\n";
}



