#include "huffmanTree.h"


int main(int argc, char* argv[])
{
    if(argc == 1)
    {
        ifstream fileIn;
        fileIn.open("Pic.bmp", ios::binary);
        cout << "opened" << endl;
        huffmanTree test(fileIn);
        cout << "constructed" << endl;
        test.showTable();
        test.encode("pic.compressed");
        decode("pic.compressed", "pic.recovered");
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