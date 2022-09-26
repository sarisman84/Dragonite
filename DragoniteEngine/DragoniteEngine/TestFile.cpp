#include "TestFile.h"
#include <stdexcept>

double MathFuncs::MyMathFuncs::Add(double a, double b)
{
    return a + b + a;
}

double MathFuncs::MyMathFuncs::Subtract(double a, double b)
{
    return a - b;
}

double MathFuncs::MyMathFuncs::Multiply(double a, double b)
{
    return a * b;
}

double MathFuncs::MyMathFuncs::Divide(double a, double b)
{
    if (b == 0)
    {
        throw new std::invalid_argument("b cannot be zero!");
    }

    return a / b;
}
