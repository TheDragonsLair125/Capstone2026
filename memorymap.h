//memory map for variables
#ifndef MEMORYMAP_H_INCLUDED
#define MEMORYMAP_H_INCLUDED
#include <unordered_map>
#include <string>
#include <stdexcept>
#include "runtime.h"
#include "token.h"

struct Variable{
    ValueType declaredType;
    RuntimeVal value;
};

class MemoryMap {
private:
    std::unordered_map<std::string, Variable> values;

public:
    void define(std::string name, Variable value){
        values[name] = value;
    }

    RuntimeVal get(Token name){
        if (values.find(name.value) != values.end()){
            return values[name.value].value;
        }

        else{
            return RuntimeVal();
        }
    }

    Variable getVariable(Token name){
        if(values.find(name.value) == values.end()){
            throw std::runtime_error("Undefined variable: " + name.value + " at line: " + std::to_string(name.line));
        }
        
        Variable variable = values[name.value];

        return variable;
    } 
    

    void assign(Token name, RuntimeVal newValue){
        if(values.find(name.value) == values.end()){
            throw std::runtime_error("Undefined variable: " + name.value + " at line: " + std::to_string(name.line));
        }

        Variable& variable = values[name.value];

        if(variable.declaredType != newValue.type){
            throw std::runtime_error("Type mismatch assigning to variable: " + name.value + " at line: " + std::to_string(name.line));
        }

        variable.value = newValue;
        return;
    }

};

#endif