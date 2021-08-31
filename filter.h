#define FILTER_TH 10000

int* filter(int* offset, int* E, int* flag, int curr, int &size) {
    int fSize = offset[curr+1] - offset[curr];

    ip_scan(flag, fSize);

    size = flag[fSize-1];
    int* B = new int[size];

    if (flag[0] == 1) B[0] = E[offset[curr]];
    int fix = offset[curr];
    if (fSize < FILTER_TH) {
        for (int i = offset[curr]; i < offset[curr] + fSize; i++) {
            if (flag[i-fix] != flag[i-1-fix])
                B[flag[i-fix] - 1] = E[i];
        }
    }
    else {
        cilk_for (int i = offset[curr]; i < offset[curr] + fSize; i++) {
            if (flag[i-fix] != flag[i-1-fix])
                B[flag[i-fix] - 1] = E[i];
        }
    }

    return B;
}
