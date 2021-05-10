#include "huffmanTree.h"

void huffmanTree::createFreqTable(string str)
{
    freqTable = new unsigned int[128];
    for (char ch : str)
    {
        freqTable[ch]++;
    }
}

struct cmp
{
    bool operator()(const node &n1, const node &n2)
    {
        return n1.freq < n2.freq;
    }
};

huffmanTree::huffmanTree(string str)
{
    text = str;
    createFreqTable(str);
    priority_queue<node, vector<node>, cmp> sortedStr;
    for (char ch = 0; ch < 127; ch++)
    {
        if (freqTable[ch] != 0)
        {
            ch_count++;
            node add(ch, freqTable[ch]);
            sortedStr.emplace(add);
        }
    }

    while (!sortedStr.empty())
    {
        node lchild = sortedStr.top();
        sortedStr.pop();
        node rchild = sortedStr.top();
        sortedStr.pop();
        if (sortedStr.empty())
        {
            root = node(-1, lchild.freq + rchild.freq, &lchild, &rchild);
        }
        else
        {
            node lroot = node(-1, lchild.freq + rchild.freq, &lchild, &rchild);
            sortedStr.emplace(lroot);
        }
    }
    generateTable(&root);
}

void huffmanTree::generateTable(const node *r, string binCode = "")
{
    while (r != nullptr)
    {
        if (r->left != nullptr || r->right != nullptr)
        {
            generateTable(r->left, binCode + "0");
            generateTable(r->right, binCode + "1");
        }
        else
        {
            encodeMap.emplace(r->ch, binCode);
            decodeMap.emplace(binCode, r->ch);
            return;
        }
    }
}

void huffmanTree::showTable()
{
    for (auto it = encodeMap.begin(); it != encodeMap.end(); it++)
    {
        cout << it->first << "\t" << it->second << endl;
    }
}

void huffmanTree::showEncoded()
{

    if(text.size() > MAXTEXTSIZE)
    {
        throw("Text is too long. Please save in a file.\n");
    }
    string result;
    for(char ch:text)
    {
        result += encodeMap[ch];
    }
    cout << result;    
}


void huffmanTree::encode(string address)
{
    ofstream file(address, ios::binary);
    file.seekp(sizeof(huffmanHead));
    huffmanHead header;
    header.codeMapLength = encodeMap.size();
    for (auto it = decodeMap.begin(); it != decodeMap.end(); it++)
    {
        header.tableSize += 1 + it->first.length();
        file.write(it->first.c_str(), it->first.length()+1);
        file << it->second;
    }

    string buffer;
    for (char ch : text)
    {
        if (buffer.length() + encodeMap[ch].length() <= BUFFERSIZE)
        {
            buffer += encodeMap[ch];
        }
        else
        {
            //如果buffer存满了，开始写入
            int len = BUFFERSIZE - buffer.length();
            buffer += encodeMap[ch].substr(0, len);

            //按字节写入文件
            uint8_t byteBuffer = 0;
            int counter = 0;
            for (unsigned char bit : buffer)
            {
                if (counter == 8)
                {
                    file.write((char *)&byteBuffer, 1);
                    byteBuffer = 0;
                    byteBuffer << 1;
                    byteBuffer += bit - '0';
                }
                else
                {
                    byteBuffer << 1;
                    byteBuffer += bit - '0';
                    counter++;
                }
            }
        }
    }

    //保存最后未存满一个buffer的数据

    //不足一个字节的bit数
    int remainedBits = buffer.length() % 8;
    uint8_t byteBuffer = 0;
    for (auto it = buffer.begin(); it != buffer.end() - remainedBits; it++)
    {
        //按字节写入文件
        int counter = 0;
        if (counter == 8)
        {
            file.write((char *)&byteBuffer, 1);
            byteBuffer = 0;
            byteBuffer << 1;
            byteBuffer += *it - '0';
        }
        else
        {
            byteBuffer << 1;
            byteBuffer += *it - '0';
            counter++;
        }
    }
    //写入剩余的buffer内容
    file.write((char *)&byteBuffer, 1);

    byteBuffer = 0;
    for (auto it = buffer.end() - remainedBits; it != buffer.end(); it++)
    {
        byteBuffer << 1;
        byteBuffer += *it;
    }
    //写入不足八位的编码
    file.write((char *)&byteBuffer, 1);

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
        if(char search = decodeMap.find(charCode) != decodeMap.end())
        {
            cout << search;
            charCode = "";
        }        
    }
}

void decode(string in, string out)
{
    ifstream fileIn;
    ofstream fileOut;
    fileIn.open(in, ios::binary);
    fileOut.open(out, ios::out);

    //读取文件头和编码表
    huffmanHead header;
    fileIn.read((char*)&header, sizeof(header));
    string codeTable;
    char* temp = new char[header.tableSize];
    fileIn.read(temp, header.tableSize);
    codeTable = temp;
    delete[] temp;

    //构造hash编码表
    unordered_map<string, char> decodeMap;
    string code;
    for(char ch:codeTable)
    {
        if(ch == '0' || ch == '1')
        {
            code.push_back(ch);
        }
        else
        {
            decodeMap.emplace(code, ch);
            code = "";
        }
    }

    auto dataStart = fileIn.cur;
    fileIn.seekg(0, fileIn.end);
    int dataSize = fileIn.tellg();
    int outOfBuffer = dataSize % BUFFERSIZE;

    char* bufferIn = new char[BUFFERSIZE];
    string outputText;

    while(dataSize != 0)
    {
        
    }
}