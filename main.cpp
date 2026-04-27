//Main file for interpeter
//included headers
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
//namespace
using namespace std;

//function declerations
vector<string> Tokenizer(string filename);
void Cleaner(string& tempToken, vector<string>& tokens);

///////////////////////////////////////////////////////////////////////////////
// MAIN
///////////////////////////////////////////////////////////////////////////////
int main(){
    //variables
    vector<string> tokens;
    string filename = "test.txt";

    tokens = Tokenizer(filename);
    
    cout << tokens.size();
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// Tokenizer: Tokenizes input file
///////////////////////////////////////////////////////////////////////////////
vector<string> Tokenizer(string filename){
    //variables
    ifstream textfile;
    string tempToken;
    vector<string> tokens;
    char tempChar;

    //gets file
    textfile.open (filename);
    
    //file reading
    if (textfile.is_open()){
        //loop to read file char by char till end
        while( textfile.get(tempChar)){
            
            //switch case to every special character that start would mean is a new token
            switch(tempChar){
            case ' ':
            case '\n':
                //in every switch statement makes sure blank tokens not added to vector
                Cleaner(tempToken, tokens);
                break;
            
            case ';':
                Cleaner(tempToken, tokens);
                //pushing back symbol as token as its what caused this case, used in all future statements
                tokens.push_back(";");
                break;

            case '+':
                Cleaner(tempToken, tokens);
                tokens.push_back("+");
                tempToken = "";
                break;
            
            case '-':
                Cleaner(tempToken, tokens);
                tokens.push_back("-");
                break;

            case '*':
                Cleaner(tempToken, tokens);
                tokens.push_back("*");
                break;

            //come back to this later for double
            case '/':
                Cleaner(tempToken, tokens);
                tokens.push_back("/");
                break;
            
            //come back to this later for double
            case '=':
                Cleaner(tempToken, tokens);
                tokens.push_back("=");
                break;

            case '(':
                Cleaner(tempToken, tokens);
                tokens.push_back("(");
                break;
            
            case ')':
                Cleaner(tempToken, tokens);
                tokens.push_back(")");
                break;
            
            case '{':
                Cleaner(tempToken, tokens);
                tokens.push_back("{");
                break;
            
            case '}':
                Cleaner(tempToken, tokens);
                tokens.push_back("}");
                break;
            
            //if no special characters means character is part of current token
            default:
            tempToken += tempChar;
            }
        }

        //checks to see if current token is empty to make sure no accidental bloat at end of file
        Cleaner(tempToken, tokens);

        textfile.close();
    }
    //file failed to load
    else{
        cout << "Invalid File";
    }

    return tokens;
    
}

///////////////////////////////////////////////////////////////////////////////
// Cleaner: helps empty tokens before pushing into vector
///////////////////////////////////////////////////////////////////////////////
void Cleaner(string& tempToken, vector<string>& tokens){
    //checks if token is empty if not pushes onto stack
    if(!tempToken.empty()){
             tokens.push_back(tempToken);
        }
    
    //starts new token by making it equal to null
    tempToken = "";

    return;
}