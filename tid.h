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
    bool isfunc = false;
    int funcaddr = -1;
    explicit var(int line, int pos, std::string type, std::string name, bool func);
    explicit var(std::pair<int, int> where, std::string type, std::string name, bool func);
    var();
};

class tid {
public:
    std::string name_;
    explicit tid(std::string name, tid* par = nullptr);

    void insert(const var& a);
    void del(const std::string& name);
    var* findVar(const std::string& name);  // as for now, we don't support multinaming
    std::string checkid(const var&);
    void noVarTree(const var&);
    void noVarCur(const var&);
    void tidChild(std::string name);
    tid* child = nullptr;
    tid* par_ = nullptr;  // to get upper

private:
    std::unordered_map<std::string, var> mem_;
    // std::vector<tid*> childs_;  // in case we have objects, we need a few tids
};

#endif //DEMOCPLUSPLUS_TID_H
