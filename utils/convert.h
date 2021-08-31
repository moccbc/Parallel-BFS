/*
    Converts the frontier from sparse -> dense.
    The frontier passed in must be of a sparse representation.
    Below is an example of what happens.
    Nodes:              0 1 2 3 4 5
    Frontier:           0 3 4 5
    New Frontier:       1 0 0 1 1 1
*/
int* convertSparseToDense(int n, int* frontier, int fsize) {
    // This is a bitset where new_frontier[i] = 1 if the
    // i-th node is included in the new frontier
    int* new_frontier = new int[n];
    cilk_for(int i = 0; i < n; i++) {
        new_frontier[i] = 0;
    }

    // Since the passed in frontier is in sparse mode,
    // frontier[i] is the label for the node that
    // has to be added in the frontier.
    if (fsize < 1000) { // Granularity control
        for(int i = 0; i < fsize; i++) {
            new_frontier[frontier[i]] = 1;
        }
    }
    else {
        cilk_for(int i = 0; i < fsize; i++) {
            new_frontier[frontier[i]] = 1;
        }
    }

    return new_frontier;
}

/*
    Converts the frontier from dense -> sparse.
    The frontier passed in must be of a dense representation.
    Below is an example of what happens. Notice that if 
    Frontier_PrefixSum[i-1] != Frontier_Prefixsum[i] then the i-th node
    must be added to new_frontier.
    Nodes:              0 1 2 3 4 5
    Frontier:           1 0 0 1 1 1
    Frontier Prefixsum: 1 1 1 2 3 4
    New Frontier:       0 3 4 5
*/
int* convertDenseToSparse(int n, int* frontier, int &fsize) {
    // Since the passed in frontier is in dense mode,
    // this will get the size of the new frontier.
    ip_scan(frontier, n);
    fsize = frontier[n-1];

    int* new_frontier = new int[fsize];

    if (frontier[0] == 1) new_frontier[0] = 0;
    // For each node in the current (also dense) frontier
    cilk_for (int i = 0; i < n; i++) {
        if (frontier[i-1] != frontier[i])
            new_frontier[frontier[i] - 1] = i;
    }
    return new_frontier;
}

/*
    Converts the frontier from sparse -> dense or dense -> sparse.
    Assumes that the frontier passed in and the new frontier
    are from different representations.
*/
int* convert(int n, int* frontier, int &fsize, string currMode) {
    // Converting from sparse -> dense
    if (currMode == "dense") {
        return convertSparseToDense(n, frontier, fsize);
    }
    // Converting from dense -> sparse
    else if (currMode == "sparse") {
        return convertDenseToSparse(n, frontier, fsize);
    }
}