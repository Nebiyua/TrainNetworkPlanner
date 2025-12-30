#ifndef TRAIN_PLANNER_CLI_H
#define TRAIN_PLANNER_CLI_H

#include <string>


class Graph;


class TrainPlannerCLI {
public:
    explicit TrainPlannerCLI(Graph& graph);
    void run();

private:
    Graph& graph_;

    // TUI state
    int selectedMenuIndex_ = 0;
    bool running_ = true;

    // TUI helpers
    void drawUI() const;
    void handleKeyPress(int key);
    void clearScreen() const;
    void printMenuPane() const;
    void printDetailsPane() const;

    // Input helpers
    int  readInt(const std::string& prompt, int minVal, int maxVal);
    std::string readLine(const std::string& prompt);
    void pause() const;

    // Menu actions
    void handleAddStation();
    void handleAddTrack();
    void handleDeleteStation();
    void handleNetworkHealthOverview();
    void handleListStations();
    void handleConnectivity();
    void handleFastestRoute();
    void handleLoadData();
    void handleSaveData();
    void handleHelp() const;
};

#endif // TRAIN_PLANNER_CLI_H
