#include <iostream>
#include <vector>
#pragma once
using namespace std;
template<typename T>
struct doubleNode
{
    T val;
    doubleNode* next;
    doubleNode* pre;
    doubleNode(T &value = 0, doubleNode* nxt = nullptr, doubleNode* prev = nullptr)
    {
        this->val = value;
        this->next = nxt;
        this->pre = prev;
    }
    ~doubleNode()
    {
        this->next = nullptr;
        this->pre = nullptr;
        this->val = 0;
    }
};

template<typename T>
class doubleList
{
    typedef doubleNode<T> dN;
    dN* head;
    dN* back;
    int length;
public:
    doubleList(T data[], int n);        //构造函数
    doubleList(const doubleList<T>& a); //拷贝构造
    dN* front(){return head;}           //获取头节点
    dN* end(){return back;}            //获取尾节点
    void insert(T value);               //插入一个值为value的节点
    T Delete(int i);                    //删除位置为i的节点
    vector<int> Find(T target);         //找到值为target的节点，并返回其坐标vector
    int getLength();                    //获取链表长度
    void show();                        //显示链表内容
    ~doubleList();                      //析构函数
};

