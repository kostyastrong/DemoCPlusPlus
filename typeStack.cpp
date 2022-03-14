//
// Created by maincpp on 14.03.2022.
//

#include "typeStack.h"


void typeStack::lastEquality(std::string &a, std::string &b) {
    if (a != b) {
        throw "Semantic error: operation with 2 dif. types on: " + mainLexer_->errCurLex();
        // i wouldn't be sure about position in the string here
    }
}

void typeStack::checkOp() {  // with replacing two els to one
    if (mem_.size() < 2) {
        throw "Programming error: less than 2 variables in the stack" + mainLexer_->errCurLex();
    }
    lastEquality(mem_.back(), mem_[mem_.size() - 2]);
    mem_.pop_back();
}
