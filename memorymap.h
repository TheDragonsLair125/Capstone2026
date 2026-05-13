//memory map for variables
#ifndef MEMORYMAP_H_INCLUDED
#define MEMORYMAP_H_INCLUDED
#include <unordered_map>
#include <string>
#include <stdexcept>
#include "runtime.h"
#include "token.h"

class MemoryMap {
private:
    std::unordered_map<std::string, RuntimeVal> values;

public:
    void define(std::string name, RuntimeVal value){
        values[name] = value;
    }

    RuntimeVal get(Token name){
        if (values.find(name.value) != values.end()){
            return values[name.value];
        }

        else{
            return RuntimeVal();
        }
    }

};

#endif