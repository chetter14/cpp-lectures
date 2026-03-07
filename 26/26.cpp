#include <iostream>
#include <type_traits>

namespace {
    void print_delim() {
        std::cout << "----------" << std::endl;
    }

    void print_all() { return; }

    template<typename T, typename... Args>
    void print_all(T head, Args... tail) {
        std::cout << head << " ";
        print_all(tail...);
    }

    template<std::size_t N> struct fact 
        : std::integral_constant<std::size_t, N * fact<N - 1>::value> {};

    template<> struct fact<0> : std::integral_constant<std::size_t, 1> {};

    template<std::size_t N> struct fibon 
        : std::integral_constant<std::size_t, fibon<N - 1>::value + fibon<N - 2>::value> {};

    template<> struct fibon<1> : std::integral_constant<std::size_t, 1> {};
    template<> struct fibon<0> : std::integral_constant<std::size_t, 0> {};

    /* Calculate an integer square root */
    int isqrt(int N, int lo, int hi) {
        int mid = (lo + hi + 1) / 2;
        if (lo == hi) 
            return lo;
        else {
            if (N < mid * mid)
                return isqrt(N, lo, mid - 1);
            else 
                return isqrt(N, mid, hi);
        }
    }

    template<bool B, typename T, typename F>
    struct conditional { using type = T; };

    template<typename T, typename F>
    struct conditional<false, T, F> { using type = F; };

    template<bool B, typename T, typename F>
    using conditional_t = typename conditional<B, T, F>::type;

    /* Calculate integer square root via metaprogramming */
    template<int N, int L = 1, int H = N, int M = (L + H + 1) / 2>
    struct IntSqrt : std::integral_constant<int, 
                        std::conditional_t<(N < M * M), 
                                            IntSqrt<N, L, M - 1>,
                                            IntSqrt<N, M, H>>{}> {};

    template<int N, int S>
    struct IntSqrt<N, S, S, S> : std::integral_constant<int, S> {};

    template<typename T> struct my_numeric_limits;

    template<> struct my_numeric_limits<char> { 
        static constexpr std::size_t max() { return 256; }
    };

    /* With SFINAE: */
    // template<typename T, std::enable_if_t<(sizeof(T) > 4), int> = 0>
    // void foo(T x) { 
    //     std::cout << "sizeof is larger than 4" << std::endl;
    // }

    // template<typename T, std::enable_if_t<(sizeof(T) <= 4), int> = 0>
    // void foo(T x) { 
    //     std::cout << "sizeof is equal or less than 4" << std::endl;
    // }

    /* Without SFINAE (but with if constexpr): */
    template<typename T>
    void foo(T x) {
        if constexpr (sizeof(T) > 4) {
            std::cout << "sizeof is larger than 4" << std::endl;
        } else {
            std::cout << "sizeof is equal or less than 4" << std::endl;
        }
    }

    template<typename Head, typename... Tail>
    void print(Head head, Tail... tail) {
        std::cout << head << " ";
        if constexpr (sizeof...(tail) > 0) {
            print(tail...);
        }
    }
}

int main() {
    {
        print_all(2, 3.14f, 2.77, "hey", 'a');
        std::cout << std::endl;
    }
    print_delim();
    {
        static_assert(24 == fact<4>::value);
        static_assert(720 == fact<6>::value);

        std::cout << fact<4>::value << std::endl;
        std::cout << fact<6>::value << std::endl;
    }
    print_delim();
    {
        static_assert(8 == fibon<6>::value);
        static_assert(55 == fibon<10>::value);

        std::cout << fibon<6>::value << std::endl;
        std::cout << fibon<10>::value << std::endl;
    }
    print_delim();
    {
        std::cout << isqrt(4, 1, 4) << std::endl;
        std::cout << isqrt(14, 1, 14) << std::endl;
        std::cout << isqrt(36, 1, 36) << std::endl;
        std::cout << isqrt(30, 1, 30) << std::endl;
    }
    print_delim();
    {
        static_assert(2 == IntSqrt<4>::value);
        static_assert(3 == IntSqrt<14>::value);
        static_assert(6 == IntSqrt<36>::value);
        static_assert(5 == IntSqrt<30>::value);

        std::cout << IntSqrt<4>::value << std::endl;
        std::cout << IntSqrt<14>::value << std::endl;
        std::cout << IntSqrt<36>::value << std::endl;
        std::cout << IntSqrt<30>::value << std::endl;
    }
    print_delim();
    {
        constexpr std::size_t sz = my_numeric_limits<char>::max();
        int arr[sz] = { 1, 2, 3 };
        std::cout << arr[0] << " " << arr[1] << " " << arr[2] << std::endl;
    }
    print_delim();
    {
        static constexpr int arr[] = { 5, 2, 8, 0, 1, 3 };
        constexpr const int *x = &arr[0];
        std::cout << x << " " << *x << std::endl;
    }
    print_delim();
    {
        double a = 2.3;
        short b = 10;
        foo(a);
        foo(b);
    }
    print_delim();
    {
        print(2, 3.14f, 9.81, "string", 'c');
        std::cout << std::endl;
    }
    print_delim();
    std::cout << "Passed!" << std::endl;
    return 0;
}