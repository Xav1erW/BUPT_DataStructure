#include "Polynomial.h"
// Polynomial::Polynomial(term* data, int n):doubleList(data, 1)
// {
//     for(int i = 1; i < n; i++)
//     {
//         if(data[i].coef == 0)
//         {
//             continue;
//         }
//         insert(data[i]);
//     }
//     cout << "inserted" << endl;
//     cout << length << endl;
//     dN* its = head;
//     // if(its->next->val == its->val)
//     // {
//     //     its->val.coef += its->next->val.coef;
//     //     Delete(its->next);
//     // }
//     // else
//     //     its = its->next;
//     while(its->next != head)         //合并相同指数项
//     {
//         if(its->next->val == its->val)
//         {
//             its->val.coef += its->next->val.coef;
//             Delete(its->next);
//         }
//         else
//             its = its->next;
//     }
//     // if(its->next->val == its->val)
//     // {
//     //     its->val.coef += its->next->val.coef;
//     //     Delete(its->next);
//     // }
//     // else
//     //     its = its->next;
    
//     if(head->val.coef == 0)
//     {
//         // its = its->next;
//         Delete(head);
//     }
//     // while(its != head)
//     // {
//     //     its = its->next;
//     //     Delete(its->pre);
//     // }
// }

Polynomial::Polynomial(term* data, int n):doubleList(data, 1)
{
    for(int i = 1; i < n; i++)      //先逐个插入
    {
        if(data[i].coef == 0)
        {
            continue;
        }
        insert(data[i]);
    }
    dN* its = head;
    while(its->next != head)         //合并相同指数项
    {
        if(its->next->val == its->val)
        {
            its->val.coef += its->next->val.coef;
            Delete(its->next);

            if(its->val.coef == 0)      //如果系数为0，则删除
            {
                its = its->next;
                Delete(its->pre);
            }
        }
        else
            its = its->next;
    }

}

void Polynomial::printPol()
{
    dN* it = head;
    //单独对于头节点
    if(it->val.expn == 0)
    {
        if(it->val.coef == 0);
        else
        {
           if(it->val.coef > 0)
            {
                cout << it->val.coef;
            }
            // else
            // {
            //     cout <<it->val.coef << "x^" << it->val.expn;
            // } 
        }
        it = it->next;
    }
    else
    {
        if(it->val.coef == 0);
        else
        {
            if(it->val.coef == 1.0)
            {
                cout << "x^" << it->val.expn; 
            }
            else if(it->val.coef > 0)
            {
                cout << it->val.coef << "x^" << it->val.expn;
            }
            else
            {
                cout << it->val.coef << "x^" << it->val.expn;
            }
        }
        
        it = it->next;
    }
    
    //之后的部分
    while(it != head)
    {
        if(it->val.expn == 0)
        {
            if(it->val.coef > 0)
            {
                cout << "+" << it->val.coef << it->val.expn;
            }
            else
            {
                cout << it->val.coef << it->val.expn;
            }
        }
        if(it->val.coef == 0)
        {
            Delete(it);
        }
        else
        {
            if(it->val.coef == 1.0)
            {
                cout << "+" <<"x^" << it->val.expn; 
            }
            else if(it->val.coef > 0)
            {
                cout << "+" << it->val.coef << "x^" << it->val.expn;
            }
            else
            {
                cout << it->val.coef << "x^" << it->val.expn;
            }
        }

        it = it->next;
    }
    cout << endl;
    cout << "print end" << endl;
}

Polynomial Polynomial:: operator+(const Polynomial& b)
{
    dN* ita = this->head;
    Polynomial sum (b);
    while(ita->next != head)
    {
        sum.insert(ita->val);
        ita = ita->next;
    }
    sum.insert(ita->val);
    dN* its = sum.front();

    if(its->next->val == its->val)
    {
        its->val.coef += its->next->val.coef;
        sum.Delete(its->next);
    }
    its = its->next;

    while(its != sum.front())         //合并相同指数项
    {
        if(its->next->val == its->val)
        {
            its->val.coef += its->next->val.coef;
            sum.Delete(its->next);
        }
        else
            its = its->next;
    }
    if(its->val.coef == 0)
    {
        its = its->next;
        sum.Delete(its->pre);
    }
    while(its != sum.front())
    {
        its = its->next;
        sum.Delete(its->pre);
    }
    return sum;
}

