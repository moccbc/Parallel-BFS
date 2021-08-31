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

