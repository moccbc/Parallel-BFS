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
#include "../filter.h"
using namespace std;

int main(int argc, char** argv) {
    int offset[] = {0, 3, 5, 6, 8};
    int E[] = {1, 2, 3, 0, 2, 3, 1, 4, 3, 1, 2, 0};
    int flag[] = {1, 0, 1};
    int curr = 0;

    int size;
    int* out = filter(offset, E, flag, curr, size);
    for (int i = 0; i < size; i++) {
        cout << out[i] << " ";
    }
    cout << endl;

	return 0;
}
