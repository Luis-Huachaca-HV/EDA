#pragma once

// File MyNS.h
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
