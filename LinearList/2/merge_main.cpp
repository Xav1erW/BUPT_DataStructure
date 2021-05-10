#include "doubleList.h"
#include "doubleList.cpp"
int main()
{
    int* data1 =new int[5] {1,2,3,4,5};
    int* data2 =new int[5] {4,5,7,8,9};
    int n = 5;
    doubleList<int> part1(data1, n);
    doubleList<int> part2(data2, n);
    auto merged = Merge(part1, part2);
    merged.show();
    delete data1;
    delete data2;
    return 0;
}