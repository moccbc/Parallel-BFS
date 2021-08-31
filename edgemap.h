#include "scan.h"
#include "filter.h"
#include "flatten.h"
#include "cas.h"
#include "convert.h"

/*
    Returns a sparse representation of the frontier.
    In sparse mode the frontier is represented by an array holding the
    explicit vertex labels from the original graph. ie) If frontier[i] = v,
    then the node with the label v is in the frontier.
*/
int* edgeMapSparse(int* offset, int* E, int* dist, int* frontier, int &fsize) {
    // This will hold all of the neighbors of the node i that 
    // will be added to the next frontier.
    int** nghs = new int*[fsize];
    // This will hold the number of neighbors of node i that 
    // you are adding to the frontier.
    int* sizes = new int[fsize];

    // For each node in the frontier
    cilk_for(int i = 0; i < fsize; i++) {
        // Current node to process
        int curr = frontier[i];
        int ofs = offset[curr];

        // outSize[j] = 1 if for the i-th node, we are adding it's j-th
        // neighbor to the new frontier.
        int outSize = offset[curr+1] - offset[curr];
        int* out_flag = new int[outSize];
        for(size_t j = 0; j < outSize; j++) out_flag[j] = 0;
        
        // Granularity control to check if you want to process
        // the node in the neighborhood in parallel or sequentially.
        if (outSize < 1000) {
            for(int j = offset[curr]; j < offset[curr+1]; j++) {
                // The neighbor that we are looking at right now
                int ngh = E[j];
                // If the neighbor has not been visited
                if (dist[ngh] == -1 && CAS(&dist[ngh], -1, (dist[curr] + 1))) {
                    out_flag[j-ofs] = 1;
                }
            }
        }
        else {
            cilk_for(int j = offset[curr]; j < offset[curr+1]; j++) {
                // The neighbor that we are looking at right now
                int ngh = E[j];
                // If the neighbor has not been visited
                if (dist[ngh] == -1 && CAS(&dist[ngh], -1, (dist[curr] + 1))) {
                    out_flag[j-ofs] = 1;
                }
            }
        }
        int nghs_size = 0;
        // Filter out the labels of the neighbors from the graph
        nghs[i] = filter(offset, E, out_flag, curr, nghs_size);
        sizes[i] = nghs_size;
        delete[] out_flag;
    }
    // Return a single array of the labels of the nodes that we want to use
    // for our frontier.
    return flatten(nghs, sizes, fsize);
}

/*
    Returns a dense representation of the frontier.
    In dense mode, the frontier is represented by a bitset of 
    size |V|, where the i-th entry is 1 if the i-th node is in the 
    frontier and 0 if it is not.
*/
int* edgeMapDense(int n, int* offset, int* E, int* dist, int* frontier, int &fsize) {
    // Bitset for the frontier
    int* newf_bit = new int[n];
    // size[i] = 1 if the i-th node is in the frontier.
    // This will allow us to run a prefix sum on it to calculate the size of the new frontier.
    int* size = new int[n];

    cilk_for(int i = 0; i < n; i++) {
        newf_bit[i] = 0;
        size[i] = 0;
    }

    // For all of the vertices in the graph
    cilk_for(int i = 0; i < n; i++) {
        // If the vertex has not been explored yet
        if (dist[i] == -1) {
            // For each neighbor of the curr vertex
            for (int j = offset[i]; j < offset[i+1]; j++) {
                // The current neighbor that we are looking at
                int ngh = E[j];
                // If the neighbor is in the frontier
                if (frontier[ngh]) {
                    dist[i] = dist[ngh] + 1;
                    newf_bit[i] = 1;
                    size[i] = 1;
                    break;
                }
            }
        }
    }

    // Calculating the new frontier size
    ip_scan(size, n);
    fsize = size[n-1];
    delete[] size;
    return newf_bit;
}

// Returns the current frontier according to the neccessary representation
int* edgeMap(int n, int* offset, int* E, int* dist, int* frontier, int &fsize, string prevMode, string currMode) {
    // Convert the frontier representation based on the mode
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