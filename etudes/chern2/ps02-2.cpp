/* Задача ps02-2
 * Time limit: 1 s
 * Memory limit:   64 M

 * Напишите определения классов A и B так, чтобы компилировалась 
 и работала следующая программа.

 * Программа должна выводить числа 2 и 10.
 */

#include <new>
#include <iostream>

class B {
public:
	int val1;
	B (int p)
		:val1 (p)	
	{
	}
	virtual void g();
	virtual ~B
	{
	}
};

class A: public B
{
public:
	int val2;
	A (int, int);
	void g ()
	{
	}
};


A::A(int val1_, int val2_) : B(val1_), val2(val2_)
{
}

int
main()
{
    B *pb1 = new(std::nothrow) B(10);
    B *pb2 = new(std::nothrow) A(1, 2);

    if ( dynamic_cast<A*>(pb2) != NULL ) {
        static_cast<A*>(pb2)->g();
        std::cout << static_cast<A*>(pb2)->val2 << std::endl;
    }
    std::cout << pb1->val1 << std::endl;

    return 0;
}
