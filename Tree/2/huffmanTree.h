#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <iostream>
#include <fstream>
// #include <stack>

using namespace std;

struct node
{
    char ch;
    unsigned int freq;
    node* left;
    node* right;
    node(char c = 0,unsigned int f = 0, node* l = nullptr, node* r = nullptr)
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
    node root;
    unsigned int* freqTable;
    int ch_count;
    string text;
    unordered_map<char, string> encodeMap;
    unordered_map<string, char> decodeMap;
    void createFreqTable(string);
    void generateTable(const node*, string);

public:
    huffmanTree(string);            //从给定字符串构建huffmanTree
    huffmanTree(ifstream);
    void showTable();               //打印编码表
    void showEncoded();             //打印编码结果
    void encode(string);            //编码string并存入文件
    void decode(string);            //根据类内的编码表解码传入的string
    ~huffmanTree(){delete[] freqTable;}
};
void decode(string, string);        //打开编码文件并解码，将文本写入指定地址
struct huffmanHead
{
    int codeMapLength;
    int remainedBits;
    int tableSize;
};

const int MAXTEXTSIZE = 2048;
const int BUFFERSIZE = 1024;
