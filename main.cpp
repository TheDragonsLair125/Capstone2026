//Main file for interpeter
//included headers
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <token.h>
//namespace
using namespace std;


//function declerations
vector<Token> Tokenizer(string filename);
void Cleaner(string& tempToken, vector<Token>& tokens, int line);
void Parser(vector<Token> tokens);
//float Math(vector<Token>& tokens, int& i, int j);

///////////////////////////////////////////////////////////////////////////////
// MAIN
///////////////////////////////////////////////////////////////////////////////
int main(){
    //variables
    vector<Token> tokens;
    string filename = "test.txt";

    tokens = Tokenizer(filename);
    
    cout << tokens.size() << endl;
    
    Parser(tokens);

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// Tokenizer: Tokenizes input file
///////////////////////////////////////////////////////////////////////////////
vector<Token> Tokenizer(string filename){
    //variables
    int line = 1;
    ifstream textfile;
    string tempToken;
    vector<Token> tokens;
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
                Cleaner(tempToken, tokens, line);
                break;

            case '\n':
                //in every switch statement makes sure blank tokens not added to vector
                Cleaner(tempToken, tokens, line);
                line++;
                break;
            
            case ';':
                Cleaner(tempToken, tokens, line);
                //pushing back symbol as token as its what caused this case, used in all future statements
                tokens.push_back(Token(TokenType::SEMICOLON, ";", line));
                break;

            case '+':
                Cleaner(tempToken, tokens, line);
                tokens.push_back(Token(TokenType::PLUS, "+", line));
                break;
            
            case '-':
                Cleaner(tempToken, tokens, line);
                tokens.push_back(Token(TokenType::MINUS, "-", line));
                break;

            case '*':
                Cleaner(tempToken, tokens, line);
                tokens.push_back(Token(TokenType::MULTIPLY, "*", line));
                break;

            //come back to this later for double
            case '/':
                Cleaner(tempToken, tokens, line);
                tokens.push_back(Token(TokenType::DIVIDE, "/", line));
                break;
            
            //come back to this later for double
            case '=':
                Cleaner(tempToken, tokens, line);
                tokens.push_back(Token(TokenType::EQUALS, "=", line));
                break;

            case '(':
                Cleaner(tempToken, tokens, line);
                tokens.push_back(Token(TokenType::LEFT_PAREN, "(", line));
                break;
            
            case ')':
                Cleaner(tempToken, tokens, line);
                tokens.push_back(Token(TokenType::RIGHT_PAREN, ")", line));
                break;
            
            case '{':
                Cleaner(tempToken, tokens, line);
                tokens.push_back(Token(TokenType::LEFT_BRACE, "{", line));
                break;
            
            case '}':
                Cleaner(tempToken, tokens, line);
                tokens.push_back(Token(TokenType::RIGHT_BRACE, "}", line));
                break;
            
            //if no special characters means character is part of current token
            default:
            tempToken += tempChar;
            }
        }

        //checks to see if current token is empty to make sure no accidental bloat at end of file
        Cleaner(tempToken, tokens, line);
        tokens.push_back(Token(TokenType::ENDFILE, "", line));

        textfile.close();
    }
    //file failed to load
    else{
        cout << "Invalid File";
    }

    cout << line << endl;
    return tokens;
    
}

///////////////////////////////////////////////////////////////////////////////
// Cleaner: helps empty tokens before pushing into vector
///////////////////////////////////////////////////////////////////////////////
void Cleaner(string& tempToken, vector<Token>& tokens, int line){
    //checks if token is empty if not pushes onto stack
    bool hasChar = false;

    if(!tempToken.empty()){
            
            for(int i = 0; i < tempToken.length(); i++){
                if(!isdigit(tempToken[i]) && tempToken[i] != '.'){
                    hasChar = true;
                }
            }

            if(hasChar == true){
                tokens.push_back(Token(TokenType::VAR, tempToken, line));
                cout << "Is variable"<< endl;
            }
            else{
                tokens.push_back(Token(TokenType::NUMBER, tempToken, line));
                cout << "Is Number"<< endl;
            }
        }
    
    //starts new token by making it equal to null
    tempToken = "";
    return;
}
