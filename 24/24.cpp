#include <algorithm>
#include <execution>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <set>

namespace {
    template<typename T, typename U>
    void snap_hashtable(const std::unordered_map<T, U>& ht) {
        std::cout << ht.size() << " " << ht.bucket_count() << std::endl;
    }

    void print_delim() {
        std::cout << "----------" << std::endl;
    }

    struct Multiplier {
        int m_;
        Multiplier(int m) : m_(m) { }
        /* 'auto' is allowed for a function parameter in C++20 */
        void operator()(auto&& val) { val = val * m_; }
    };
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
        std::cout << "After large emplace without pre-reserve:\n";
        snap_hashtable(ht);
    }
    print_delim();
    {
        std::unordered_map<int, int> ht;
        ht.max_load_factor(2);
        for (int i = 0; i < 30; ++i) {
            ht.emplace(i, -i);
        }

        for (auto it = ht.begin(), ite = ht.end(); it != ite; ++it) {
            std::cout << "Pair: " << it->first << " " << it->second << std::endl;
        }

        print_delim();

        std::cout << "Iterate by buckets" << std::endl;
        for (int i = 0; i < ht.bucket_count(); ++i) {
            std::cout << "Bucket #" << i << ": " << std::endl;
            for (auto it = ht.begin(i), ite = ht.end(i); it != ite; ++it) {
                std::cout << "\tPair: " << it->first << " " << it->second << std::endl;
            }
        }
        snap_hashtable(ht);
    }
    print_delim();
    {
        std::unordered_map<int, int> mp { {1, -1}, {2, -2}};
        auto &x = mp[100];
        std::cout << x << std::endl;

        std::for_each(mp.begin(), mp.end(), [&](const decltype(mp)::value_type& pair){
            std::cout << "Pair: " << pair.first << " " << pair.second << std::endl;
        });
    }
    print_delim();
    {
        std::set<int> s { 11, 2, 21, 8, 42, 5, 0 };
        auto itb = s.lower_bound(5);
        auto ite = s.upper_bound(30);

        for (auto it = itb; it != ite; ++it) 
            std::cout << *it << std::endl;
    }
    print_delim();
    {
        std::set<int, std::greater<int>> s { 11, 2, 21, 8, 42, 5, 0 }; 
        auto itb = s.lower_bound(30);
        auto ite = s.upper_bound(5);

        for (auto it = itb; it != ite; ++it) 
            std::cout << *it << std::endl;
    }
    print_delim();
    {
        std::set<int, std::less_equal<int>> s { 11, 2, 21, 8, 42, 5, 0, 11 }; 
        auto itb = s.lower_bound(5);
        auto ite = s.upper_bound(30);

        /* Probably, 11 is going to be printed out twice, even though it's in std::set */
        /* This happens because of 'less_equal' */
        for (auto it = itb; it != ite; ++it) 
            std::cout << *it << std::endl;
    }
    print_delim();
    {
        std::set<int> s { 11, 2, 21, 8, 42, 5, 0 }; 
        constexpr auto min = 10, max = 30;

        for (auto it = s.begin(), ite = s.end(); it != ite;) {
            if (*it < max && *it > min)
                /* 'it++' to keep iterator valid */
                s.erase(it++);
            else 
                ++it;
        }

        std::for_each(s.begin(), s.end(), [&](auto &&val) {
            std::cout << val << std::endl;
        });
    }
    print_delim();
    {
        std::vector<int> v{ 4, 0, 2, 8 };

        std::for_each(std::execution::par, v.begin(), v.end(), Multiplier(3));

        std::for_each(v.begin(), v.end(), [&](auto &&val) {
            std::cout << val << std::endl;
        });
    }
    
    /* It's legal =) */
    ([](){})();

    print_delim();
    {
        int a = 1, b = 2;

        auto fval = [a, b](int x) { return a + b * x; };
        auto fvalm = [a, b](int x) mutable { a += b * x; return a; };
        auto fref = [&a, &b](int x) { a += b * x; return a; };

        std::cout << fval(1) << std::endl;
        std::cout << a << " " << b << std::endl;
        std::cout << fvalm(1) << std::endl;
        std::cout << a << " " << b << std::endl;
        std::cout << fref(1) << std::endl;
        std::cout << a << " " << b << std::endl;

        print_delim();
        a = 10;

        std::cout << fval(1) << std::endl;
        std::cout << a << " " << b << std::endl;
        std::cout << fvalm(1) << std::endl;
        std::cout << a << " " << b << std::endl;
        std::cout << fref(1) << std::endl;
        std::cout << a << " " << b << std::endl;
    }
    print_delim();
    {
        int x = 10;
        auto lam = [&arg = x]() { arg *= 2; };

        lam();
        std::cout << x << std::endl;
    }

    std::cout << "Passed!" << std::endl;
    return 0;
}