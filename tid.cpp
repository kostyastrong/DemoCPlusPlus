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

int tid::checkid(const std::string& name) {
    auto res = mem_.find(name);
    if (res != mem_.end()) return res->second.line_;
    if (par_ == nullptr) return -1;
    else return par_->checkid(name);
}

void tid::noVar(const var& a) {
    int prev = checkid(a.name_);
    if (prev != -1) {
        std::cout << "Error on:" << a.line_ << ", prev. declaration on: " << prev;
        exit(0);
    }
}

void tid::tidChild(std::string name) {
    childs_.push_back(new tid(std::move(name), this));
}

tid::tid(std::string name, tid* par) {
    par_ = par;
    swap(name, name_);
}

var::var(int line, std::string type) {
    line_ = line;
    std::swap(type, type_);
}

var::var() {}

