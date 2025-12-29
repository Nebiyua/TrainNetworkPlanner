#ifndef TRAIN_PLANNER_CLI_H
#define TRAIN_PLANNER_CLI_H

#include <string>

// Your backend class is named Graph (from Graph.h)
class Graph;

class TrainPlannerCLI {
public:
    // CLI operates on an existing Graph instance (owned elsewhere).
    explicit TrainPlannerCLI(Graph& graph);

    // Starts the interactive loop (menu -> actions -> repeat).
    void run();

private:
    Graph& graph_;

    // -------- UI helpers --------
    void printMenu() const;
    int  readInt(const std::string& prompt, int minVal, int maxVal);
    std::string readLine(const std::string& prompt);
    void pause() const;

    // -------- Menu actions mapped to Graph.h --------
    void handleAddStation();       // -> graph_.addStation(name, code)
    void handleAddTrack();         // -> graph_.addTrack(from, to, dist, time)
    void handleListStations();     // -> graph_.listStations()
    void handleConnectivity();     // -> graph_.isPathExisting(start, end)
    void handleFastestRoute();     // -> graph_.getFastestRoute(start, end)
    void handleLoadData();         // -> graph_.loadData()
    void handleSaveData();         // -> graph_.saveData()
    void handleHelp() const;
};

#endif // TRAIN_PLANNER_CLI_H
