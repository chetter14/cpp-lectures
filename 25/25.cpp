#include <iostream>
#include <cassert>
#include <functional>
#include <iterator>
#include <vector>
#include <algorithm>
#include <numeric>

namespace {
    void print_delim() {
        std::cout << "----------" << std::endl;
    }

    struct Lam {
        int x;
        void foo() {
            // [x]() mutable { x += 3; }();    // compile error !
            // [&x]() { x += 3; }();           // compile error !

            /* Below 'this' pointer is captured, and so 'x' variable is changed in each function */
            [=] { x += 3; std::cout << "x = " << x << std::endl; }();
            [&] { x += 3; std::cout << "x = " << x << std::endl; }();

            [this] { x += 3; std::cout << "x = " << x << std::endl; }();
        }
    };

    auto factory(int x) {
        static int a = 0;
        return [=] (int y) {
            static int b = 0;
            a += x; b += y;
            return a + b;
        };
    }

    template<typename ActTy> struct Finally {
        ActTy act_;
        explicit Finally(ActTy act) : act_(std::move(act)) {}
        ~Finally() { act_(); }
    };
}

int main() {
    {
        Lam obj;
        obj.x = 1;
        obj.foo();
        assert(obj.x == 10);
    }
    print_delim();
    {
        auto func1 = factory(1);

        /* Before: a = 0, b = 0 */
        int x = func1(10);
        /* After: a = 1, b = 10 */
        std::cout << x << std::endl;
        assert(x == 11);
        
        /* Before: a = 1, b = 10 */
        x = func1(30);
        /* After: a = 2, b = 40 */
        std::cout << x << std::endl;
        assert(x == 42);
    }
    print_delim();
    {
        int x = 1, y = 2;

        auto t = [&x, &y] { return x + y; };
        std::function<int()> f = [&x, &y] { return x + y; };
        
        std::cout << t() << " " << f() << std::endl;
        assert(t() == 3 && f() == 3);

        // t = [] { return 42; };   // compile error!
        f = [] { return 42; };

        std::cout << f() << std::endl;
        assert(f() == 42);
    }
    print_delim();
    {
        int x = 1;
        Finally on_close([&x] { std::cout << "In the end, x = " << x << std::endl; });
        
        /* Do some work with x */
        x = 10;
        x *= 2;

        /* Here it'd be printed that x = 20 */
    }
    print_delim();
    {
        int arr[] = { 1, 5, 9, 2, 6, 14, 11 };
        std::vector<int> v(7);

        std::ostream_iterator<int> osit(std::cout, " ");

        std::copy_n(arr, 7, v.begin());
        std::copy(v.begin(), v.end(), osit);
        std::cout << std::endl;

        std::fill(v.begin(), v.end(), 0);

        std::copy_if(arr, arr + 7, v.begin(), [](int elem) { return elem % 2 == 0; });
        std::copy(v.begin(), v.end(), osit);
        std::cout << std::endl;
    }
    print_delim();
    {
        std::vector<int> v { 5, 8, 2, 19, 14 }; 
        std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
        std::cout << std::endl;

        if (std::find_if(v.begin(), v.end(), [](int elem) { return elem % 7 == 0; }) != v.end()) {
            std::cout << "Contains '% 7 == 0' value(s)" << std::endl;
        }

        if (std::any_of(v.begin(), v.end(), [](int elem) { return elem % 5 == 0; })) {
            std::cout << "Contains '% 5 == 0' value(s)" << std::endl;
        } 
    }
    print_delim();
    {
        std::vector<int> a(6), b(6);
        std::fill(a.begin(), a.end(), 1);
        std::fill(b.begin(), b.end() - 1, 1);
        b.at(5) = 2;

        auto [ita, itb] = std::mismatch(a.begin(), a.end(), b.begin());
        if (ita != a.end() && itb != b.end()) {
            std::cout << "Mismatch" << std::endl;
        }

        /* Or equivalent: */
        if (!std::equal(a.begin(), a.end(), b.begin())) {
            std::cout << "Not equal" << std::endl;
        }
    }
    print_delim();
    {
        std::vector<int> v(10);
        std::iota(v.begin(), v.end(), 1);
        std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
        std::cout << std::endl;

        auto fst = std::find(v.begin(), v.end(), 1);
        auto lst = std::next(fst, 5);
        auto pos = std::find(v.begin(), v.end(), 4);
        auto dpos = std::next(pos, 5);
        std::copy_backward(fst, lst, dpos);

        std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
        std::cout << std::endl;
    }
    print_delim();
    {
        std::ostream_iterator<int> osit(std::cout, " ");

        std::vector<int> v(7);
        std::iota(v.begin(), v.end(), 1);
        std::copy(v.begin(), v.end(), osit);
        std::cout << std::endl;

        std::transform(v.begin(), v.end(), v.begin(), [](auto &&elem) { return -elem; });
        std::copy(v.begin(), v.end(), osit);
        std::cout << std::endl;
    }
    print_delim();

    std::cout << "Passed!" << std::endl;
    return 0;
}