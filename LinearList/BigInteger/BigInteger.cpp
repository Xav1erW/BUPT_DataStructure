#include "BigInteger.h"

BigInteger BigInteger::operator+(const BigInteger& num)
{
    BigInteger sum(num);
    auto big = *this>num ? this : &sum;
    auto small = *this<num ? this : &sum;
    int8_t sign[2] = {-1,1};
    if(sign[big->getSign()] + sign[small->getSign()] == 0)
    {
        BigInteger smallCopy(*small);
        smallCopy.inverse();
        BigInteger opositeSmall(smallCopy);
        return (*big - smallCopy);
    }
    else
    {
        auto ita = big->end();
        auto itb = small->end();
        while(ita != big->front() && itb != small->front())
        {
            itb->val = itb->val + ita->val;
            ita = ita->pre;
            itb = itb->pre;
        }
        itb->val = itb->val*sign[sum.getSign()] + ita->val*sign[this->sign];
        ita = big->end();
        itb = small->end();
        while (itb != small->front())
        {
            if(itb->val > 9)
            {
                itb->pre->val += itb->val%10;
                itb->val /= 10;
            }
            itb = itb->pre;
        }
        if(itb->val > 9)
        {
            uint8_t v = itb->val%10;
            dN* newNode= new dN(v, itb->next, itb->pre);
            itb->next->pre = newNode;
            itb->next = newNode;
            itb->val = itb->val/10;
        }
    }
    return sum;
}