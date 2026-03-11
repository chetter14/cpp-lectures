#include <iostream>
#include <iterator>
#include <type_traits>
#include <vector>
#include <list>
#include <string>
#include <concepts>
#include <ranges>

namespace {
    template<typename It>
    struct is_input_iterator : std::is_base_of<
        std::input_iterator_tag,
        typename std::iterator_traits<It>::iterator_category> {};

    template<typename It>
    struct is_random_iterator : std::is_base_of<
        std::random_access_iterator_tag,
        typename std::iterator_traits<It>::iterator_category> {};

    template<typename Iter>
        requires is_input_iterator<Iter>::value
    int my_distance(Iter first, Iter last) { ; }

    template<typename Iter>
        requires is_random_iterator<Iter>::value
    int my_distance(Iter first, Iter last) { ; }

    template<typename T, typename U>
        requires requires(T t, U u) { t == u; }
    bool check_eq(T&& lhs, U&& rhs) { return (lhs == rhs); }

    template<typename T>
    constexpr int somepred() { return 14; }

    template<typename T>
        requires (somepred<T>() == 42)
    void foo(T val) { ; }

    template<typename T>
        requires requires(T t) { somepred<T>() == 42; }
    void bar(T val) { ; }

    template<typename T>
    concept A = requires(T a, T b) { a + b; };

    template<typename T>
    concept B = requires() { typename T::inner; };

    template<typename T>
    concept C = requires(T t) { 
        { ++t } noexcept;
    };

    template<typename From, typename To>
    concept convertible_to = 
        std::is_convertible_v<From, To> &&
        requires(From (&f)()) { static_cast<To>(f()); };

    template<typename T>
    concept D = requires(T x) { 
        // { *x };
        // requires std::convertible_to<decltype(*x), typename T::inner>;

        /* Or (syntax sugar): */
        { *x } -> std::convertible_to<typename T::inner>;
    };

    template<std::input_iterator Iter>
    void my_distance_2(Iter begin, Iter end) { 
        std::cout << "Called with input iterator" << std::endl;
    }

    template<std::random_access_iterator Iter>
    void my_distance_2(Iter begin, Iter end) { 
        std::cout << "Called with random access iterator" << std::endl;
    }

    template<typename T>
    concept Range = requires (T&& t) { 
        std::ranges::begin(t);
        std::ranges::end(t);
    };

    template<typename T> 
    using iterator_t = decltype(std::ranges::begin(std::declval<T&>()));

    template<typename T> 
    using sentinel_t = decltype(std::ranges::end(std::declval<T&>()));
}

int main() {
    {
        std::vector<int> v { 2, 3, 4, 5 };
        
        /* Compile error ! Call to 'my_distance' is ambiguous */
        // my_distance(v.begin(), v.end());
    }

    {
        check_eq(0, 1);
        check_eq(4.2f, 0.1f);
        /* Compile error! "invalid operands to binary expression" */
        // check_eq(std::string{"hello"}, 1);
    }
    {
        /* Compile error! 'somepred<int>() == 42' (14 == 42) evaluated to false */
        // foo(2);

        bar(2);
    }
    {
        std::vector<int> v { 2, 3, 4, 5 };
        my_distance_2(v.begin(), v.end());

        std::list<int> l { 1, 2, 3 };
        my_distance_2(l.begin(), l.end());
    }
    {
        std::vector<int> v { 8, 10, 3, 11, 2, 17, 1, 9 };

        auto new_v = v | 
            std::ranges::views::filter([](auto v) { return v < 10; }) |
            std::ranges::views::transform([](auto v) { return (-v); });

        std::ranges::copy(new_v, std::ostream_iterator<int> { std::cout, " " });
        std::cout << std::endl;
    }
    std::cout << "Passed!" << std::endl;
    return 0;
}