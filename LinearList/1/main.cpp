#include "doubleList.h"
#include "doubleList.cpp"
int main()
{
    int test[5] = {1,2,3,4,5};
    doubleList<int> list (test, 5);
    list.show();
    list.insert(3);
    cout << "Length:" << list.getLength() << endl;
    list.show();
    list.insert(10);
    list.show();
    try{
        list.Delete(10);
        cout << "Length:" << list.getLength() << endl;
    }catch(const char* msg){
        cout << msg;
    }

    try{
        list.Delete(2);
        cout << "Length:" << list.getLength() << endl;
    }catch(const char* msg){
        cout << msg;
    }

    try{
        list.show();
        list.Delete(6);
        cout << "Length:" << list.getLength() << endl;
    }catch(const char* msg){
        cout << msg;
    }

    try{
        vector<int> result = list.Find(2);
        for(int idx : result)
        {
            cout << idx << ' ';
        }
        cout << endl;

        result.clear();

        result = list.Find(9);
        for(int idx : result)
        {
            cout << idx << ' ';
        }
        cout << endl;
    }catch(const char* msg){
        cout << msg;
    }
    return 0;
}