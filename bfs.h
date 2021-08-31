#include <atomic> 
#include "scan.h"
#include "filter.h"
#include "flatten.h"
#include "util.h"
#define FRONTIER_TH 100000

int* edgeMapSparse(int* offset, int* E, int* dist, 
                   int* frontier, int &fsize) 
{
    int** nghs = new int*[fsize];
    int* sizes = new int[fsize];
    cilk_for(int i = 0; i < fsize; i++) {
        // Current node to process
        int curr = frontier[i];
        int ofs = offset[curr];

        // The flags that will be used to find the next frontier
        int outSize = offset[curr+1] - offset[curr];
        int* out_flag = new int[outSize];
        for(size_t j = 0; j < outSize; j++) out_flag[j] = 0;
        
        // Process each of the neighbors in parallel
        if (outSize < 1000) {
            for(int j = offset[curr]; j < offset[curr+1]; j++) {
                // The neighbor that we are looking at right now
                int ngh = E[j];
                if (dist[ngh] == -1 && CAS(&dist[ngh], -1, (dist[curr] + 1))) {
                    out_flag[j-ofs] = 1;
                }
            }
        }
        else {
            cilk_for(int j = offset[curr]; j < offset[curr+1]; j++) {
                // The neighbor that we are looking at right now
                int ngh = E[j];
                if (dist[ngh] == -1 && CAS(&dist[ngh], -1, (dist[curr] + 1))) {
                    out_flag[j-ofs] = 1;
                }
            }
        }
        int nghs_size = 0;
        nghs[i] = filter(offset, E, out_flag, curr, nghs_size);
        sizes[i] = nghs_size;
        delete[] out_flag;
    }
    return flatten(nghs, sizes, fsize);
}

int* edgeMapDense(int n, int* offset, int* E, int* dist, 
                  int* frontier, int &fsize) 
{
    int* newf_bit = new int[n];
    int* size = new int[n];
    cilk_for(int i = 0; i < n; i++) {
        newf_bit[i] = 0;
        size[i] = 0;
    }
    // For all of the vertices in the graph
    cilk_for(int i = 0; i < n; i++) {
        if (dist[i] == -1) {
            // For each neighbor of the curr vertex
            int ofs = offset[i];
            for (int j = offset[i]; j < offset[i+1]; j++) {
                int ngh = E[j];
                if (frontier[ngh]) {
                    dist[i] = dist[ngh] + 1;
                    newf_bit[i] = 1;
                    size[i] = 1;
                    break;
                }
            }
        }
    }
    ip_scan(size, n);
    fsize = size[n-1];
    delete[] size;
    return newf_bit;
}

int* edgeMap(int n, int* offset, int* E, int* dist, int* frontier, 
             int &fsize, string prevMode, string currMode) 
{
    // Convert the frontier based on the mode
    if (prevMode != currMode) {
        frontier = convert(n, frontier, fsize, currMode);
    }

    if (currMode == "dense") {
        frontier = edgeMapDense(n, offset, E, dist, frontier, fsize);
    }
    else if (currMode == "sparse") {
        frontier = edgeMapSparse(offset, E, dist, frontier, fsize);
    }

    return frontier;
}

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
    int round = 1;
    while (fsize > 0) {
        //cout << "Round: " << round << " " << currMode << endl;
        currMode = fsize > FRONTIER_TH ? "dense":"sparse";
        frontier = edgeMap(n, offset, E, dist, frontier, fsize, prevMode, currMode);
        prevMode = currMode;
        round++;
    }
    delete[] frontier;
}
