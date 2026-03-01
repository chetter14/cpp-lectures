#include <algorithm>
#include <iostream>
#include <unordered_map>

namespace {
    template<typename T, typename U>
    void snap_hashtable(const std::unordered_map<T, U>& ht) {
        std::cout << ht.size() << " " << ht.bucket_count() << std::endl;
    }
}

int main() {

    {
        std::unordered_map<int, int> ht;
        std::cout << "Before reserve:\n";
        snap_hashtable(ht);

        ht.reserve(100);
        std::cout << "After reserve:\n";
        snap_hashtable(ht);

        for (int i = 0; i < 100; ++i) {
            ht.emplace(i, -i);
        }
        std::cout << "After emplace:\n";
        snap_hashtable(ht);

        for (int i = 100; i < 500; ++i) {
            ht.emplace(i, -i);
        }
        std::cout << "After large emplace:\n";
        snap_hashtable(ht);
    }

    std::cout << "Passed!" << std::endl;
    return 0;
}