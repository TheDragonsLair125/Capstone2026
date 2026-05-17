//memory map for variables
#ifndef MEMORYMAP_H_INCLUDED
#define MEMORYMAP_H_INCLUDED
#include <unordered_map>
#include <string>
#include <stdexcept>
#include "runtime.h"
#include "token.h"

//custom variable type to keep track of variable type preventing python like variables
struct Variable{
    ValueType declaredType;
    RuntimeVal value;
};

class MemoryMap {
private:
// map used to store data
    std::unordered_map<std::string, Variable> values;

public:
    //creates new variable
    void define(std::string name, Variable value){
        values[name] = value;
    }

    //gets variable value and returns it including nulls if is empty
    RuntimeVal get(Token name){
        if (values.find(name.value) != values.end()){
            return values[name.value].value;
        }

        else{
            return RuntimeVal();
        }
    }

    //function for getting a variable and current value for use in expressions / statements
    Variable getVariable(Token name){
        //if is undeclared errors
        if(values.find(name.value) == values.end()){
            throw std::runtime_error("Undefined variable: " + name.value + " at line: " + std::to_string(name.line));
        }
        
        Variable variable = values[name.value];

        return variable;
    } 
    
    //assignment function for updating variable value
    void assign(Token name, RuntimeVal newValue){
        //makes sure variable actually exists
        if(values.find(name.value) == values.end()){
            throw std::runtime_error("Undefined variable: " + name.value + " at line: " + std::to_string(name.line));
        }

        Variable& variable = values[name.value];

        //makes sure cant make a string equal an int and vice versa same with bools
        if(variable.declaredType != newValue.type){
            throw std::runtime_error("Type mismatch assigning to variable: " + name.value + " at line: " + std::to_string(name.line));
        }

        variable.value = newValue;
        return;
    }

};

#endif