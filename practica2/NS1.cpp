//File NS1.cpp
#include "MyNS.h" 
#include <iostream>
long MyNS::factorial(int n)
{
	if (n == 0) {
		return 1;
	}
	return n * factorial(n - 1);
}
// Constructor for class CTest
MyNS::CTest::CTest()
{
	// Initialize something
}
void MyNS::CTest::Method1()
{
	std::cout << "we are on method1";
	// You have to write your code here
}
// More implementation for NS1.h prototypes goes here