Polynomial Polynomial:: operator-()const
{
    Polynomial negative(*this);
    dN* it = negative.front();
    it->val.coef = -it->val.coef;
    it = it->next;
    while(it != negative.front())
    {
        it->val.coef = -it->val.coef;
        it = it->next;
    }
    return negative;
}

Polynomial Polynomial:: operator-(const Polynomial& b)
{
    dN* ita = this->head;
    Polynomial sum (-b);
    while(ita->next != head)
    {
        sum.insert(ita->val);
        ita = ita->next;
    }
    sum.insert(ita->val);
    dN* its = sum.front();

    if(its->next->val == its->val)
    {
        its->val.coef -= its->next->val.coef;
        sum.Delete(its->next);
    }
    else its = its->next;

    while(its != sum.front())         //合并相同指数项
    {
        if(its->next->val == its->val)
        {
            its->val.coef -= its->next->val.coef;
            sum.Delete(its->next);
        }
        else 
        {
            its = its->next;
        }
    }
    if(its->val.coef == 0)
    {
        its = its->next;
        sum.Delete(its->pre);
    }
    while(its != sum.front())
    {
        its = its->next;
        sum.Delete(its->pre);
    }
    return sum;
}

Polynomial Polynomial:: operator*(const Polynomial& b)
{
    dN* ita = this->head;
    Polynomial ans(b);
    dN* itAns = ans.front();

    term product(ita->val.coef*itAns->val.coef, ita->val.expn + itAns->val.expn); //单独处理头节点
    ans.Delete(itAns);
    ans.insert(product);
    itAns = itAns->next;
    while(itAns!= ans.front())
    {
        term product(ita->val.coef*itAns->val.coef, ita->val.expn + itAns->val.expn);
        ans.Delete(itAns);
        ans.insert(product);
        itAns = itAns->next;
    }
    ita = ita->next;

    while(ita != head)
    {
        term product(ita->val.coef*itAns->val.coef, ita->val.expn + itAns->val.expn);
        ans.Delete(itAns);
        ans.insert(product);
        itAns = itAns->next;
        while(itAns!= ans.front())
        {
            term product(ita->val.coef*itAns->val.coef, ita->val.expn + itAns->val.expn);
            ans.Delete(itAns);
            ans.insert(product);
            itAns = itAns->next;
        }
        ita = ita->next;
    }

    itAns = ans.front();            //处理头节点
    if(itAns->next->val == itAns->val)
    {
        itAns->val.coef += itAns->next->val.coef;
        ans.Delete(itAns->next);
    }
    itAns = itAns->next;

    while(itAns != ans.front())         //合并相同指数项
    {
        if(itAns->next->val == itAns->val)
        {
            itAns->val.coef += itAns->next->val.coef;
            ans.Delete(itAns->next);
        }
        else
        {
            itAns = itAns->next;
        }
    }

    if(itAns->val.coef == 0)
    {
        itAns = itAns->next;
        ans.Delete(itAns->pre);
    }
    while(itAns != ans.front())
    {
        itAns = itAns->next;
        ans.Delete(itAns->pre);
    }
    return ans;
}


double Polynomial::calculate(double x)
{
    double ans = 0;
    dN* it = head;
    ans += it->val.coef * pow(x, it->val.expn);
    it = it->next;
    while(it != head)
    {
        ans += it->val.coef * pow(x, it->val.expn);
        it = it->next;
    }
    return ans;
}

Polynomial Polynomial::derivative()
{
    Polynomial ans(*this);
    dN* it = ans.front();
    it->val.coef *= it->val.expn;
    it->val.expn--;
    it = it->next;
    while(it != ans.front())
    {
        it->val.coef *= it->val.expn;
        it->val.expn--;
        it = it->next;
    }
    return ans;
}

Polynomial pol_in()
{
    /*****************************
    输入格式：第一行，多项式项数 n
    后跟n行，每行两个数：系数和指数
    ******************************/
    
    int n;
    cin >> n;
    term* pol_in = new term[n];
    for(int i = 0; i < n; i++)
    {
        cin >> pol_in[i].coef >> pol_in[i].expn;
    }
    cout << "loop out" << endl;
    Polynomial ans(pol_in, n);
    delete pol_in;
    return ans;
}