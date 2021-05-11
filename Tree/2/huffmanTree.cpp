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
    for (char ch = 0; ch < 127; ch++)
    {
        if (freqTable[ch] != 0)
        {
            ch_count++;
            node* add = new node(ch, freqTable[ch]);
            sortedStr.emplace(add);
        }
    }


    while (!sortedStr.empty())
    {
        node* lchild = sortedStr.top();
        sortedStr.pop();
        node* rchild = sortedStr.top();
        sortedStr.pop();
        if (sortedStr.empty())              //队列空，没有剩余节点，直接创建最后的根节点
        {
            root = new node(-1, lchild->freq + rchild->freq, lchild, rchild);
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
    new (this)huffmanTree(data);
}

void huffmanTree::generateTable(const node *r, string binCode)
{
    // cout << binCode << endl;

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

// void huffmanTree::generateTable(const node *r, string binCode)
// {

// }


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
    cout << result << endl;    
}


void huffmanTree::encode(string address)
{
    ofstream file(address, ios::binary);
    file.seekp(sizeof(huffmanHead));
    huffmanHead header;
    // header.codeMapLength = encodeMap.size();
    for (auto it = decodeMap.begin(); it != decodeMap.end(); it++)
    {
        header.tableSize += it->first.length() + 1;
        file.write(it->first.c_str(), it->first.length());
        file << it->second;
    }

    string code = "";
    for (char ch : text)
    {
        code += encodeMap[ch];
        // if (code.length() + encodeMap[ch].length() <= BUFFERSIZE)
        // {
        //     code += encodeMap[ch];
        // }
        // else
        // {
        //     //如果buffer存满了，开始写入
        //     int len = BUFFERSIZE - code.length();
        //     code += encodeMap[ch].substr(0, len);

            // code = "";
            // code += encodeMap[ch].substr(len);
            // cout << "buffer full" << endl;
        // }
    }
    cout << "code:" << code << endl;
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

    // //保存最后未存满一个buffer的数据
    // int counter = 0;
    // for (auto it = code.begin(); it != code.end() - header.remainedBits; it++)
    // {
    //     //按字节写入文件
    //     if (counter%8 == 0 && counter!= 0)
    //     {
    //         file.write((char *)&byteBuffer, 1);
    //         byteBuffer = 0;
    //         byteBuffer << 1;
    //         byteBuffer += *it - '0';
    //         counter++;
    //         header.dataSize ++;
    //     }
    //     else
    //     {
    //         byteBuffer << 1;
    //         byteBuffer += *it - '0';
    //         counter++;
    //     }
    // }
    // //写入剩余的buffer内容
    // file.write((char *)&byteBuffer, 1);
    // header.dataSize++;

    // byteBuffer = 0;
    // for (auto it = code.end() - header.remainedBits; it != code.end(); it++)
    // {
    //     byteBuffer << 1;
    //     byteBuffer += *it;
    // }
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
        if(char search = decodeMap.find(charCode) != decodeMap.end())
        {
            cout << search;
            charCode = "";
        }        
    }
    cout << charCode << endl;
}

