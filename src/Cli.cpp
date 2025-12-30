#include "Cli.h"
#include "Graph.h"
#include <iostream>
#include <string>
#include <limits>
#include <cstring>
#include <conio.h> // For _getch() on Windows
#define CLEAR_SCREEN "\033[2J\033[H"
#include "Cli.h"
#include "Graph.h"
#include <iostream>
#include <string>
#include <limits>
#include <cstring>
#include <cctype>
#include <pdcurses.h>

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
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_CYAN); // Highlight
    init_pair(2, COLOR_GREEN, COLOR_BLACK); // Details

    while (running_) {
        drawUI();
        int ch = getch();
        handleKeyPress(ch);
    }
    endwin();
    std::cout << "Goodbye!\n";
}

void TrainPlannerCLI::drawUI() const {
    clear();
    int startx = 2, starty = 1;
    int menuWidth = 16, detailsWidth = 36;
    int totalWidth = menuWidth + detailsWidth + 3;
    int totalHeight = menuSize + 4;

    // Draw borders
    mvhline(starty, startx, '-', totalWidth);
    mvhline(starty + totalHeight - 1, startx, '-', totalWidth);
    mvvline(starty, startx, '|', totalHeight);
    mvvline(starty, startx + menuWidth + 1, '|', totalHeight);
    mvvline(starty, startx + totalWidth, '|', totalHeight);
    mvaddch(starty, startx, '+');
    mvaddch(starty, startx + menuWidth + 1, '+');
    mvaddch(starty, startx + totalWidth, '+');
    mvaddch(starty + totalHeight - 1, startx, '+');
    mvaddch(starty + totalHeight - 1, startx + menuWidth + 1, '+');
    mvaddch(starty + totalHeight - 1, startx + totalWidth, '+');

    // Headers
    mvprintw(starty, startx + 2, "Menu");
    mvprintw(starty, startx + menuWidth + 4, "Details");

    // Menu items and details
    for (int i = 0; i < menuSize; ++i) {
        int row = starty + 2 + i;
        if (i == selectedMenuIndex_) {
            attron(COLOR_PAIR(1));
            mvprintw(row, startx + 2, "%-*s", menuWidth - 2, menuItems[i]);
            attroff(COLOR_PAIR(1));
        } else {
            mvprintw(row, startx + 2, "%-*s", menuWidth - 2, menuItems[i]);
        }
        // Details
        if (i == selectedMenuIndex_) {
            attron(COLOR_PAIR(2));
            mvprintw(row, startx + menuWidth + 4, "Details for %s", menuItems[i]);
            attroff(COLOR_PAIR(2));
        } else {
            mvprintw(row, startx + menuWidth + 4, "%*s", detailsWidth - 2, "");
        }
    }

    // Instructions
    mvprintw(starty + totalHeight, startx, "Use UP/DOWN arrows to navigate, Enter to select, q to quit.");
    refresh();
}

void TrainPlannerCLI::handleKeyPress(int key) {
    if (key == KEY_UP) {
        if (selectedMenuIndex_ > 0) --selectedMenuIndex_;
    } else if (key == KEY_DOWN) {
        if (selectedMenuIndex_ < menuSize - 1) ++selectedMenuIndex_;
    } else if (key == 10 || key == KEY_ENTER) { // Enter key
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



