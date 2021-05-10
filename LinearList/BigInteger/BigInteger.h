#include "doubleList.h"
class BigInteger:doubleList<uint8_t>
{
private:
    bool sign;          //正负号
public:
    BigInteger(const uint8_t* num, int length):doubleList(num, length){};
    BigInteger(const BigInteger& num):doubleList(num){sign = num.getSign();}
    BigInteger operator+(const BigInteger&);
    BigInteger operator-(const BigInteger&);
    BigInteger operator*(const BigInteger&);
    bool operator<(const BigInteger&);
    bool operator>(const BigInteger&);
    bool operator==(const BigInteger&);
    bool getSign()const {return sign;}
    void print();
    void inverse(){sign = sign? false : true;};
};