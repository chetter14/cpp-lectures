#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <vector>
#include <iostream>
#include <memory>

namespace Doc {

    template<typename T>
    void draw(const T& x, std::ostream& os) {
        os << x << std::endl;
    }

    class Object {
    public:
        template<typename T>
        Object(T val) : self_(std::make_shared<DrawableObject<T>>(std::move(val))) { }

        void dump(std::ostream& os) const {
            self_->draw_(os);
        }

    private:
        struct Drawable {
            virtual ~Drawable() = default;
            virtual void draw_(std::ostream& os) const = 0;
        };

        template<typename T>
        struct DrawableObject final : public Drawable { 
        public:
            DrawableObject(T data) : data_(std::move(data)) { }

            void draw_(std::ostream& os) const override {
                draw(data_, os);
            }

        private:
            T data_;
        };

        std::shared_ptr<Drawable> self_;
    };

    void draw(const Object& obj, std::ostream& os) {
        obj.dump(os);
    }

    using Document = std::vector<Object>;

    void draw(const Document& doc, std::ostream& os) {
        os << "<document>" << std::endl;
        for (const auto &val : doc) draw(val, os);
        os << "</document>" << std::endl;
    }
}

#endif // DOCUMENT_H
