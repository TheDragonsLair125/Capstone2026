//Main file for interpeter
//included headers
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "token.h"
#include "parser.h"
#include "expression.h"
//namespace
using namespace std;


//function declerations
vector<Token> Tokenizer(string filename);
void Cleaner(string& tempToken, vector<Token>& tokens, int line);
//float Math(vector<Token>& tokens, int& i, int j);

///////////////////////////////////////////////////////////////////////////////
// MAIN
///////////////////////////////////////////////////////////////////////////////
int main(){
    //variables
    vector<Token> tokens;
    string filename;
    Interpeter interpeter;

    cout << "Please input file name. \n";
    cin >> filename;

    tokens = Tokenizer(filename);

    Parser parser(tokens);

    vector<Stmt*> statements = parser.parse();

    //parser.printExpr(expression);
    for(Stmt* stmt : statements){
        interpeter.interpret(stmt);
    }
    
    //cout << tokens.size() << endl;
    
    //Parser(tokens);

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
    bool inString = false;
    bool inComment = false;

    //gets file
    textfile.open (filename);
    
    //file reading
    if (textfile.is_open()){
        //loop to read file char by char till end
        while( textfile.get(tempChar)){
            
            if(inComment == false){
                if(inString == false){
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
                    case '/':{
                        Cleaner(tempToken, tokens, line);
                        char nextChar = textfile.peek();

                        if(nextChar == '/'){
                            textfile.get(nextChar);
                            inComment = true;
                        }
                        else{
                        tokens.push_back(Token(TokenType::DIVIDE, "/", line));
                        }
                        break;
                    }
                    //come back to this later for double
                    case '=':{
                        Cleaner(tempToken, tokens, line);

                        char nextChar = textfile.peek();

                        if(nextChar == '='){

                            textfile.get(nextChar);
                            tokens.push_back(Token(TokenType::EQUAL_TO, "==", line));
                        }
                        else{
                        tokens.push_back(Token(TokenType::EQUALS, "=", line));
                        }

                        break;
                    }
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
            
                    case '<':{
                        Cleaner(tempToken, tokens, line);

                        char nextChar = textfile.peek();

                        if(nextChar == '<'){

                            textfile.get(nextChar);
                            tokens.push_back(Token(TokenType::OUTPUT, "<<", line));
                        }
                        else if(nextChar == '='){
                            textfile.get(nextChar);
                            tokens.push_back(Token(TokenType::LESS_THAN_EQUAL, "<=", line));
                        }
                        else{
                            tokens.push_back(Token(TokenType::LESS_THAN, "<", line));
                        }

                        break;
                    }

                    case '>':{
                        Cleaner(tempToken, tokens, line);

                        char nextChar = textfile.peek();

                        if(nextChar == '>'){

                            textfile.get(nextChar);
                            tokens.push_back(Token(TokenType::INPUT, ">>", line));
                        }
                        else if(nextChar == '='){
                            textfile.get(nextChar);
                            tokens.push_back(Token(TokenType::GREATER_THAN_EQUAL, ">=", line));
                        }
                        else{
                            tokens.push_back(Token(TokenType::GREATER_THAN, ">", line));
                        }

                        break;
                    }
           
                    case '"':
                        Cleaner(tempToken, tokens, line);
                        inString = true;
                        break;
                    
                    case '!':{
                        Cleaner(tempToken, tokens, line);

                        char nextChar = textfile.peek();

                        if(nextChar == '='){

                            textfile.get(nextChar);
                            tokens.push_back(Token(TokenType::NOT_EQUAL_TO, "!=", line));
                        }
                        else{
                        tokens.push_back(Token(TokenType::NOT, "!", line));
                        }

                        break;
                    }
            
                    //if no special characters means character is part of current token
                    default:
                    tempToken += tempChar;
                    }
                }
                else{
                    if(tempChar == '"'){
                        tokens.push_back(Token(TokenType::STRING, tempToken, line));
                        inString = false;
                        tempToken = "";
                    }

                    else{
                        tempToken += tempChar;
                    }
                }
            }
            else{
                if(tempChar == '\n'){
                    line++;
                    inComment = false;
                }
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
        exit(EXIT_FAILURE);
    }

    //cout << line << endl;
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
                if(tempToken == "int"){
                    tokens.push_back(Token(TokenType::INT, tempToken, line));
                }

                else if(tempToken == "nullptr"){
                    tokens.push_back(Token(TokenType::NIL, tempToken, line));
                }
                
                else if(tempToken == "cout"){
                    tokens.push_back(Token(TokenType::PRINT, tempToken, line));
                }

                else if(tempToken == "cin"){
                    tokens.push_back(Token(TokenType::CIN, tempToken, line));
                }

                else if(tempToken == "string"){
                    tokens.push_back(Token(TokenType::STRINGVAR, tempToken, line));
                }

                else if(tempToken == "bool"){
                    tokens.push_back(Token(TokenType::BOOL, tempToken, line));
                }

                else if(tempToken == "true"){
                    tokens.push_back(Token(TokenType::TRUE, tempToken, line));
                }

                else if(tempToken == "false"){
                    tokens.push_back(Token(TokenType::FALSE, tempToken, line));
                }

                else if(tempToken == "if"){
                    tokens.push_back(Token(TokenType::IF, tempToken, line));
                }

                else if(tempToken == "else"){
                    tokens.push_back(Token(TokenType::ELSE, tempToken, line));
                }

                else if(tempToken == "while"){
                    tokens.push_back(Token(TokenType::WHILE, tempToken, line));
                }

                else{
                    tokens.push_back(Token(TokenType::VAR, tempToken, line));
                }

            }
            else{
                tokens.push_back(Token(TokenType::NUMBER, tempToken, line));
            }
        }
    
    //starts new token by making it equal to null
    tempToken = "";
    return;
}
