#include "Polynomial.h"
#include <random>
#include <algorithm>
using namespace std;
int main()
{
    int n = 9;
    
    term * p1 = new term[n];
    term * p2 = new term[n];

    //随机数生成
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> intGen(0, 10);
    uniform_real_distribution<>  realGen(-20, 20);

    //构造两个term数组
    int e1[n], e2[n];
    double c1[n], c2[n];
    for(int i = 0; i < n; i++)
    {
        e1[i] = intGen(gen);
        e2[i] = intGen(gen);
        c1[i] = realGen(gen);
        c2[i] = realGen(gen);
        term temp1(c1[i], e1[i]);
        p1[i] = temp1;
        term temp2(c2[i], e2[i]);
        p2[i] = temp2;
    }
    sort(p1, p1+n);
    sort(p2, p2+n);
    cout << "p1:" << endl;
    for(int i = 0; i < n; i++)
    {
        cout << p1[i].coef << " "; 
    }
    cout << endl;
    for(int i = 0; i < n; i++)
    {
        cout << p1[i].expn << " "; 
    }
    cout << endl;
    cout << "p2:" << endl;
    for(int i = 0; i < n; i++)
    {
        cout << p2[i].coef << " "; 
    }
    cout << endl;
    for(int i = 0; i < n; i++)
    {
        cout << p2[i].expn << " "; 
    }
    cout << endl;

    Polynomial test1(p1, n);
    Polynomial test2(p2, n);

    test1.printPol();
    test2.printPol();

    Polynomial sum(test1 + test2);
    cout << "sum:";
    sum.printPol();

    Polynomial difference(test1 - test2);
    cout << "difference:";
    difference.printPol();

    Polynomial product(test1 * test2);
    cout << "product:";
    product.printPol();

    double value = test1.calculate(10);
    cout << "value:" << value << endl;

    Polynomial deriv(test1.derivative());
    cout << "derivative:";
    deriv.printPol();

    Polynomial test = pol_in();
    test.printPol();
    return 0;
}
