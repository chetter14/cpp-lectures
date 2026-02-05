#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

namespace {
    template<typename ... T> 
    void foo(T ... args) {
        /* Do something */
    }

    template<typename ... T> void h(T ... args) { /* Do nothing */ }

    template<typename ... T>
    void goo(T ... args) {
        foo(args...);                   // foo(x, y);
        foo(&args...);                  // foo(&x, &y);
        foo((args + 1)...);             // foo(x + 1, y + 1);
        foo(h(args...) + h(args)...);   // foo(h(x, y, z) + h(x), 
                                        //      h(x, y, z) + h(y), 
                                        //      h(x, y, z) + h(z));
    }

    /* Ideal redirection of function call */
    template<typename Fun, typename ... Args>
    decltype(auto) transparent(Fun&& fun, Args&& ... args) {
        return std::forward<Fun>(fun)(std::forward<Args>(args)...);
    }

    int sum(int a, int b) { return a + b; }
    double equation(double x, double y, double z) { return x * x + y * 2 - z; }

    template<typename T> struct MyStruct {
        template<typename U> void foo(U arg) { std::cout << "General method" << std::endl; }
    };

    /* Specialize templated method of templated struct */
    template<> 
    template<>
    void MyStruct<int>::foo<int>(int arg) { std::cout << "int method" << std::endl; }
}

namespace structs {
    struct foo {
        using value_type = double;
    };
    struct boo { };
}

namespace {
    template<typename, typename = void> 
    struct has_value_type : std::false_type {};

    /* If 'typename T::value_type' exists/is correct, then inherit from std::true_type */
    template<typename T>
    struct has_value_type<T, std::void_t<typename T::value_type>> 
        : std::true_type {};

    template<typename T>
    struct MyVector {
        MyVector(std::size_t size, T value) { 
            std::cout << "Size and def value init" << std::endl; 
        }

        /* 'std::declval' function returns Iter& here */
        template<typename Iter, 
                typename = std::void_t<decltype(*std::declval<Iter&>()), decltype(++std::declval<Iter&>())>>
        MyVector(Iter first, Iter last) {
            std::cout << "Iterators init" << std::endl;
        }

    private:
        std::size_t size_;
        T *arr_;
    };

    template<typename ... Args>
    double divide_right(Args ... args) { return (args / ...); }

    template<typename ... Args>
    double divide_left(Args ... args) { return (... / args); }
}

int main() {
    {
        foo(2);
        std::string str = "hello";
        foo(str);

        foo(2, 5.1, str, "hey");
        foo(1, "one", 2, "two");
    }

    {
        auto x = transparent(sum, 2, 3);
        auto y = transparent(equation, 2.0, 3.0, 1.0);

        std::cout << x << " " << y << std::endl;
    }

    {
        MyStruct<double> obj1;
        obj1.foo(1.3f);                     // General method
        obj1.foo(2);                        // General method 
        obj1.foo(std::string("hey"));   // General method 

        MyStruct<int> obj2;
        obj2.foo(2);        // int method
    }

    {
        std::cout << has_value_type<structs::foo>{} << std::endl;
        std::cout << has_value_type<structs::boo>{} << std::endl;
    }

    {
        std::vector<int> v(4, 1);

        MyVector<int> v1(2, 0);             // Size and def value init
        MyVector<int> v2(v.begin(), v.end());   // Iterators init
    }

    {
        std::cout << divide_left(5.0, 4.0, 3.0) << std::endl; 
        std::cout << divide_right(5.0, 4.0, 3.0) << std::endl; 
    }

    std::cout << "Passed!" << std::endl;
    return 0;
}
