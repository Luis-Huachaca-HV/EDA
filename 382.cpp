// 382.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
using namespace std;
float Addition(float a, float b) {
    return a + b;
}
float Subtraction(float a, float b) {
    return a - b;

}
float Multiplication(float a, float b) {
    return a * b;
}
float Division(float a, float b) {
    return a / b;
}

float cod1() {
    // Code A
    float a, b, c; int opt;
    // enter the operands
    cin >> a >> b;
    // OPT-> 0-Addition, 1-Subtraction, 3-Multiplication, 4-Division
    cin >> opt;
    switch (opt)
    {
    case 0: c = Addition(a, b); break;
    case 1: c = Subtraction(a, b); break;
    case 2: c = Multiplication(a, b); break;
    case 3: c = Division(a, b); break;
        // more cases operations go here
    }
    return c;
}

float cod2() {
    // Code B
    // User type to simplify the declaration
    typedef float (*lpfnOperation)(float, float);
    // CVector of pointer to functions
    lpfnOperation vpf[4] = { &::Addition, &::Subtraction,
    &::Multiplication, &::Division };
    float a, b, c; int opt;
    // enter the operands
    cin >> a >> b;
    // enter the operation 0-Addition, 1-Subtraction, 3-Multiplication, 4-Division
    cin >> opt;
    // The next line replaces the switch and replaces the whole switch
    c = (*vpf[opt])(a, b);
    return c;
}
int main()
{

    cout << cod1() << endl;
    cout<< cod2();
    // Code A
   
   
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
