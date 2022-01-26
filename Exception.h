//
// Created by maincpp on 26.01.2022.
//

#ifndef DEMOCPLUSPLUS_EXCEPTION_H
#define DEMOCPLUSPLUS_EXCEPTION_H
#include<exception>
#include<string>
#include<iostream>

class Exception: public std::exception {
public:
    friend void swap(Exception&, Exception&);
    Exception(int line, int index, std::string lexem, int type, std::string info, std::string file):
        line_(line), index_(index), type_(type), info_(std::move(info)), file_(std::move(file)), lexem_(std::move(lexem)) {};
    Exception(std::pair<int, int> where, std::pair<std::string, int> lexem, std::string info, std::string file);
    Exception(const Exception&);
    Exception& operator=(Exception other);

    void show();
    int getType();


private:
    int line_, index_, type_;
    std::string info_, file_, lexem_;
};


#endif //DEMOCPLUSPLUS_EXCEPTION_H
