#include <iostream>
#include <deque>
#include <forward_list>
#include <stack>
#include <vector>
#include <bitset>
#include <string>
#include <cassert>
#include <string_view>
#include <unordered_map>
#include <iterator>
#include <numeric>
#include <algorithm>

namespace {
    std::string_view data = "Some data is here";

    void print_sv(std::string_view sv) {
        std::cout << sv << std::endl;
    }

    struct Key {
        std::string name;
        std::string surname;
        auto operator<=>(const Key&) const = default;
    };
}

/* Add specialization of std::hash for our 'Key' class */
namespace std {
    template <> struct hash<Key> {
        std::size_t operator()(const Key& k) const {
            std::hash<std::string> h;
            return h(k.name) ^ (h(k.surname) << 1);
        }
    };
}

int main() {

    {
        std::deque<int> deq;
        deq.push_back(2);
        deq.push_back(6);
        deq.push_front(4);

        for (int x : deq) 
            std::cout << x << " ";
        std::cout << std::endl;
    }

    {
        std::forward_list<int> fl1 { 1, 2, 3 };
        std::forward_list<int> fl2 { 5, 6, 7 };
        auto it = fl1.begin();

        fl1.splice_after(fl1.before_begin(), fl2);

        std::ostream_iterator<int> osit(std::cout, " ");
        std::copy(fl1.begin(), fl1.end(), osit);
        std::cout << std::endl;

        std::cout << *it << std::endl;
    }

    {
        std::stack<int> s;
        s.push(2); 
        std::cout << s.top() << std::endl;

        std::stack<int, std::forward_list<int>> s1;
        // s1.push(2);                              // compile error !
        // std::cout << s1.top() << std::endl;      // compile error !

        std::stack<int, std::vector<int>> s2;
        s2.push(2);
        std::cout << s2.top() << std::endl;
    }

    {
        std::bitset<16> s1 = 0xDEAF;
        std::bitset<16> s2 = 0xBEEF;

        std::cout << s1 << "\n" << s2 << std::endl;

        auto s = (s1 & s2);
        std::cout << s << std::endl;
        std::cout << s[0] << " " << s[4] << " " << s[20] << std::endl;
    }

    {
        std::string s = "hello";

        unsigned int x = s.find("end");
        // assert(x == std::string::npos);  // fails !
        std::cout << x << " " << std::string::npos << std::endl;

        auto y = s.find("end");
        assert(y == std::string::npos);

        std::string::size_type z = s.find("end");
        assert(z == std::string::npos);
    }

    {
        print_sv(data);
    }

    {
        std::unordered_map<Key, double> empSalary;
        empSalary.emplace(Key{ "John", "Black"}, 80.5);
        empSalary.emplace(Key{ "Bob", "Brown"}, 75.1);

        std::cout << empSalary.at(Key{ "Bob", "Brown"}) << std::endl;
    }

    {
        std::vector<int> v(100);
        std::ostream_iterator<int> osit(std::cout, " ");
        std::iota(v.begin(), v.end(), 0);
        std::sort(v.begin(), v.end(), [](auto lhs, auto rhs) { return false; });
        std::copy(v.begin(), v.end(), osit);
        std::cout << std::endl;
    }

    std::cout << "Passed!" << std::endl;
    return 0;
}