void decode(string in, string out)
{
    ifstream fileIn;
    ofstream fileOut;
    fileIn.open(in, ios::binary);
    fileOut.open(out);

    //读取文件头和编码表
    huffmanHead header;
    fileIn.read((char*)&header, sizeof(header));
    string codeTable;
    char* temp = new char[header.tableSize];
    fileIn.read(temp, header.tableSize);
    codeTable = temp;
    delete[] temp;

    cout << header.remainedBits << "\t" << header.tableSize << endl;

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

    // auto dataStart = fileIn.tellg();
    // fileIn.seekg(0, fileIn.end);
    // int dataSize = fileIn.tellg()-dataStart;      //数据字节数
    // int outOfBuffer = dataSize % BUFFERSIZE;
    // fileIn.seekg(dataStart);        //回到数据开始的地方

    char* dataIn = new char[header.dataSize];
    string outputText;
    unsigned char mask = 128;           //二进制10000000，用于利用and取最高位
    code = "";
    // int dataLeft = header.dataSize;               //还未读入的字节数
    // while(dataLeft != 0)
    
    fileIn.read(dataIn, header.dataSize);
    // dataLeft = 0;
    int i = 0;
    int bitCount = 0;               //记录已经读取的bit数，每8位读取下一字节
    unsigned char byte = 0;
    while (i != header.dataSize-1)             //逐个读取dataIn中的数据
    {
        if(bitCount % 8 == 0)
        {
            byte = dataIn[i];
            i++;
            char bitByChar = ((byte&mask) >> 7) + '0';   //用字符‘0’‘1’存储单个bit
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
            char bitByChar = ((byte&mask) >> 7) + '0';   //用字符‘0’‘1’存储单个bit
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
    byte = dataIn[header.dataSize-1];
    byte << 8-header.remainedBits;
    bitCount = 8-header.remainedBits;
    while(bitCount != 8)
    {
        char bitByChar = ((byte&mask) >> 7)+'0';
        byte <<= 1;
        bitCount++;
        code.push_back(bitByChar);
        if(decodeMap.find(code) != decodeMap.end())     //在hash表中找到存在的对应字符
        {
            outputText.push_back(decodeMap[code]);
            code = "";              //code置零，读取下一个编码准备
        }
    }
    
        // if(dataSize == outOfBuffer)
        // {
        //     fileIn.read(bufferIn, outOfBuffer);
        //     dataSize = 0;
        //     int i = 0;
        //     int bitCount = 0;               //记录已经读取的bit数，每8位读取下一字节
        //     unsigned char byte = 0;
        //     while (i != outOfBuffer-1)             //逐个读取buffer中的数据
        //     {
        //         if(bitCount % 8 == 0)
        //         {
        //             byte = bufferIn[i];
        //             i++;
        //             char bitByChar = byte&&mask + '0';   //用字符‘0’‘1’存储单个bit
        //             byte << 1;
        //             bitCount ++;
        //             code.push_back(bitByChar);
        //             if(decodeMap.find(code) != decodeMap.end())      //在hash表中找到存在的对应字符
        //             {
        //                 outputText.push_back(decodeMap[code]);
        //                 code = "";              //code置零，读取下一个编码准备
        //             }
        //         }
        //         else
        //         {
        //             char bitByChar = byte&&mask + '0';   //用字符‘0’‘1’存储单个bit
        //             byte << 1;
        //             bitCount ++;
        //             code.push_back(bitByChar);
        //             if(decodeMap.find(code) != decodeMap.end())     //在hash表中找到存在的对应字符
        //             {
        //                 outputText.push_back(decodeMap[code]);
        //                 code = "";          //code置零，读取下一个编码准备
        //             }
        //         }
        //     }
        //     //读取不足一字节的数据
        //     byte = bufferIn[outOfBuffer-1];
        //     byte << 8-header.remainedBits;
        //     bitCount = 8-header.remainedBits;
        //     while(bitCount != 8)
        //     {
        //         char bitByChar = byte&&mask+'0';
        //         byte << 1;
        //         bitCount++;
        //         code.push_back(bitByChar);
        //         if(decodeMap.find(code) != decodeMap.end())     //在hash表中找到存在的对应字符
        //         {
        //             outputText.push_back(decodeMap[code]);
        //             code = "";              //code置零，读取下一个编码准备
        //         }
        //     }
        // }
        // else
        // {
        //     fileIn.read(bufferIn, BUFFERSIZE);
        //     dataSize -= BUFFERSIZE;
        //     int i = 0;
        //     int bitCount = 0;               //记录已经读取的bit数，每8位读取下一字节
        //     unsigned char byte = 0;
        //     while (i != BUFFERSIZE)             //逐个读取buffer中的数据
        //     {
        //         if(bitCount % 8 == 0)
        //         {
        //             byte = bufferIn[i];
        //             i++;
        //             char bitByChar = byte&&mask + '0';   //用字符‘0’‘1’存储单个bit
        //             byte << 1;
        //             bitCount ++;
        //             code.push_back(bitByChar);
        //             if(decodeMap.find(code) != decodeMap.end())      //在hash表中找到存在的对应字符
        //             {
        //                 outputText.push_back(decodeMap[code]);
        //                 code = "";              //code置零，读取下一个编码准备
        //             }
        //         }
        //         else
        //         {
        //             char bitByChar = byte&&mask + '0';   //用字符‘0’‘1’存储单个bit
        //             byte << 1;
        //             bitCount ++;
        //             code.push_back(bitByChar);
        //             if(decodeMap.find(code) != decodeMap.end())     //在hash表中找到存在的对应字符
        //             {
        //                 outputText.push_back(decodeMap[code]);
        //                 code = "";              //code置零，读取下一个编码准备
        //             }
        //         }
        //     }
        // }
    
    delete[] dataIn;
    cout << outputText << endl;
    fileOut << outputText;
}