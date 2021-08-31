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
using namespace std;

int main(int argc, char** argv) {
    srand(time(NULL));
    int n = atoi(argv[1]);
    int* arr = new int[n];
    cilk_for(int i = 0; i < n; i++) arr[i] = rand() % 2;

    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    ip_scan(arr, n);

    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

	return 0;
}
