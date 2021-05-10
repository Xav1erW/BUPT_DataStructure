#include <iostream>
#include <limits.h>
#include <vector>
using namespace std;
template<typename T>
struct singleNode
{
    T val;
    singleNode* next;
    singleNode(T &value, singleNode* nxt = nullptr)
    {
        this->val = value;
        this->next = nxt;
    }
    singleNode()
    {
        this->next = nullptr;
    }
};

template<typename T>
class singleListWithHead
{
    typedef singleNode<T> sN;
    sN* head;
    int length;
public:
    singleListWithHead(T data[], int n);
    void insert(T value);
    T Delete(int i);
    vector<int> Find(T target);
    int getLength();
    void show();
    ~singleListWithHead();
};
// template<typename T>
// singleListWithHead<T>::singleListWithHead(T data[], int n)  //尾插法
// {
//     length = n;
//     this->head = new sN(INT_MIN);
//     sN* ptr = head;
//     for(int i = 0; i < n; i++)
//     {
//         ptr->next = new sN(data[i]);
//         ptr = ptr->next;
//     }
// }

template<typename T>
singleListWithHead<T>::singleListWithHead(T data[], int n)   //头插法
{
    length = n;
    this->head = new sN;
    sN* second = nullptr;
    for(int i = 0; i < n; i++)
    {
        sN* temp = new sN(data[i], second);
        second = temp;
        head->next = temp;
    }
}

template<typename T>
void singleListWithHead<T>::insert(T value)
{
    sN* left = head;
    sN* right = head->next;
    bool inserted = false;      //标记是否成功插入
    while(right != nullptr)
    {
        if((value <= right->val && value >= left-> val) || (value >= right->val && value <= left->val) && left != head)
        {
            left->next = new sN(value, right);
            inserted = true;
            return;
        }
        left = left->next;
        right = right->next;
    }
    if (!inserted)
    {
        right = new sN(value);
        left->next = right;
    }
    this->length ++;
    return;
}

template<typename T>
T singleListWithHead<T>:: Delete(int i)
{
    sN* it = head;
    if(i > this->length) throw("Out of index range");
    for(int j = 1; j < i; j++)
    {
        it = it->next;
    }
    sN* temp = it->next;
    it->next = temp->next;
    T r = temp->val;
    delete temp;
    this->length--;
    return r;
}

template<typename T>
vector<int> singleListWithHead<T>:: Find(T target)
{
    if(this->length == 0) throw("Empty List");
    vector<int> ans;
    sN* it = head->next;
    int index = 1;
    while(it != nullptr)
    {
        if(it->val == target)
        {
            ans.push_back(index);
        }
        index++;
        it = it->next;
    }
    if(ans.empty()) throw("Can't Find");
    return ans;
}

template<typename T>
int singleListWithHead<T>::getLength()
{
    return this->length;
}

template<typename T>
singleListWithHead<T>:: ~singleListWithHead()
{
    sN* it = head;
    while(it != nullptr)
    {
        sN* temp = it;
        it = it->next;
        delete temp;
    }
}

template<typename T>
void singleListWithHead<T>::show()
{
    if(this->length == 0) throw("Empty List");
    sN* it = head->next;
    while(it != nullptr)
    {
        cout << it->val << " ";
        it = it->next;
    }
    cout << endl;
}

int main()
{
    int a[5] = {1,2,3,4,5};
    singleListWithHead <int> test(a, 5);
    test.show();
    try{
        test.Delete(2);
    }catch(const char* msg){cout << msg << endl;}
    test.show();
    cout << "Length:" <<test.getLength() << endl;
    try
    {
        vector<int> index =  test.Find(3);
        int length = index.size();
        for(int i = 0; i < length; i++)
        {
            cout << "index list:" << index[i] << " ";
        }
        cout << endl;
    }catch(const char* msg)
    {
        cout << msg << endl;
    }
    // cout << "here" << endl;
    test.insert(2);
    test.show();
    test.insert(100);
    test.show();
    return 0;
}