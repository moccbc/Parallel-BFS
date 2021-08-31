#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include <algorithm>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include "../get_time.h"
#include "../scan.h"
#include "gen_graph.h"
using namespace std;

int main(int argc, char** argv) {
    srand(time(0));
    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    int m2 = m;

    pair<int*, int*> G = gen(n, m);
    int* offset = G.first;
    int* E = G.second;

    cout << n << " " << m << endl;
    for (int i = 0; i < n; i++) {
        cout << offset[i] << " ";
    }
    cout << endl;

    delete[] offset;
    delete[] E;

	return 0;
}
