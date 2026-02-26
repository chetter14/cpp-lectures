#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <fstream>
#include <iterator>
#include <filesystem>

namespace {

}

int main() {
    {
        std::vector<int> v { 1, 2, 3 };
        std::list<std::string>::iterator lit;

        /* Seg fault */
        /* std::copy(v.begin(), v.end(), lit); */
    }

    {
        std::vector<int> vect;
        auto iter = vect.begin();

        /* Seg fault */
        /* std::cout << *iter << std::endl; */
    }

    {
        std::vector<int> v { 1, 2, 30, 40, 50 };
        auto vit = v.begin();
        std::advance(vit, 3);
        std::cout << *vit << std::endl; 

        v.clear();
        v.shrink_to_fit();
        /* UB */
        std::cout << *vit << std::endl;
    }
    
    {
        std::ifstream file("file.txt");
        std::istream_iterator<std::string> beg(file), end;
        std::copy(beg, end, std::ostream_iterator<std::string>(std::cout));
        std::cout << std::endl;

        /* 'beg' iterator is invalid here, so code below is not guaranteed to be correct */
        std::vector<std::string> vect(beg, end);
        std::copy(vect.begin(), vect.end(), std::ostream_iterator<std::string>(std::cout));
        std::cout << std::endl;
    }

    {
        std::filesystem::path p("file.txt");

        auto fnm = p.filename();
        std::cout << fnm << std::endl;

        auto new_name = fnm.replace_extension(".out").string();
        std::cout << new_name << std::endl;

        std::cout << fnm.extension().string() << " " << fnm.stem().string() << std::endl;
    }

    {
        std::filesystem::directory_iterator start{"folder"}, fin;
        /* Move directory_iterator-s, do not copy them */
        std::vector<std::filesystem::directory_entry> entries(
            std::make_move_iterator(start), std::make_move_iterator(fin));

        for (auto &&x : entries) {
            std::cout << x.path().string() << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "Passed!" << std::endl;
    return 0;
}