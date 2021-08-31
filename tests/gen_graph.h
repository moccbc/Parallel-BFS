pair<int*, int*> gen(int n, int m) {
    int* deg = new int[n];
    int* offset = new int[n];
    int* E = new int[m];
    int m2 = m;
    for (int i = 0; i < n; i++) {
        int ofs = rand() % m2;
        if (i != n-1) {
            deg[i] = ofs;
        }
        else {
            deg[i] = m2;
        }
        m2 -= ofs;
    }
    for (int i = 0; i < n; i++) {
        cout << deg[i] << " ";
    }
    cout << endl;

    ip_scan(deg, n);

    offset[0] = 0;
    cilk_for(int i = 1; i < n; i++) offset[i] = deg[i-1];

    return {offset, E};
}