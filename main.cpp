#include <iostream>
#include <cstdio>
#include "LexicalAnalyzator.h"
#include "SyntaxAnalyzator.h"

//LexicalAnalyzator worker(statesTransactions, statesType);  // why doesn't this work
std::string a = statesTransactions;

int main() {
    LexicalAnalyzator *worker = new LexicalAnalyzator(statesTransactions, statesType);
    SyntaxAnalyzator syntaxworker(worker);
    syntaxworker.mainLexer_->splitFile();
    std::cout << syntaxworker.work();
    return 0;
}
