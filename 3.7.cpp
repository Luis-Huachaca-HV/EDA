// aa.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;
#include <vector>


namespace MyNS
{
    int gnCount = 8; // Global counter
    // Some small function implemented inline
    double Addition(double a, double b) // Global function
    {
        return a + b;
    }
    // Some prototypes
    long factorial(int n);// Global function
    class CTest
    {
    public:
        CTest();
        void Method1();
    };
    // More variables, functions, methods, classes, structs go here
};

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
// More implementation 
int main()
{
   

        cout << MyNS::factorial(5) << endl;
        cout << MyNS::gnCount << endl;
}

