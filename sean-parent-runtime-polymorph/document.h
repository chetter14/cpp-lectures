#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <vector>
#include <iostream>
#include <memory>

template<typename T>
void draw(const T& x, std::ostream& os) {
    os << x << std::endl;
}

namespace Doc {

    class Object {
    public:
        template<typename T>
        Object(T val) : self_(std::make_unique<DrawableObject<T>>(std::move(val))) { }

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
            DrawableObject(T&& data) : data_(std::move(data)) { }

            void draw_(std::ostream& os) const override {
                draw(data_, os);
            }

        private:
            T data_;
        };

        std::unique_ptr<Drawable> self_;
    };

    using Document = std::vector<Object>;
}

template<>
void draw<Doc::Object>(const Doc::Object& obj, std::ostream& os);

template<>
void draw<Doc::Document>(const Doc::Document& doc, std::ostream& os);

#endif // DOCUMENT_H
