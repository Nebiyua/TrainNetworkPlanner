#include <iostream>
#include "../include/Graph.h"

using namespace std;



int main() {
    Graph network;
    TrainPlannerCLI cli(network);
    cli.run();
    return 0;
}