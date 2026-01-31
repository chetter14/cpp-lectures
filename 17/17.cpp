#include <iostream>
#include <iterator>
#include <utility>

namespace {
    template<typename T> class container {
    public:
        // No deduction hint (C++17) here
        template<typename Iter> container(Iter begin, Iter end) {}
    };

    // Deduction hint (C++17) after class declaration 
    template<typename Iter> container(Iter begin, Iter end)
      -> container<typename std::iterator_traits<Iter>::value_type>;

    template<typename T> void foo(T&& val) { }

    template<typename T> class BufferA {
    public:
        void emplace(T&& val) { }
    };

    template<typename T> class BufferB {
    public:
        template<typename U> void emplace(U&& val) { }
    };

    template<typename Fun, typename Arg> 
    decltype(auto) transparent(Fun fun, Arg&& arg) {
        return fun(std::forward<Arg>(arg));
    }

    int square(int val) { return val * val; }
    int increment(int val) { return val + 1; }
}

int main() {
    {
        std::vector v{2 ,3, 4};
        container c(v.begin(), v.end());
    }

    {
        int x = 1;
        auto a = x; // int
        auto &b = x;    // int &
        auto &&c = x;   // int &

        auto d = x + 1; // int 
        // auto &e = x + 1; // compile error! can't bind rvalue to int & 
        auto &&f = x + 1;   // int &&

        const int &y = 2;
        int &z = x;
        auto w = y; // int
        auto v = z; // int 
    }

    { 
        int x = 1;
        const auto a = x;   // const int
        const auto &b = x;  // const int &
        // const auto &&c = x; // can't bind lvalue to const int &&

        const auto d = x + 1;   // const int
        const auto &e = x + 1;  // const int & ; it's an exception 
        const auto &&f = x + 1; // const int &&
    }

    {
        int x = 1;
        decltype(x) a = x;  // int
        decltype(x) b = x + 1;  // int 

        decltype(x) & c = x; // int &
        // decltype(x) & d = x + 1; // can't bind rvalue to int &

        // decltype(x) && e = x;    // can't bind lvalue to int &&
        decltype(x) && f = x + 1;    // int &&
    }

    {
        int x = 1;

        decltype((x)) a = x;    // int &
        // decltype((x)) b = x + 1;    // can't bind rvalue to int & 

        decltype((x)) & c = x;  // int &
        // decltype((x)) & d = x + 1;  // can't bind rvalue to int &

        decltype((x)) && e = x; // int &
        // decltype((x)) && f = x + 1; // can't bind rvalue to int &
    }

    {
        int x = 1;

        decltype(x + 1) a = x;  // int
        decltype(x + 1) b = x + 1;  // int

        decltype(x + 1) & c = x;    // int &
        // decltype(x + 1) & d = x + 1;    // can't bind rvalue to int &

        // decltype(x + 1) && e = x;   // can't bind lvalue to int &&
        decltype(x + 1) && f = x + 1;   // int &&
    }

    {
        int x = 1;

        // decltype(std::move(x)) a = x;   // can't bind lvalue to int &&
        decltype(std::move(x)) b = x + 1;   // int &&

        decltype(std::move(x)) & c = x; // int &
        // decltype(std::move(x)) & d = x + 1; // can't bind rvalue to int &

        // decltype(std::move(x)) && e = x;    // can't bind lvalue to int &&
        decltype(std::move(x)) && f = x + 1;    // int &&
    }

    {
        int x = 1;
        int &y = x;
        const int a = 2;

        foo(x); // foo<int &>(int &val)
        foo(y); // foo<int &>(int &val)
        foo(a); // foo<const int &>(const int &val)
        foo(x + 1); // foo<int>(int &&val)
        foo(0); // foo<int>(int &&val)
        foo(std::move(x));  // foo<int>(int &&val)
        foo(std::move(a));  // foo<int>(int &&val)
    }

    {
        int x = 1;
        int &y = x;
        const int z = 2;
        const int &w = z;

        BufferA<int> a;
        // a.emplace(x);   // can't bind lvalue to int &&
        a.emplace(std::move(x));
        a.emplace(x + 1);

        BufferB<int> b;
        b.emplace(x);   // emplace<int &>(int &val)
        b.emplace(std::move(x));    // emplace<int>(int &&val)
        b.emplace(x + 1);   // emplace<int>(int &&val)
    }

    {
        int x = transparent(&square, 2);
        std::cout << x << std::endl;

        int y = transparent(&increment, 10);
        std::cout << y << std::endl;
    }

    std::cout << "Passed!" << std::endl;

    return 0;
}