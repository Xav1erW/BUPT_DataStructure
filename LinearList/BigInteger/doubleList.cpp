#include "doubleList.h"
template<typename T>
doubleList<T>::doubleList(const T* data = nullptr, int n = 0)
{
    if(n <= 0) throw("NO DATA\n");
    this->length = n;
    this->head = new dN(data[0], head, head);
    dN* it = head;
    for(int i = 1; i < n; i++)
    {
        dN* temp = new dN(data[i], head, it);
        head->pre = temp;
        it->next = temp;
        it = it->next;
    }
    back = head->pre;
}

template<typename T>
doubleList<T>::doubleList(const doubleList<T>& a)
{
    this->length = a.getLength();
    this->head = new dN(a.front()->val, head, head);
    dN* it = this->head;
    dN* itA = a.front()->next;
    for(int i = 1; i < length; i++)
    {
        dN* temp = new dN(itA->val, head, it);
        head->pre = temp;
        it->next = temp;
        it = it->next;
        itA = itA->next;
    }
    back = head->pre;
}

template<typename T>
void doubleList<T>::insert(T value)
{
    if(this->length == 0)
    {
        head->next = new dN(value, head, head);
        head->pre = head->next;
    }
    else
    {
        bool inserted = false;
        dN* it = head;
        while(it->next != head)
        {
            if(value >= it->val && value <= it->next->val)
            {
                dN* ins = new dN(value, it->next, it);
                it->next = ins;
                ins->next->pre = ins;
                inserted = true;
                break;
            }
            it = it->next;
        }
        if(!inserted)
        {
            dN* ins = new dN(value, head, head->pre);
            head->pre = ins;
            ins->pre->next = ins;
        }
    }
    this->length  ++;
    back = head->pre;
}

template<typename T>
T doubleList<T>::Delete(int i)
{
    if(i > length) throw("INDEX OVERFLOW\n");
    dN* it = head;
    for(int j = 1; j < i; j++)
    {
        it = it->next;
    }
    if(i == 1){head = head->next;}
    it->pre->next = it->next;
    it->next->pre = it->pre;
    T value = it->val;
    delete it;
    length--;
    back = head->pre;
    return value;
}

template<typename T>
T doubleList<T>::Delete(dN* node)
{
    if(node == head){head = head->next;}
    node->pre->next = node->next;
    node->next->pre = node->pre;
    T value = node->val;
    delete node;
    length --;
    back = head->pre;
    return value;
}

template<typename T>
vector<int> doubleList<T>::Find(T target)
{
    vector<int> ans;
    dN* it = head;
    int i = 0;
    for(int i = -1; i < length; i++)
    {
        if(it->val == target)
        {
            ans.emplace_back(i+0);
        }
        it = it->next;
    }
    if(ans.empty()) throw("CAN'T FIND\n");
    return ans;
}

template<typename T>
int doubleList<T>::getLength() const
{
    return length;
}

template<typename T>
void doubleList<T>::show()
{
    dN* it = head;
    for(int i = 0; i < length; i++)
    {
        cout << it->val << " ";
        it = it->next;
    }
    cout << endl;
}

template<typename T>
doubleList<T>::~doubleList()
{
    dN* it = head;
    head->pre->next = nullptr;
    while(it->next != nullptr)
    {
        it = it->next;
        delete it->pre;
    }
    delete it;
    cout << "delete success!" << endl;
}

template<typename U>
doubleList<U> Merge(const doubleList<U>& a, const doubleList<U>& b)
{
    typedef doubleNode<U> dN;
    doubleList<U> ans(a);
    dN* itB = b.head;
    for(int i = 0; i < b.length; i++)
    {
        ans.insert(itB->val);
        itB = itB->next;
    }
    dN* it = ans.head;
    int i = 1;
    while(it->next != ans.head)
    {
        if(it->val == it->next->val)
        {
            ans.Delete(i+1);
        }
        else
        {
            it = it->next;
            i++;
        }
    }
    return ans;
}



template<typename T>
void doubleList<T>::push_back(T value)
{
    dN* add = new dN(value, head, back);
    head->pre = add;
    back->next = add;
}

template<typename T>
void doubleList<T>::push_back(dN* node)
{
    node->next = head;
    node->pre = back;
    head->pre = node;
    back->next = node;
}

template<typename T>
void doubleList<T>::insert(T value, dN* node)
{
    dN* newNode(value);
    newNode->pre = node;
    newNode->next = node->next;
    node->next = newNode;
    newNode->next->pre = newNode;
}