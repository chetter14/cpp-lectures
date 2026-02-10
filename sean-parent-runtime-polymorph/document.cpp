#include "document.h"

template<>
void draw<Doc::Object>(const Doc::Object& obj, std::ostream& os) {
    obj.dump(os);
}

template<>
void draw<Doc::Document>(const Doc::Document& doc, std::ostream& os) {
    os << "<document>" << std::endl;
    for (const auto &val : doc) draw(val, os);
    os << "</document>" << std::endl;
}