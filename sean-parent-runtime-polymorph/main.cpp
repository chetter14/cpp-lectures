#include <iostream>
#include <string>
#include <cassert>
#include "document.h"

namespace {
    struct MyValue {
    public:
        MyValue(double x, double y) : x_(x), y_(y) { 
            std::cout << "Constructor" << std::endl; 
        }
        MyValue(const MyValue& val) {
            std::cout << "Copy constructor" << std::endl;
            x_ = val.x_;
            y_ = val.y_;
        }
        MyValue(MyValue&& val) {
            std::cout << "Move constructor" << std::endl;
            x_ = val.x_;
            y_ = val.y_;
            val.x_ = 0.0;
            val.y_ = 0.0;
        }

        MyValue& operator=(const MyValue& val) { 
            std::cout << "Copy assignment" << std::endl;
            x_ = val.x_;
            y_ = val.y_;
            return *this;
        }

        MyValue& operator=(MyValue&& val) { 
            std::cout << "Move assignment" << std::endl;
            x_ = val.x_;
            y_ = val.y_;
            val.x_ = 0.0;
            val.y_ = 0.0;
            return *this;
        }

        double getX() const noexcept { return x_; }
        double getY() const noexcept { return y_; }

    private:
        double x_, y_;
    };
}

template<>
void draw<MyValue>(const MyValue& val, std::ostream& os) {
    os << val.getX() << " " << val.getY() << std::endl;
}

int main() {
    Doc::Document mydoc;

    // mydoc.emplace_back(1);
    // mydoc.emplace_back(3.14);
    // mydoc.emplace_back(std::string("sample text"));

    // int a = 2;
    // double b = 9.81;
    // std::string c = "text #2";

    // mydoc.emplace_back(a);
    // mydoc.emplace_back(b);
    // mydoc.emplace_back(std::move(c));

    // Doc::draw(mydoc, std::cout);

    {
        MyValue a(7.3, 15.8);
        mydoc.emplace_back(a);

        draw(mydoc, std::cout);

        assert(a.getX() == 7.3 && a.getY() == 15.8);
    }

    return 0;
}
