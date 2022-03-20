//
// Created by maincpp on 14.03.2022.
//

#include "typeStack.h"


void typeStack::lastEquality(std::string &a, std::string &b) {
    if (a != b) {  // we don't have casts for now
        throw "Semantic error: operation with 2 dif. types on: " + mainLexer_->errCurLex();
        // i wouldn't be sure about position in the string here
    }
}


void typeStack::checkOp() {  // with replacing two els to one
    if (mem_.size() < 2) {
        return;
        // throw "almost empty stack: " + mainLexer_->errCurLex();
    }
    std::string* operation = mem_[mem_.size() - 2];
    if (mem_.size() == 2 || *operation == "++" || *operation == "--" || *operation == "!") {
        checkBinOp();
    }
    if (mem_.size() < 3) {
        return;
        // throw "Programming error: less than 2 variables in the stack:" + mainLexer_->errCurLex();
    }
    lastEquality(*mem_.back(), *mem_[mem_.size() - 3]);
    if ((*operation != "+" && *operation != "=" && *operation != "!=" && *operation != "==")
        && *mem_.back() == "string") {
        throw "Unbelievable operation with strings:" + mainLexer_->errCurLex();
    }

    delete mem_.back();
    mem_.pop_back();

    delete mem_.back();
    mem_.pop_back();
}

void typeStack::pushop(std::string* a) {  // the main perat
    mem_.push_back(a);
}

void typeStack::pushop(std::string a) {
    mem_.push_back(new std::string(std::move(a)));
}

void typeStack::pushtype(std::string *a) {
    mem_.push_back(a);
    checkOp();
}

void typeStack::pushtype(std::string a) {
    mem_.push_back(new std::string(std::move(a)));
    checkOp();
}

typeStack::typeStack(LexicalAnalyzator *a): mainLexer_(a){
}

void typeStack::memClear() {
    mem_.clear();
}

void typeStack::checkBinOp() {
    if (mem_.size() < 2) return;
    std::string* a = mem_.back();
    std::string* operation = mem_[mem_.size() - 2];
    mem_.pop_back();
    delete mem_.back();
    mem_.pop_back();
    mem_.push_back(a);
    if (*a == "string") {
        throw "Unbelievable operation with strings:" + mainLexer_->errCurLex();
    }
}
