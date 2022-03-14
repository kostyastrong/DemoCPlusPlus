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
    std::deque<std::string> mem_;
private:
    LexicalAnalyzator* mainLexer_ = nullptr;
    void lastEquality(std::string &a, std::string &b);
    void checkOp();
};


#endif //DEMOCPLUSPLUS_TYPESTACK_H
