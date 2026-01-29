#include <iostream>
#include <string>

namespace {
    template<typename T> struct MyStruct {
    public:
        explicit MyStruct(T&& x) : x_(x) { }
        void foo() const { 
            std::cout << "Not int type " << x_ << std::endl; 
        }
    private:
        T x_;
    };

    template<> struct MyStruct<int> {
    public:
        explicit MyStruct(int x) : x_(x) { }
        void foo() const { 
            std::cout << "It's int and value is " << x_ << std::endl; 
        }
    private:
        int x_;
    };

    enum class TypeName {
        Integer = 0,
        Double,
        String,
        Char,
        Other
    };

    template<TypeName tm> struct TypeStruct {
        // Empty by default
    };

    // Specialization for int
    template<> struct TypeStruct<TypeName::Integer> {
    public:
        void print() const { std::cout << "Integer" << std::endl; }
    };

    // Specialization for double
    template<> struct TypeStruct<TypeName::Double> {
    public:
        void print() const { std::cout << "Double" << std::endl; }
    };

    // Specialization for string
    template<> struct TypeStruct<TypeName::String> {
    public:
        void printString() const { std::cout << "String" << std::endl; }
    };

    template<typename T> T max(T x, T y) { return x > y ? x : y; }

    // Instantiate here for int type
    template int max(int a, int b);

    // In case, to block instantiation and say that it's done somewhere else:
    // extern template int max(int a, int b);

    // Specialization for double type
    template<> double max(double x, double y) { return 1.23; }

    // Restrict instantion of max() for char type
    template<> char max(char x, char y) = delete;

    template<typename T, typename U> struct ValuesPair {
    public:
        ValuesPair(T x, U y) : x_(x), y_(y) {}
        void foo() const { std::cout << x_ << " " << y_ << std::endl; } 
    private:
        T x_;
        U y_;
    };

    // Partial specialization
    template<typename T> struct ValuesPair<T, T> {
    public:
        ValuesPair(T x, T y) : x_(x), y_(y) {}
        void fooSameType() const { 
            std::cout << "One type " << x_ << " " << y_ << std::endl; 
        } 
    private:
        T x_, y_;
    };
}

int main() {
    {
        MyStruct a(2);
        a.foo();

        MyStruct b(4.23);
        b.foo();

        MyStruct c(std::string("text"));
        c.foo();

        std::string str{"Hello world!"};
        // Compile error:
        // MyStruct d(str);
        MyStruct d(std::move(str));
        d.foo();
    }

    {
        TypeStruct<TypeName::Integer> ts1;
        ts1.print();
        TypeStruct<TypeName::Double> ts2;
        ts2.print();
        TypeStruct<TypeName::String> ts3;
        ts3.printString();

        TypeStruct<TypeName::Char> ts4;
        // Compile error:
        // ts4.print();

        TypeStruct<TypeName::Other> ts5;
        // Compile error:
        // ts5.print();
    }

    {
        double a = 5.2, b = 8.03;
        // Output will be - 1.23 (because of specialization)
        std::cout << max(a, b) << std::endl;
    }
    {
        int a = 4, b = 10;
        std::cout << max(a, b) << std::endl;
    }
    {
        char a = 'A', b = 'b';
        // Compile error:
        // std::cout << max(a, b) << std::endl;
    }

    {
        ValuesPair a{1, 3.87};
        a.foo();

        ValuesPair b{1.23f, 8.34f};
        b.fooSameType();
    }

    return 0;
}
