#define SIZES_TH 100
#define OLD_TH 1000

/*
    Takes a group of arrays and "flattens" them into one
    single array.
*/
int* flatten(int** nghs, int* sizes, int &oldSize) {
    // offset will hold the prefix sum of the sizes
    // array. This will help locate where to put the
    // node labels that are stored in nghs into B
    int* offset = new int[oldSize];
    cilk_for(size_t i = 0; i < oldSize; i++) {
        offset[i] = sizes[i];
    }
    ip_scan(offset, oldSize);

    int newSize = offset[oldSize-1];
    int* B = new int[newSize];

    if (oldSize < OLD_TH) { // Granularity control
        // For each node in the frontier
        for(int i = 0; i < oldSize; i++) {
            // Find the offset so that we know where to put
            // the nodes in B
            int ofs;
            if (i == 0) ofs = 0;
            else ofs = offset[i-1];

            // Add the nodes in the B array
            if (sizes[i] < SIZES_TH) {
                for(int j = 0; j < sizes[i]; j++) {
                    B[ofs+j] = nghs[i][j];
                }
            }
            else {
                cilk_for(int j = 0; j < sizes[i]; j++) {
                    B[ofs+j] = nghs[i][j];
                }
            }

            delete[] nghs[i];
        }
    }
    else {
        cilk_for(int i = 0; i < oldSize; i++) {
            int ofs;
            if (i == 0) ofs = 0;
            else ofs = offset[i-1];

            if (sizes[i] < SIZES_TH) {
                for(int j = 0; j < sizes[i]; j++) {
                    B[ofs+j] = nghs[i][j];
                }
            }
            else {
                cilk_for(int j = 0; j < sizes[i]; j++) {
                    B[ofs+j] = nghs[i][j];
                }
            }

            delete[] nghs[i];
        }
    }

    delete[] nghs;
    delete[] sizes;
    delete[] offset;
    oldSize = newSize;
    return B;
}