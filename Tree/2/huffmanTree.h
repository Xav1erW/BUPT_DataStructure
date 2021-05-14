#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <iostream>
#include <fstream>

using namespace std;
struct node
{
    unsigned char ch;
    unsigned int freq;
    node* left;
    node* right;
    node(unsigned char c = 0,unsigned int f = 0, node* l = nullptr, node* r = nullptr)
    {
        ch = c;
        freq = f;
        left = l;
        right = r;
    }
};


class huffmanTree
{
private:
    node* root;
    unsigned int* freqTable;
    string text;
    unordered_map<unsigned char, string> encodeMap;
    unordered_map<string, unsigned char> decodeMap;
    void createFreqTable(string);
    void generateTable(const node *r, string binCode = "");
    void deleteNodes(node*);
public:
    huffmanTree(string);            //从给定字符串构建huffmanTree
    huffmanTree(ifstream&);          //从文件构建huffmanTree
    void showTable();               //打印编码表
    void showEncoded();             //打印编码结果
    void encode(string);            //编码string并存入文件
    void decode(string);            //根据类内的编码表解码传入的string，并打印结果
    ~huffmanTree();
};
void decode(string, string);        //打开编码文件并解码，将文本写入指定地址，两个string是数据和输出文件地址
struct huffmanHead
{
    // int codeMapLength;              //共出现多少个不同字符
    int remainedBits = 0;               //最后多出的不满八位的bit数
    int tableSize = 0;                  //文件中编码表的字节长度
    int dataSize = 0;
};

const int MAXTEXTSIZE = 2048;       //可直接输出编码的最大文本大小
const int BUFFERSIZE = 1024;        //buffer大小


