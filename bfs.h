#include "edgemap.h"
#define FRONTIER_TH 100000

// Main driver function for parallel bfs
void BFS(int n, int m, int* offset, int* E, int s, int* dist) {
    // Initializing all of the distances to 0
    cilk_for(int i = 0; i < n; i++) dist[i] = -1;
    dist[s] = 0;

    // Initial frontier only consists of the start node
    int fsize = 1;
    int* frontier = new int[fsize];
    frontier[0] = s;

    string prevMode = "sparse";
    string currMode = "sparse";

    // Process level by level
    // But process each level in parallel
    while (fsize > 0) {
        // Sets the mode of the frontier based on how big the current frontier is
        currMode = fsize > FRONTIER_TH ? "dense":"sparse";

        // Get the next frontier
        frontier = edgeMap(n, offset, E, dist, frontier, fsize, prevMode, currMode);
        prevMode = currMode;
    }
    delete[] frontier;
}
