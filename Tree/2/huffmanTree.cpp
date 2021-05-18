#include "huffmanTree.h"

void huffmanTree::createFreqTable(string str)
{
    freqTable = new unsigned int[256];
    for (unsigned char ch : str)
    {
        freqTable[ch]++;
    }
}

struct cmp
{
    bool operator()(const node* n1, const node* n2)
    {
        return n1->freq > n2->freq;
    }
};

huffmanTree::huffmanTree(string str)
{
    text = str;
    createFreqTable(str);
    priority_queue<node*, vector<node*>, cmp> sortedStr;
    for (unsigned char ch = 0; ch < 255; ch++)
    {
        if (freqTable[ch] != 0)
        {
            node* add = new node(ch, freqTable[ch]);
            sortedStr.emplace(add);
        }
    }
    if (freqTable[255] != 0)
    {
        node* add = new node(255, freqTable[255]);
        sortedStr.emplace(add);
    }

    while (!sortedStr.empty())
    {
        node* lchild = sortedStr.top();
        sortedStr.pop();
        node* rchild = sortedStr.top();
        sortedStr.pop();
        if (sortedStr.empty())              //队列空，没有剩余节点，直接创建最后的根节点
        {
            root = new node(0, lchild->freq + rchild->freq, lchild, rchild);
        }
        else                                //最大两节点的次root节点
        {
            node* subroot = new node(-1, lchild->freq + rchild->freq, lchild, rchild);
            sortedStr.emplace(subroot);
        }
    }
    const node* temp = root;
    generateTable(temp);
}

huffmanTree::huffmanTree(ifstream& file)
{
    file.seekg(0, file.end);
    int bytes = file.tellg();       //文件字节数
    file.seekg(0);                  //位置置零
    char* data = new char[bytes];
    file.read(data, bytes);
    string text(data, bytes);
    
    delete[] data;
    new (this)huffmanTree(text);
}

void huffmanTree::generateTable(const node *r, string binCode)
{
    if (r != nullptr)
    {
        if (r->left == nullptr && r->right == nullptr)      //左右都为空，为叶子节点，构建表
        {
            encodeMap.emplace(r->ch, binCode);
            decodeMap.emplace(binCode, r->ch);
            return;
        }
        else            //遍历左右子树
        {
            generateTable(r->left, binCode + "0");
            generateTable(r->right, binCode + "1");
        }
    }
}



void huffmanTree::showTable()
{
    for (auto it = encodeMap.begin(); it != encodeMap.end(); it++)
    {
        cout << (int)it->first << "\t" << it->second << endl;
    }
}

void huffmanTree::showEncoded()
{

    if(text.size() > MAXTEXTSIZE)
    {
        throw("Text is too long. Please save in a file.\n");
    }
    string result;
    for(unsigned char ch:text)
    {
        result += encodeMap[ch];
    }
    cout << result << endl;    
}


void huffmanTree::encode(string address)
{
    ofstream file(address, ios::binary);
    file.seekp(sizeof(huffmanHead));
    huffmanHead header;
    //写入编码表，结构：char + code + '\0'，最后的'\0'作为标志
    for (auto it = decodeMap.begin(); it != decodeMap.end(); it++)
    {
        header.tableSize += it->first.length() + 2;
        file << it->second;
        file.write(it->first.c_str(), it->first.length());
        file << '\0';
    }

    string code = "";
    for (unsigned char ch : text)
    {
        code += encodeMap[ch];
    }
    //按字节写入文件
    uint8_t byteBuffer = 0;
    int counter = 0;            //记录写入的bit数
    for (char bit : code)
    {
        if (counter%8 == 0 && counter != 0)     //凑够一个字节，开始写
        {
            file.write((char *)&byteBuffer, 1);
            byteBuffer = 0;
            byteBuffer <<= 1;
            byteBuffer += bit - '0';
            counter++;
            header.dataSize++;
        }
        else
        {
            byteBuffer <<= 1;
            byteBuffer += bit - '0';
            counter++;
        }
    }


    //不足一个字节的bit数
    header.remainedBits = code.length() % 8;

    //写入不足八位的编码
    file.write((char *)&byteBuffer, 1);
    header.dataSize++;


    //写入文件头
    file.seekp(0);
    file.write((char*)&header, sizeof(header));
}


