#include <initializer_list>
#include <iostream>
#include <iterator>
#include <vector>
#include <cassert>
#include <string>

namespace {
    template<typename T> class MyClass {
    public:
        MyClass(std::initializer_list<T> il) : v(il.begin(), il.end()) { }

        void foo() {
            for (auto &&x : v) {
                std::cout << x << " ";
            }
            std::cout << std::endl;
        }

    private:
        std::vector<T> v;
    };

    template<typename T> class MyArray {
    public:
        MyArray(std::size_t size, T value) 
            : size_(size), cap_(size), arr_(new T[size_]) {
            std::fill(arr_, arr_ + size_, value);
            std::cout << "Size and value constructor" << std::endl;
        }

        template<typename Iter> MyArray(Iter begin, Iter end) 
            : size_(std::distance(begin, end)), 
                cap_(size_), arr_(new T[size_]) {
            std::copy(begin, end, arr_);
            std::cout << "Two iterators constructor" << std::endl;
        }

    private:
        std::size_t size_, cap_;
        T *arr_;
    };

    template<typename Iter> MyArray(Iter begin, Iter end) 
        -> MyArray<typename std::iterator_traits<Iter>::value_type>;

    template<typename T, T v> struct integral_constant {
        static const T value = v;
        using value_type = T;
        using type = integral_constant;
        operator value_type() { return value; }
    };

    using true_type = integral_constant<bool, true>;
    using false_type = integral_constant<bool, false>;

    template<typename T, typename U> 
    struct is_same : false_type { };

    template<typename T>
    struct is_same<T, T> : true_type { };
}

int main() {
    {
        // std::vector<const int&> v;  // compile error!
    }

    {
        int n = 10;
        std::vector<bool> vec(n);
        // bool *x = &vec[0];  // compile error!

        std::vector<char> vec2(n);
        char *x = &vec2[0];
        assert(*x == vec2[0]);
    }

    {
        MyClass obj1{ 2, 3, 4, 5 };
        obj1.foo();

        MyClass obj2{ 'a', 'b', 'c', 'd' };
        obj2.foo();
    }

    {
        MyArray arr(std::size_t(4), 1);

        std::vector<double> v{ 5.1, 8.4, 0.5 };
        MyArray arr2(v.begin(), v.end());
    }

    {
        integral_constant<int, 6> ic;
        double x = ic;
        int y = ic * 10;
        std::cout << x << " " << y << std::endl;
    }

    {
        static_assert(is_same<int, int>::value);
        static_assert(!is_same<std::string, char>::value);
    }

    std::cout << "Passed" << std::endl;

    return 0;
}