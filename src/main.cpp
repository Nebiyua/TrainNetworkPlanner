#include <iostream>
#include "../include/Graph.h"
#include "../include/Cli.h"

using namespace std;



int main() {
    Graph network;
    TrainPlannerCLI cli(network);
    cli.run();
    return 0;
}