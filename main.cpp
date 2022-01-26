#include <iostream>
#include <cstdio>
#include "LexicalAnalyzator.h"
#include "SyntaxAnalyzator.h"

//LexicalAnalyzator worker(statesTransactions, statesType);  // why doesn't this work
std::string a = statesTransactions;

int main() {
    std::cout << 1;
    std::cout << a << std::endl;
    a = statesTransactions;
    std::cout << a << std::endl;
    std::cout << "Hello, World!" << std::endl;
    //std::cout << statesTransactions;
    //LexicalAnalyzator::getChars(statesTransactions);
    LexicalAnalyzator *worker = new LexicalAnalyzator(statesTransactions, statesType);
    SyntaxAnalyzator syntaxworker(worker);
    syntaxworker.mainLexer_->splitFile();
    std::cout << syntaxworker.work();
    return 0;
}
