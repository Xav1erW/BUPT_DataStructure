#include "huffmanTree.h"


int main()
{
    ifstream fileIn;
    fileIn.open("text", ios::binary);
    cout << "opened" << endl;
    huffmanTree test(fileIn);
    cout << "constructed" << endl;
    test.showTable();
    test.showEncoded();
    test.encode("text.compressed");
    decode("text.compressed", "text.recovered");
    return 0;
}