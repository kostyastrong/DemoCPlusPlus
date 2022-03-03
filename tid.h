//
// Created by maincpp on 28.02.2022.
//

#ifndef DEMOCPLUSPLUS_TID_H
#define DEMOCPLUSPLUS_TID_H
#include<unordered_map>
#include<string>
#include<vector>
#include<iostream>


struct var{
    std::string type_, name_;
    int line_ = -1, pos_ = -1;
    explicit var(int line, int pos, std::string type, std::string name);
    explicit var(std::pair<int, int> where, std::string type, std::string name);
    var();
};


class tid {
public:
    std::string name_;
    explicit tid(std::string name, tid* par = nullptr);

    void insert(const var& a);
    void del(const std::string& name);
    int checkid(const std::string& name);  // as for now, we don't support multinaming
    void noVar(const var&);
    void tidChild(std::string name);

private:
    std::unordered_map<std::string, var> mem_;
    std::vector<tid*> childs_;  // in case we have objects, we need a few tids
    tid* par_ = nullptr;  // to get upper
};


#endif //DEMOCPLUSPLUS_TID_H
