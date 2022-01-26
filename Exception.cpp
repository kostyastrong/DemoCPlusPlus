//
// Created by maincpp on 26.01.2022.
//

#include "Exception.h"

void Exception::show() {
    std::cout << info_ << '\n';
    std::cout << "Lexem and type: " << lexem_ << ", " << type_ << '\n';
    std::cout << "Line and index: " << line_ << ", " << index_ << '\n';
}

Exception::Exception(std::pair<int, int> where, std::pair<std::string, int> lexem, std::string info, std::string file) {
    Exception tmp = Exception(where.first, where.second, lexem.first, lexem.second,
                              std::move(info), std::move(file));
    swap(*this, tmp);
}

Exception::Exception(const Exception & a) {
    info_ = a.info_;
    type_ = a.type_;
    index_ = a.index_;
    lexem_ = a.lexem_;
    file_ = a.file_;
    line_ = a.line_;
}

void swap(Exception & a, Exception & b) {
    using std::swap;
    swap(a.info_, b.info_);
    swap(a.index_, b.index_);
    swap(a.type_, b.type_);
    swap(a.lexem_, b.lexem_);
    swap(a.file_, b.file_);
    swap(a.line_, b.line_);
}

Exception &Exception::operator=(Exception other) {
    swap(*this, other);
    return *this;
}

int Exception::getType() {
    return type_;
}


