//runtime.h

#include<string>

enum ValueType{
    NUMBER_VAL,
    STRING_VAL,
    BOOL_VAL,
    NIL_VAL,
};

class RuntimeVal {
public:
    ValueType type;

    double numberVal;
    std::string stringVal;
    bool boolVal;

    RuntimeVal(){
        type = NIL_VAL;
    }
};