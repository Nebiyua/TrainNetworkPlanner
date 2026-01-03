# 🇪🇹 Ethio-Train Network Planner

A C++ Data Structures & Algorithms project (CoSc3111) that simulates a railway network management system. It allows users to manage stations, plan routes, and analyze network connectivity using graph algorithms.

## 🚀 Features
- **Network Management:** Add stations and tracks dynamically.
- **Persistence:** Automatically saves/loads data to CSV files (`data/stations.csv`, `data/tracks.csv`).
- **Search:** Efficient station lookup using a **Binary Search Tree (BST)** ($O(\log n)$).
- **Connectivity:** Check if a path exists between cities using **BFS** (Breadth-First Search).
- **Routing:** Find the fastest route using **Dijkstra’s Algorithm** (Priority Queue).
- **Interactive UI:** Clean, ATM-style interface with color-coded feedback.

## 🛠️ Architecture
The project adheres to modular design principles:
- **`include/`**: Header files defining ADTs (Abstract Data Types).
- **`src/`**: Implementation of logic.
- **`data/`**: Persistent storage.

### Data Structures Used:
1.  **Binary Search Tree (BST):** To store Station objects sorted by name.
2.  **Adjacency List (Linked List):** To store tracks leaving each station.
3.  **Stack:** To manage user action history (Undo feature foundation).
4.  **Queue:** To facilitate BFS traversal for connectivity checks.

## 📦 How to Build & Run

### Prerequisites
- A C++ Compiler (G++ recommended)
- Git

### Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/YOUR_USERNAME/TrainNetworkPlanner.git
   cd TrainNetworkPlanner