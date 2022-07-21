//
// Created by Дарья Фомина on 25.03.2022.
//

#ifndef DEMOCPLUSPLUS_POLISH_H
#define DEMOCPLUSPLUS_POLISH_H
#include <vector>
#include <deque>
#include <string>
#include <map>
#include <iostream>

struct element{
    std::string operation;
    std::string value;
    //std::string link;
    int indx;
    int what;
};

class polish {
public:
    std::deque<std::string> rpnStack;
    std::deque<element> rpn;
    std::map<std::string, int> priority_table;
    polish() {
        rpn.resize(200);
        //rpnStack.push_back("{");
        priority_table["_"] = 11, priority_table["++"] = 11, priority_table["--"] = 11, priority_table["!"] = 11;
        priority_table["*"] = 10, priority_table["/"] = 10, priority_table["%"] = 10;
        priority_table["+"] = 9, priority_table["-"] = 9;
        priority_table["<"] = 8, priority_table["<="] = 8, priority_table[">"] = 8, priority_table[">="] = 8;
        priority_table["=="] = 7, priority_table["<>"] = 7;
        priority_table["&"] = 6, priority_table["|"] = 5, priority_table["^"] = 4, priority_table["&&"] = 3;
        priority_table["||"] = 2, priority_table["="] = 1, priority_table["+="] = 1, priority_table["*="] = 1;
        priority_table["/="] = 1, priority_table["%="] = 1;
    }
    int free = 0;
    void pushOp(element op);
    void putOp(element op, int place);
    void blank();
    void pushStack(std::string op, int priority);
    element getOperation(std::string op);
    element getValue(std::string value);
    element getIndex(int indx);
    bool rightAssoc(std::string op);
    int getFree();

};


#endif //DEMOCPLUSPLUS_POLISH_H
