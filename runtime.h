//runtime.h

#include<string>

enum ValueType{
    NUMBER_VAL,
    STRING_VAL,
    BOOL_VAL,
    NULL_VAL,
};

class RuntimeVal {
public:
    ValueType type;

    double numberVal;
    std::string stringVal;
    bool boolVal;

    RuntimeVal(){
        type = NULL_VAL;
    }
};