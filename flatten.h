#define SIZES_TH 100
#define OLD_TH 1000

int* flatten(int** nghs, int* sizes, int &oldSize) {
    int* offset = new int[oldSize];

    cilk_for(size_t i = 0; i < oldSize; i++) {
        offset[i] = sizes[i];
    }

    ip_scan(offset, oldSize);

    int newSize = offset[oldSize-1];
    int* B = new int[newSize];

    if (oldSize < OLD_TH) {
        for(int i = 0; i < oldSize; i++) {
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