void huffmanTree::decode(string code)
{
    string charCode;
    for(char bit:code)
    {
        charCode.push_back(bit);
        //如果找到了对应的字符
        if(decodeMap.find(charCode) != decodeMap.end())
        {
            unsigned char search = decodeMap[charCode];
            cout << search;
            charCode = "";
        }        
    }
    cout << endl;
}

void decode(string in, string out)
{
    ifstream fileIn;
    ofstream fileOut;
    fileIn.open(in, ios::binary);
    fileOut.open(out, ios::binary);

    //读取文件头和编码表
    huffmanHead header;
    fileIn.read((char*)&header, sizeof(header));
    char* temp = new char[header.tableSize];
    fileIn.read(temp, header.tableSize);
    string codeTable(temp, header.tableSize);
    delete[] temp;


    //构造hash编码表
    //文件中编码表 char + code + '\0'
    unordered_map<string, unsigned char> decodeMap;
    string code;
    bool isChar = true;         //如果读到'\0'表示下一个是字符，转为true，其余时间为false，默认为true，因为编码表开头是字符
    unsigned char value;
    for(unsigned char ch:codeTable)
    {
        if(isChar)
        {
            value = ch;
            isChar = false;
        }
        else if(ch == '\0')
        {
            isChar = true;
            decodeMap.emplace(code, value);
            code = "";
        }
        else
        {
            code.push_back(ch);
        }
    }

    char* dataIn = new char[header.dataSize];
    string outputText;
    unsigned char mask = 128;           //二进制10000000，用于利用and取最高位
    code = "";
    
    fileIn.read(dataIn, header.dataSize);
    int i = 0;
    int bitCount = 0;               //记录已经读取的bit数，每8位读取下一字节
    unsigned char byte = 0;
    while (i != header.dataSize)             //逐个读取dataIn中的数据
    {
        if(bitCount % 8 == 0)
        {
            byte = dataIn[i];
            i++;
            if(i == header.dataSize) break;     //读到最后一字节，直接跳出循环单独处理
            unsigned char bitByChar = ((byte&mask) >> 7) + '0';   //用字符‘0’‘1’存储单个bit
            byte <<= 1;
            bitCount ++;
            code.push_back(bitByChar);
            if(decodeMap.find(code) != decodeMap.end())      //在hash表中找到存在的对应字符
            {
                outputText.push_back(decodeMap[code]);
                code = "";              //code置零，读取下一个编码准备
            }
        }
        else
        {
            unsigned char bitByChar = ((byte&mask) >> 7) + '0';   //用字符‘0’‘1’存储单个bit
            byte <<= 1;
            bitCount ++;
            code.push_back(bitByChar);
            if(decodeMap.find(code) != decodeMap.end())     //在hash表中找到存在的对应字符
            {
                outputText.push_back(decodeMap[code]);
                code = "";          //code置零，读取下一个编码准备
            }
        }
    }
    //读取不足一字节的数据
    byte <<= (8-header.remainedBits);
    bitCount = 8-header.remainedBits;
    while(bitCount != 8)
    {
        unsigned char bitByChar = ((byte&mask) >> 7)+'0';
        byte <<= 1;
        bitCount++;
        code.push_back(bitByChar);
        if(decodeMap.find(code) != decodeMap.end())     //在hash表中找到存在的对应字符
        {
            outputText.push_back(decodeMap[code]);
            code = "";              //code置零，读取下一个编码准备
        }
    }
    
    delete[] dataIn;
    
    fileOut.write(outputText.c_str(), outputText.length());
}

huffmanTree::~huffmanTree()
{
    delete[] freqTable;
    deleteNodes(root);
}

void huffmanTree::deleteNodes(node* n)
{
    if(n == nullptr)
    {
        return;
    }
    else
    {
        deleteNodes(n->left);
        deleteNodes(n->right);
        delete n;
    }
}