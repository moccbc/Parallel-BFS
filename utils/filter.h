#define FILTER_TH 10000
/*
    Extracts the labels of neighbors of curr from the graph 
    according to what the flag array indicates.
    The graph is represented by (offset, E) as per the CSR
    format for representing graphs.
    The flag array is a bitset where flag[i] = 1 if the i-th
    neighbor of curr is supposed to be filtered out.
*/
int* filter(int* offset, int* E, int* flag, int curr, int &size) {
    // Getting the size of the neighborhood of curr
    int neigh_size = offset[curr+1] - offset[curr];

    // Getting the prefix sums of the flags so that we know the 
    // size and which neighbors' labels we have to return.
    ip_scan(flag, neigh_size);
    size = flag[neigh_size-1];
    // This is the array that will hold the labels of vertices.
    int* B = new int[size];

    if (flag[0] == 1) B[0] = E[offset[curr]];
    // This is needed because our for loops start at offset[curr]
    // but we need to index to B[0...neigh_size]
    int fix = offset[curr];
    if (neigh_size < FILTER_TH) { // Granularity control
        for (int i = offset[curr]; i < offset[curr] + neigh_size; i++) {
            if (flag[i-fix] != flag[i-1-fix])
                B[flag[i-fix] - 1] = E[i];
        }
    }
    else {
        cilk_for (int i = offset[curr]; i < offset[curr] + neigh_size; i++) {
            if (flag[i-fix] != flag[i-1-fix])
                B[flag[i-fix] - 1] = E[i];
        }
    }

    return B;
}
