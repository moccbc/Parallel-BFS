using namespace std;
#define THRESHOLD 10000

/*
    Scan computes the prefix sum of the given array. 
    It does so by first squareroot decomposing the array,
    computing the sum of the blocks, then uses those blocks
    to compute the prefix sum of the array.
*/
template<typename T>
void ip_scan(T* In, int n) {
    if (n == 0) return;

    // If the size of the array is below THRESHOLD run it sequentially
    // This granularity control improves performance by reducing the 
    // overhead of spawning new threads for small enough frontiers.
    if (n <= THRESHOLD) {
        for (int i = 1; i < n; i++) In[i] += In[i-1];
        return;
    }

    // Apply the squareroot decomposition technique
    size_t blocks = ceil(sqrt(n));
    size_t blockSize = n / blocks;
    if (n % blocks != 0) blockSize++;

    // Offset will hold the total sums of the elements in each block
    T* offset = new T[blocks];
    offset[0] = 0;

    // Compute the sum of the individual blocks
    cilk_for (size_t i = 1; i < blocks; i++) {
        offset[i] = 0;
        for (size_t j = (i-1)*blockSize; j < i*blockSize; j++) { // for each element in the block
            // This check is needed for when the last block is not of size blockSize
            if (j >= n) break; 
            offset[i] += In[j];
        }
    }

    // Compute the prefix sum of the squareroot decomposed array
    for (size_t i = 1; i < blocks; i++) offset[i] += offset[i-1];

    // Compute the prefix sum of the entire array
    // For each block in parallel
    cilk_for (size_t i = 0; i < blocks; i++) {
        // For each element in the original array that resides in block i
        for (size_t j = 0; j < blockSize; j++) {
            size_t index = j+blockSize*i;
            if (index < n) {
                // If it is the first element in the block, then the sum
                // up to it was already calculated when we computed the
                // prefix sum in offset
                if (j == 0) {
                    In[index] += offset[i];
                }
                else {
                    In[index] += In[index-1];
                }
            }
        }
    }
    delete[] offset;
}