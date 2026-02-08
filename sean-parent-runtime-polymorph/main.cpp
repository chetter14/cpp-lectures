#include <iostream>
#include <string>
#include "document.h"

int main() {
    Doc::Document mydoc;

    mydoc.emplace_back(1);
    mydoc.emplace_back(3.14);
    mydoc.emplace_back(std::string("sample text"));

    int a = 2;
    double b = 9.81;
    std::string c = "text #2";

    mydoc.emplace_back(a);
    mydoc.emplace_back(b);
    mydoc.emplace_back(std::move(c));

    Doc::draw(mydoc, std::cout);

    return 0;
}
