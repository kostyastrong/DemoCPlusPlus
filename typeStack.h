//
// Created by maincpp on 14.03.2022.
//

#ifndef DEMOCPLUSPLUS_TYPESTACK_H
#define DEMOCPLUSPLUS_TYPESTACK_H
#include "LexicalAnalyzator.h"
#include <deque>
#include <string>



class typeStack {
public:
    void memClear();
    typeStack(LexicalAnalyzator*);
    std::deque<std::string*> mem_;  // string pointers is faster
    void pushtype(std::string*);
    void pushtype(std::string);
    void pushop(std::string*);
    void pushop(std::string);
    void checkOp();
    void checkBinOp();  // binary operations

private:
    LexicalAnalyzator* mainLexer_ = nullptr;
    void lastEquality(std::string &a, std::string &b);
};


#endif //DEMOCPLUSPLUS_TYPESTACK_H
