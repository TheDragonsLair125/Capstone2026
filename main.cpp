//Main file for interpeter
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main(){
    //variables
    string tempToken;
    char tempChar;
    vector<string> tokens;
    ifstream textfile;

    //gets file
    textfile.open ("test.txt");
    
    //file reading
    if (textfile.is_open()){
        while( textfile.get(tempChar)){
            
            switch(tempChar){
            case ' ':
                cout << tempToken;
                tempToken = "";

            case '\n':
                cout << tempToken;
                tempToken = "";
            
            default:
            tempToken += tempChar;
            }
        }

        cout << tempToken;

        textfile.close();
    }

    else{
        cout << "Invalid File";
    }

    return 0;
}