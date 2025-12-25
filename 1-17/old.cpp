#include <iostream>

namespace { 
	class MyInt {
	public:
		explicit MyInt(int x) : x_(x) { }
		
		operator int() { return x_; }
		operator double() { return x_; }
		
	private:
		int x_;
	};
}

int main() {
	MyInt obj(3);
	std::cout << obj << "\n";
	std::cout << static_cast<double>(obj) << "\n";
	
	return 0;
}