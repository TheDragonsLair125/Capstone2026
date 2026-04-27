//Main file for interpeter
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(){
    string line;
    ifstream textfile;
    textfile.open ("test.txt");
    
    if (textfile.is_open()){
        while( getline(textfile,line)){
            cout << line << '\n';
        }
        textfile.close();
    }

    else{
        cout << "Invalid File";
    }

    return 0;
}