//
// Created by maincpp on 21.12.2021.
//

#ifndef COMPILER_LEXICALANALYZATOR_H
#define COMPILER_LEXICALANALYZATOR_H
#include <string>
#include <unordered_set>
#include <fstream>
#include <iostream>
#include <utility>

// namespace for these const

extern const std::string LEXPATH;
extern const std::string reservedDefPath;
extern const std::string inputDefPath;
extern const std::string outputDefPath ;
extern const std::string devidersDefPath;
extern const std::string statesType;
extern const std::string statesTransactions;
extern const int stateNotExist;
extern const int stateNotEnd;

class State {
public:
    int type_;
    State* jump_[256];
};

class Cell { //delete used
public:
    Cell* next = nullptr;
    Cell* prev = nullptr;
    std::string lexem_;
    int t_ = -1;
    Cell(std::string lexem, int t, int line, int index): lexem_(std::move(lexem)), t_(t), line_(line), index_(index){};
    std::pair<int, int> where();
private:
    int line_, index_;
};

class LexicalAnalyzator {
public:
    static std::unordered_set<std::string>* getWords(const std::string& reserved=reservedDefPath);
    static std::unordered_set<char>* getChars(const std::string& reserved);
    void fillStates(const std::string& source=statesTransactions);
    void fillTypes(const std::string& source=statesType);
    void splitFile(const std::string& reserved=reservedDefPath,
                   const std::string& inPath=inputDefPath,
                   const std::string& outPath=outputDefPath);
    void retLex();


    LexicalAnalyzator(const std::string& sourceTrans, const std::string& sourceTypes);
    std::pair<std::string, int> getLexem();
    std::pair<std::string, int> movLexem();
    Cell* getCurrentLexCell();
    void jumpToCell(Cell* memorizedCell);
    void pbInMemory(Cell*);

private:
    int curType_ = 0;
    int ind_ = 0;
    std::string curString_;
    std::string lexem_;
    State* states_[64];
    //int states[64][256];

    Cell* memory_ = nullptr;
    Cell* lastWrite_ = nullptr;
    int statesType_[64]{};

    std::unordered_set<std::string>* reservedWords_;
    std::unordered_set<char>* deviders_;

};


#endif //COMPILER_LEXICALANALYZATOR_H