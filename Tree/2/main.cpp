#include "huffmanTree.h"
int main(int argc, char* argv[])
{
    if(argc == 1)
    {
        cout << "From file or encode string?" <<endl;
        cout << "[1] from file" << endl;
        cout << "[2] encode string" << endl;
        char in;
        cin >> in;
        if(in == '1')
        {
            ifstream fileIn;
            cout << "Please input the file name." << endl;
            string fileName;
            cin >> fileName;
            fileIn.open(fileName, ios::binary);
            huffmanTree test(fileIn);

            cout << "Code Table: "<< endl;
            test.showTable();

            try
            {
                cout << "----------Encoded-----------" << endl;
                test.showEncoded();
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
            
            cout << "Which file to save encoded file" << endl;
            string encodedFile;
            cin >> encodedFile;
            test.encode(encodedFile);

            cout << "Which file to decode" << endl;
            string decodeFile;
            cin >> decodeFile;
            decode(decodeFile, decodeFile+".recovered");
        }
        else if(in == '2')
        {
            string text;
            cout << "Please input string to encode" << endl;
            getchar();
            getline(cin, text);
            huffmanTree test(text);

            cout << "--------Code Table: ---------"<< endl;
            test.showTable();

            cout << "----------Encoded-----------" << endl;
            test.showEncoded();
            
        }
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