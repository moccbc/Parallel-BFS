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
#include "../flatten.h"
#include "../filter.h"
using namespace std;

// Assumes that the frontier passed in and the new frontier
// are from different modes
int* convert(int n, int* offset, int* E, int* frontier,
             int &fsize, string currMode) 
{
    int* new_frontier;
    if (currMode == "dense") {
        new_frontier = new int[n];
        cilk_for(int i = 0; i < n; i++) {
            new_frontier[i] = 0;
        }
        cilk_for(int i = 0; i < fsize; i++) {
            new_frontier[frontier[i]] = 1;
        }
    }
    else if (currMode == "sparse") {
        new_frontier = filter2(offset, E, frontier, n, fsize);
    }
    return new_frontier;
}

int main(int argc, char** argv) {
    srand(time(NULL));
    int oldSize = atoi(argv[1]);
    int newSize = 0;

    int** nghs = new int*[oldSize];
    int* sizes = new int[oldSize];

    cilk_for(int i = 0; i < oldSize; i++) {
        sizes[i] = rand() % oldSize;
    }

    for (int i = 0; i < oldSize; i++) {
        nghs[i] = new int[sizes[i]];
    }
    for (int i = 0; i < oldSize; i++) {
        newSize += sizes[i];
        for (int j = 0; j < sizes[i]; j++) {
            nghs[i][j] = rand() % 10;
        }
    }

    //for (int i = 0; i < oldSize; i++) {
    //    for (int j = 0; j < sizes[i]; j++) {
    //        cout << nghs[i][j] << " ";
    //    }
    //    cout << endl;
    //}

    int* frontier = flatten(nghs, sizes, oldSize);

    if (newSize != oldSize) {
        cout << "Sizes do not match" << endl;
        cout << oldSize << " " << newSize << endl;
        exit(0);
    }

    int* newf = convert(oldSize, NULL, NULL, frontier, oldSize, "dense");

    for (int i = 0; i < oldSize; i++) cout << newf[i] << " ";
    cout << endl;

    for (int i = 0; i < newSize; i++) {
        cout << frontier[i] << " ";
    }
    cout << endl;

	return 0;
}
