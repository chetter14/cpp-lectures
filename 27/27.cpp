#include <climits>
#include <initializer_list>
#include <ios>
#include <iostream>
#include <cassert>
#include <array>
#include <type_traits>
#include <utility>

namespace {
    void print_delim() {
        std::cout << "----------" << std::endl;
    }

    constexpr std::size_t int_log(std::size_t N) {
        std::size_t pos = sizeof(std::size_t) * CHAR_BIT, mask = 0;

        if (N == 0) throw "N == 0 is invalid";

        do {
            pos -= 1;
            mask = 1ull << pos;
        } while ((N & mask) != mask);
        
        if (N != mask) pos += 1;

        return pos;
    }

    consteval int ctsqr(int n) { return n * n; }

    constinit int GL_VAR = 10;

    template<typename T>
    consteval std::size_t ilist_sz(std::initializer_list<T> il) {
        std::size_t sz = il.size();
        sz += 1;
        sz -= 1;
        return sz;
    }

    struct Complex {
        constexpr explicit Complex(double r, double i) : r_(r), i_(i) {}
        constexpr explicit Complex(double r) : Complex(r, 0.0) {}
        constexpr explicit Complex() : Complex(0.0, 0.0) {}
        constexpr double real() const { return r_; }
        constexpr double imag() const { return i_; }

        constexpr Complex operator+=(const Complex& rhs) {
            r_ += rhs.r_; i_ += rhs.i_; return *this;
        }

    private:
        double r_, i_;
    };

    constexpr Complex operator+(const Complex& lhs, const Complex& rhs) {
        Complex res = lhs;
        res += rhs;
        return res;
    }

    constexpr Complex operator""_i(long double arg) {
        return Complex(0.0, arg);
    }

    /* For compile-time binary to ull conversion: */
    /*  (kinda difficult and overwhelming) */
    template<unsigned long long Sum, char... Chars> struct binparser;
    
    template<unsigned long long Sum, char... Rest>
    struct binparser<Sum, '0', Rest...> {
        static constexpr unsigned long long value = binparser<Sum * 2, Rest...>::value;
    };

    template<unsigned long long Sum, char... Rest>
    struct binparser<Sum, '1', Rest...> {
        static constexpr unsigned long long value = binparser<Sum * 2 + 1, Rest...>::value;
    };

    template<unsigned long long Sum>
    struct binparser<Sum> {
        static constexpr unsigned long long value = Sum;
    };

    // template<char... Chars>
    // constexpr unsigned long long operator""_binary() {
    //     return binparser<0, Chars...>::value;
    // }

    /* Better approach: */
    template<char... Chars>
    constexpr unsigned long long operator""_binary() {
        std::array<char, sizeof...(Chars)> arr{ Chars... };
        unsigned long long res = 0UL;

        std::for_each(arr.begin(), arr.end(), [&] (char ch) {
            switch(ch) {
                case '0': res = res * 2; break;
                case '1': res = res * 2 + 1; break;
                default: throw "Unknown symbol";
            };
        });

        return res;
    }

    template<typename T, T... Ints>
    class integer_sequence;

    template<std::size_t... Ints>
    using index_sequence = integer_sequence<std::size_t, Ints...>;

    template<typename T, std::size_t N, std::size_t... Ns>
    constexpr std::array<T, N> 
    make_array_impl(std::initializer_list<T> il, std::index_sequence<Ns...>) {
        return std::array<T, N>{ *(il.begin() + Ns)... };
    }

    template<typename T, std::size_t N>
    constexpr std::array<T, N> make_array(std::initializer_list<T> il) {
        return make_array_impl<T, N>(il, std::make_index_sequence<N>());
    }

    template<typename T, typename U, typename = void>
    struct is_equality_comparable : std::false_type {};

    template<typename T, typename U>
    struct is_equality_comparable<T, U,
        std::void_t<decltype(std::declval<T>() == std::declval<U>())>>
        : std::true_type {};

    template<typename T, typename U>
    constexpr bool is_equality_comparable_v = is_equality_comparable<T, U>::value;

    // template<typename T, typename U,
    //     typename = std::enable_if_t<is_equality_comparable_v<T, U>>>
    // bool check_eq(T &&lhs, U &&rhs) { return lhs == rhs; }

    template<typename T, typename U>
        requires is_equality_comparable_v<T, U>
    bool check_eq(T &&lhs, U &&rhs) { return lhs == rhs; }
    
    // Compile error below because of redefinition of default template parameter in the 2nd function !
    
    // template<typename T, typename = std::enable_if_t<(sizeof(T) > 4), int>>
    // void foo(T x) { /* Some logic here */ ; }

    // template<typename T, typename = std::enable_if_t<(sizeof(T) <= 4), int>>
    // void foo(T x) { /* Another logic here */ ; }

    // In C++20 compiles fine:

    template<typename T>
        requires (sizeof(T) > 4)
    void foo(T x) { std::cout << "It's larger than 4 bytes" << std::endl; }

    template<typename T>
        requires (sizeof(T) <= 4)
    void foo(T x) { std::cout << "It's equal to or less than 4 bytes" << std::endl; }
}

int main() {
    {
        constexpr auto x = int_log(16);
        static_assert(x == 4);

        // constexpr auto y = int_log(0);   // compile error at 'throw ...'
        
        std::cout << "Enter a number: ";
        int y;
        std::cin >> y;
        
        auto z = int_log(y);
        std::cout << "Integer log of your number - " << z << std::endl;
    }
    print_delim();
    {
        constexpr auto x = ctsqr(2);
        static_assert(x == 4);
        std::cout << x << std::endl;

        int y = 2;
        // int sqr_y = ctsqr(y);   // compile error! 'consteval' has to be evaluated at compile-time
        
        // constinit auto z = 3;   // compile error! local variable 

        assert(GL_VAR == 10);
        std::cout << GL_VAR << std::endl;
    }
    print_delim();
    {
        constexpr auto n = ilist_sz({ 1, 2, 3 });
        std::cout << n << std::endl;
    }
    print_delim();
    {
        constexpr Complex a{ 1.0, 2.0 };
        static_assert(a.real() == 1.0 && a.imag() == 2.0);

        constexpr Complex b{ 3.0, 3.0 };
        static_assert(b.real() == 3.0 && b.imag() == 3.0);

        constexpr auto c = a + b;
        static_assert(c.real() == 4.0 && c.imag() == 5.0);

        constexpr Complex d = 1.0_i;
        static_assert(d.real() == 0.0 && d.imag() == 1.0);

        std::cout << "Created objects (literals) and did calculations at compile time"
            << std::endl;
    }
    print_delim();
    {
        constexpr auto x = 111_binary;
        static_assert(x == 7);

        std::cout << "binary to ull conversion occurred at compile time!" << std::endl;
    }
    print_delim();
    {
        constexpr std::array<int, 5> arr { 1, 2, 3, 4, 5 };
        static_assert(arr[1] == 2 && arr[4] == 5);

        std::cout << "array init occurred at compile time!" << std::endl;
    }
    print_delim();
    {
        std::cout << std::boolalpha;
        std::cout << is_equality_comparable_v<std::string, int> << std::endl;
        std::cout << is_equality_comparable_v<std::string, std::string> << std::endl;
        
        // std::cout << check_eq(std::string("hey"), 2) << std::endl;  // compile error!
        std::cout << check_eq(2.0, 5.0) << std::endl;
    }
    print_delim();
    {
        foo(3);
        foo(8.2);
        foo(2.3f);
        foo(std::string("ehy"));
    }
    std::cout << "Passed!" << std::endl;
    return 0;
}