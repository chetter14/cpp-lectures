#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <type_traits>

namespace {
    template<typename C, typename F>
    std::size_t traverse(C&& cont, F fun) {
        auto it = std::find_if(cont.begin(), cont.end(), fun);
        return std::distance(cont.begin(), it);
    }

    std::ostream& operator<<(std::ostream& out, std::output_iterator_tag) {
        out << "output iterator";
        return out;
    }

    std::ostream& operator<<(std::ostream& out, std::input_iterator_tag) {
        out << "input iterator";
        return out;
    }

    std::ostream& operator<<(std::ostream& out, std::forward_iterator_tag) {
        out << "forward iterator";
        return out;
    }

    std::ostream& operator<<(std::ostream& out, std::bidirectional_iterator_tag) {
        out << "bidirectional iterator";
        return out;
    }

    std::ostream& operator<<(std::ostream& out, std::random_access_iterator_tag) {
        out << "random access iterator";
        return out;
    }

    template<typename Iter>
    void print_iterator_type() {
        std::cout << typename std::iterator_traits<Iter>::iterator_category{} << std::endl; 
    }

    template<bool B, typename T>
    struct conditional { using type = T; };

    template<typename T>
    struct conditional<false, T> { };

    template<bool B, typename T>
    using conditional_t = typename conditional<B, T>::type;

    template<bool B, typename T = void>
    struct enable_if { using type = T; };

    template<typename T>
    struct enable_if<false, T> { };

    template<bool B, typename T>
    using enable_if_t = typename enable_if<B, T>::type;

    template<typename It>
    using iterator_category_t = typename std::iterator_traits<It>::iterator_category;

    /* Define function only for random access iterators */
    template<typename It, typename T = std::enable_if_t<
        std::is_base_of_v<std::random_access_iterator_tag, iterator_category_t<It>>>>
    void foo(It first, It last) {
        std::cout << "Called for random access iterator" << std::endl;
    }

    /* Used for pointer dereference operator in iterator class */
    template<typename Reference>
    struct arrow_proxy {
        Reference ref;
        Reference *operator->() { return &ref; }
    };

    template<typename KeyIt, typename ValueIt>
    class zip_iterator_t {
    public:
        using KeyType = typename std::iterator_traits<KeyIt>::value_type;
        using ValueType = typename std::iterator_traits<ValueIt>::value_type;
        
        using KeyRef = typename std::iterator_traits<KeyIt>::reference;
        using ValueRef = typename std::iterator_traits<ValueIt>::reference;

        using value_type = std::pair<KeyType, ValueType>;
        using reference = std::pair<KeyRef, ValueRef>;
        using pointer = arrow_proxy<reference>;

    public:
        zip_iterator_t(KeyIt kit, ValueIt vit) : kit_(kit), vit_(vit) { }
        zip_iterator_t &operator++() { kit_++; vit_++; return *this; }

        reference operator*() const { return {*kit_, *vit_}; }
        pointer operator->() const { return pointer{{*kit_, *vit_}}; }

    private:
        KeyIt kit_;
        ValueIt vit_;
    };

    template<typename Keys, typename Values>
    class zip_range_t {
        Keys& K_;
        Values& V_;

    public:
        auto begin() {
            return make_zip_iterator(std::begin(K_), std::begin(V_));
        }

    };

    template<typename Keys, typename Values>
    auto make_zip_range(Keys& K, Values& V) {
        return zip_range_t<Keys, Values>(K, V);
    }
}

int main() {
    
    {
        std::vector<int> v { 1, 5, 8, 10, 2, 3 };
        std::cout << traverse(v, [](auto x) -> bool { return x > 9; }) << std::endl;
    }

    {
        print_iterator_type<typename std::vector<int>::iterator>();
        print_iterator_type<typename std::list<int>::iterator>();
        print_iterator_type<std::istream_iterator<int>>();
        print_iterator_type<std::ostream_iterator<int>>();
    }

    {
        std::vector<int> v;
        std::list<int> l;
        foo(v.begin(), v.end());
        // foo(l.begin(), l.end());     // compile error
    }

    {
        std::vector<int> keys { 1, 2, 3, 4, 5 };
        std::vector<double> values { 1.0, 2.0, 3.0, 4.0, 5.0 };

        for (auto &&both : make_zip_range(keys, values)) {
            ;
        }
    }

    return 0;
}