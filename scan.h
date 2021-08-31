using namespace std;
#define THRESHOLD 10000

template<typename T>
void ip_scan(T* In, int n) {
    if (n == 0) return;
    if (n <= THRESHOLD) {
        for (int i = 1; i < n; i++) In[i] += In[i-1];
        return;
    }

    size_t blocks = ceil(sqrt(n));
    size_t blockSize = n / blocks;
    if (n % blocks != 0) blockSize++;

    T* offset = new T[blocks];
    offset[0] = 0;

    cilk_for (size_t i = 1; i < blocks; i++) {
        offset[i] = 0;
        for (size_t j = (i-1)*blockSize; j < i*blockSize; j++) {
            if (j >= n) break;
            offset[i] += In[j];
        }
    }

    for (size_t i = 1; i < blocks; i++) offset[i] += offset[i-1];

    cilk_for (size_t i = 0; i < blocks; i++) {
        for (size_t j = 0; j < blockSize; j++) {
            size_t index = j+blockSize*i;
            if (index < n) {
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