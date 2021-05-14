#include "huffmanTree.h"


int main(int argc, char* argv[])
{
    if(argc == 1)
    {
        ifstream fileIn;
        cout << "Please input the file name." << endl;
        string fileName;
        cin >> fileName;
        fileIn.open(fileName, ios::binary);
        cout << "opened" << endl;
        huffmanTree test(fileIn);
        cout << "constructed" << endl;

        cout << "Code Table: "<< endl;
        test.showTable();

        cout << "Which file to save encoded file" << endl;
        string encodedFile;
        cin >> encodedFile;
        test.encode(encodedFile);

        cout << "Which file to decode" << endl;
        string decodeFile;
        cin >> decodeFile;
        decode(decodeFile, decodeFile+".recovered");
    }
    
    else
    {
        ifstream fileIn;
        fileIn.open(argv[1], ios::binary);
        cout << "opened" << endl;
        huffmanTree test(fileIn);
        cout << "constructed" << endl;
        test.showTable();
        string name = argv[1];
        test.encode(name +".compressed");
        decode(name +".compressed", name+".recovered");
    }
    return 0;
}