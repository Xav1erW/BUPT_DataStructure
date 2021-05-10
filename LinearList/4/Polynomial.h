#include <iostream>
#include <string>
#include <cstring>
#include "doubleList.h"
#include <cmath>
#include <vector>
#pragma once
using namespace std;

struct term
{
    double coef;
    int expn;
    term(double c = 0, int e = 0)
    {
        coef = c;
        expn = e;
    }
    bool operator < (const term& b)
    {
        return this->expn < b.expn;
    }
    bool operator > (const term& b)
    {
        return this->expn > b.expn;
    }
    bool operator <= (const term& b)
    {
        return this->expn <= b.expn;
    }
    bool operator >= (const term& b)
    {
        return this->expn >= b.expn;
    }
    bool operator == (const term& b)
    {
        return this->expn == b.expn;
    }
};

class Polynomial : public doubleList<term>
{
public:
    Polynomial(term* = nullptr, int = 0);
    void printPol();
    Polynomial operator+(const Polynomial&);
    Polynomial operator-(const Polynomial&);
    Polynomial operator-()const;
    Polynomial operator*(const Polynomial&);
    double calculate(double x);
    Polynomial derivative();
    friend Polynomial pol_in();
};

#include "Polynomial.tpp"
