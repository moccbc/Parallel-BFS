template<typename ET>
inline bool CAS(ET* a, ET oldval, ET newval) {
    static_assert(sizeof(ET) <= 8, "Bad CAS length");
    if (sizeof(ET) == 1) {
        uint8_t r_oval, r_nval;
        std::memcpy(&r_oval, &oldval, sizeof(ET));
        std::memcpy(&r_nval, &newval, sizeof(ET));
        return __sync_bool_compare_and_swap(reinterpret_cast<uint8_t*>(a), r_oval, r_nval);
    } else if (sizeof(ET) == 4) {
        uint32_t r_oval, r_nval;
        std::memcpy(&r_oval, &oldval, sizeof(ET));
        std::memcpy(&r_nval, &newval, sizeof(ET));
        return __sync_bool_compare_and_swap(reinterpret_cast<uint32_t*>(a), r_oval, r_nval);
    } else { // if (sizeof(ET) == 8) {
        uint64_t r_oval, r_nval;
        std::memcpy(&r_oval, &oldval, sizeof(ET));
        std::memcpy(&r_nval, &newval, sizeof(ET));
        return __sync_bool_compare_and_swap(reinterpret_cast<uint64_t*>(a), r_oval, r_nval);
    } 
}

// Assumes that the frontier passed in and the new frontier
// are from different modes
int* convert(int n, int* frontier,
             int &fsize, string currMode) 
{
    if (currMode == "dense") {
        int* new_frontier = new int[n];
        cilk_for(int i = 0; i < n; i++) {
            new_frontier[i] = 0;
        }
        if (fsize < 1000) {
            for(int i = 0; i < fsize; i++) {
                new_frontier[frontier[i]] = 1;
            }
        }
        else {
            cilk_for(int i = 0; i < fsize; i++) {
                new_frontier[frontier[i]] = 1;
            }
        }
        return new_frontier;
    }
    else if (currMode == "sparse") {
        ip_scan(frontier, n);
        fsize = frontier[n-1];
        int* new_frontier = new int[fsize];

        if (frontier[0] == 1) new_frontier[0] = 0;
        cilk_for (int i = 0; i < n; i++) {
            if (frontier[i] != frontier[i-1])
                new_frontier[frontier[i] - 1] = i;
        }
        return new_frontier;
    }
}

