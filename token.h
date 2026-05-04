//This file serves as the storage and constructor for the custom token class I made.

enum TokenType{
    //single charcters
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    PLUS, MINUS, MULTIPLY, DIVIDE, SEMICOLON, EQUALS,

    //Others
    VAR, NUMBER, ENDFILE
};

class Token{
    public:
        TokenType type;
        string value;
        int line;
    
    Token(TokenType t, string v, int l){
        type = t;
        value = v;
        line = l;
    }
        
};