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

    template<typename KeyIt, typename ValueIt>
    class zip_iterator_t {
    public:
        using KeyType = typename std::iterator_traits<KeyIt>::value_type;
        using ValueType = typename std::iterator_traits<ValueIt>::value_type;
        using KeyDiff = typename std::iterator_traits<KeyIt>::difference_type;
        using ValueDiff = typename std::iterator_traits<ValueIt>::difference_type;
        using KeyRef = typename std::iterator_traits<KeyIt>::reference;
        using ValueRef = typename std::iterator_traits<ValueIt>::reference;
        using KeyCat = typename std::iterator_traits<KeyIt>::iterator_category;
        using ValueCat = typename std::iterator_traits<ValueIt>::iterator_category;

        static_assert(std::is_base_of_v<std::forward_iterator_tag, KeyCat>);
        static_assert(std::is_base_of_v<std::forward_iterator_tag, ValueCat>);

    private:
        /* Used for pointer dereference operator */
        template<typename Reference>
        struct arrow_proxy {
            Reference ref;
            Reference *operator->() { return &ref; }
        };

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = std::pair<KeyType, ValueType>;
        using reference = std::pair<KeyRef, ValueRef>;
        using difference_type = std::pair<KeyDiff, ValueDiff>;
        using pointer = arrow_proxy<reference>;

    public:
        zip_iterator_t(KeyIt kit, ValueIt vit) : kit_(kit), vit_(vit) { }

        zip_iterator_t &operator++() { 
            kit_++; 
            vit_++; 
            return *this; 
        }

        zip_iterator_t &operator++(int) { 
            auto temp{*this};
            operator++();
            return temp;
        }

        reference operator*() const { return {*kit_, *vit_}; }
        pointer operator->() const { return pointer{{*kit_, *vit_}}; }

        bool equals(const zip_iterator_t &rhs) const {
            return (kit_ == rhs.kit_) && (vit_ == rhs.vit_);
        }

    private:
        KeyIt kit_;
        ValueIt vit_;
    };

    template<typename KeyIt, typename ValueIt>
    bool operator==(const zip_iterator_t<KeyIt, ValueIt> &lhs, const zip_iterator_t<KeyIt, ValueIt> &rhs) {
        return lhs.equals(rhs);
    }

    template<typename KeyIt, typename ValueIt>
    bool operator!=(const zip_iterator_t<KeyIt, ValueIt> &lhs, const zip_iterator_t<KeyIt, ValueIt> &rhs) {
        return !lhs.equals(rhs);
    }

    template<typename KeyIt, typename ValueIt>
    auto make_zip_iterator(KeyIt kit, ValueIt vit) {
        return zip_iterator_t{kit, vit};
    }

    template<typename Keys, typename Values>
    class zip_range_t {
        Keys& K_;
        Values& V_;
        using KeyIter = typename std::remove_reference_t<Keys>::iterator;
        using ValueIter = typename std::remove_reference_t<Values>::iterator;

    public:
        zip_range_t(Keys& K, Values& V) : K_(K), V_(V) { }

        zip_iterator_t<KeyIter, ValueIter> begin() {
            return make_zip_iterator(std::begin(K_), std::begin(V_));
        }
        zip_iterator_t<KeyIter, ValueIter> end() {
            return make_zip_iterator(std::end(K_), std::end(V_));
        }
    };

    template<typename Keys, typename Values>
    auto make_zip_range(Keys& K, Values& V) {
        return zip_range_t<Keys, Values>(K, V);
    }

    /* Hinnant trick */
    template<typename Container, typename ConstIter>
    typename Container::iterator remove_constness(Container &cont, ConstIter citer) {
        return cont.erase(citer, citer);
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
        std::vector<double> values { 1.2, 2.3, 3.4, 4.5, 5.6 };

        for (auto &&both : make_zip_range(keys, values)) {
            std::cout << both.first << " " << both.second << std::endl;
        }
    }

    {
        std::vector<int> v{ 1, 2, 3, 4, 5 };
        auto cit = v.cbegin();
        auto it = remove_constness(v, cit);
        // *cit = 2;    // compile error
        *it = 20;

        for (auto x : v) std::cout << x << " ";
        std::cout << std::endl;
    }

    {
        std::vector<int> v{ 10, 20, 30, 40, 50 };
        auto ri = v.rbegin() + 4;
        auto it = ri.base();
        std::cout << *ri << " " << *it << std::endl; /* 10 20 */
    }

    {
        std::vector<double> vect{ 3.0, 5.2, 9.1 };
        auto it = vect.begin() + 2;
        auto ins_it = std::inserter(vect, vect.begin() + 2);

        *ins_it = 15.1;
        ++ins_it;
        *ins_it = 11.8;

        for (auto x : vect) std::cout << x << " ";
        std::cout << std::endl;

        /* 9.1 (even though now, technically, it's the last element in vector)*/
        std::cout << *it << std::endl;
    }

    return 0;
}