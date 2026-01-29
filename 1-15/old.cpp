#include <iostream>
#include <string>
#include <vector>
#include <cassert>

namespace { 
	class MyInt {
	public:
		explicit MyInt(int x) : x_(x) { }
		
		operator int() { return x_; }
		// operator double() { return x_; }
		
	private:
		int x_;
	};

	union MyUni {
		std::string str;
		std::vector<int> vect;

		// Correctly, add an index/type tag and write proper handling logic 
		// 	in constructor(s) and destructor (call appropriate ones).

		// Now constructor and destructor, essentially, do nothing.
		MyUni() { std::cout << "Default constructor\n"; }
		~MyUni() { std::cout << "Default destructor\n"; }
	};

	class IBase {
	public:
		virtual IBase *clone() const = 0;
		virtual void out() const = 0;
		virtual void assign(int x) = 0;
		virtual ~IBase(){}
	};

	class Derived final : public IBase {
	public:
		explicit Derived(int x) : x_(x) { }

		virtual IBase *clone() const override {
			return new Derived(x_);
		}

		void out() const override { std::cout << x_ << std::endl; }
		void assign(int x) override { x_ = x; }
	private:
		int x_;
	};
}

int main() {
	{
		MyInt obj(3);
		std::cout << obj << "\n";

		int a = obj;
		double b = obj;
		MyInt c{2};

		// Compile error:
		// MyInt d = 2;

		std::cout << static_cast<double>(c) << "\n";
	}

	{
		MyUni uni;
		uni.str = "Hello world";
		std::cout << uni.str << std::endl;
	}

	{
		IBase *a = new Derived(23);
		IBase *b = a->clone();
		a->out();
		b->out();
		
		a->assign(104);
		a->out();
		b->out();

		b->assign(41);
		a->out();
		b->out();
	}

	{
		IBase *a = new Derived(1);
		Derived b(5);

		assert(typeid(IBase*) == typeid(a));
		assert(typeid(IBase) != typeid(*a));

		assert(typeid(Derived) == typeid(b));
		assert(typeid(Derived) == typeid(*a));
		
		// Assert will be failed:
		// assert(typeid(IBase) == typeid(*a));

		std::cout << "Passed asserts" << std::endl;
	}

	return 0;
}