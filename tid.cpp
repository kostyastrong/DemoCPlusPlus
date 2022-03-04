//
// Created by maincpp on 28.02.2022.
//

#include "tid.h"

#include <utility>

void tid::insert(const var& a) {
    std::string name = a.name_;
    noVar(a);
    mem_[a.name_] = a;
}

void tid::del(const std::string& name) {
    auto res = mem_.find(name);
    if (res != mem_.end()) mem_.erase(res);
    if (par_ == nullptr) return;
    par_->del(name);
}

int tid::findVar(const std::string& name) {
    auto res = mem_.find(name);
    if (res != mem_.end()) return res->second.line_;
    if (par_ == nullptr) return -1;
    else return par_->findVar(name);
}

void tid::checkid(const var& a) {
    int prev = findVar(a.name_);
    if (prev == -1) {
        std::cout << "Using of undeclared variable\nError on the line: " << a.line_;
        exit(0);
    }
}

void tid::noVar(const var& a) {
    int prev = findVar(a.name_);
    if (prev != -1) {
        std::cout << "Error on:" << a.line_ << ", prev. declaration on: " << prev;
        exit(0);
    }
    if (par_ == nullptr) return;
    par_->noVar(a);
}

void tid::tidChild(std::string name="") {
    delete child;  // even with nullptr
    child = new tid(std::move(name), this);
    // childs_.push_back(new tid(std::move(name), this));
}

tid::tid(std::string name, tid* par) {
    par_ = par;
    swap(name, name_);
}

var::var(int line, int pos, std::string type, std::string name): line_(line), pos_(pos) {
    std::swap(type, type_);
    std::swap(name, name_);
}

var::var() {}

var::var(std::pair<int, int> where, std::string type, std::string name): line_(where.first), pos_(where.second) {
    std::swap(type, type_);
    std::swap(name, name_);
}


