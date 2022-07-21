//
// Created by Дарья Фомина on 25.03.2022.
//

#include "polish.h"

void polish::pushOp(element op) {
    rpn[free] = op;
    ++free;
}

void polish::putOp(element op, int place) {
    rpn[place] = op;
}

void polish::blank() {
    ++free;
}

bool polish::rightAssoc(std::string op) {
    if (op == "=" || op == "+=" || op == "-=" || op == "/=" || op == "%=")
        return true;
    return false;
}
//priority 12 - func call
//priority 13 - name - operation
//priority 14 - goto, goto_false
//priority 15 - for
void polish::pushStack(std::string op, int priority = -1) {
    std::cout << op << std::endl;
    for (auto elem : rpnStack) {
        std::cout << elem << " ";
    }
    std::cout << "||||||||" << std::endl;
    int cnt = 0;
    for (auto elem : rpn) {
        std::cout << cnt << " -- ";
        if (elem.what == 0) {
            std::cout << elem.operation << " ";
        } else if (elem.what == 1) {
            std::cout << elem.value << " ";
        } else {
            std::cout << elem.indx << " ";
        }
        ++cnt;
    }
    std::cout << "??????????" << std::endl;
    if (priority == 15) {
        while (rpnStack.back() != "(") {
            pushOp(getOperation(rpnStack.back()));
            rpnStack.pop_back();
        }
        pushOp(getOperation(op));
        return;
    }
    if (priority == 13 || priority == 14) {
        pushOp(getOperation(op));
        return;
    }
    if (priority == 12) {
        if (op == ";") {
            pushOp(getOperation(";"));
            return;
        }
        if (op == "(") {
            rpnStack.push_back(op);
            return;
        }
        if (op == ")") {
            while (rpnStack.back() != "(") {
                pushOp(getOperation(rpnStack.back()));
                rpnStack.pop_back();
            }
            rpnStack.pop_back();
            pushOp(getOperation(rpnStack.back()));
            rpnStack.pop_back();
            return;
        }
        if (op == ",") {
            while (rpnStack.back() != "(") {
                pushOp(getOperation(rpnStack.back()));
                rpnStack.pop_back();
            }
            return;
        }
        rpnStack.push_back(op);
        return;
    }
    if (op == "{" || op == "[" || op == "(") {
        rpnStack.push_back(op);
        return;
    }
    if (op == "]") {
        while (rpnStack.back() != "[") {
            pushOp(getOperation(rpnStack.back()));
            rpnStack.pop_back();
        }
        std::cout << rpnStack.back() << "111111" << std::endl;
        pushOp(getOperation(rpnStack.back()));
        rpnStack.pop_back();
        pushOp(getOperation(op));
        return;
    }
    if (op == "}") {
        //std::cout << "heeeeeeey" << std::endl;
        while (rpnStack.back() != "{") {
            pushOp(getOperation(rpnStack.back()));
            rpnStack.pop_back();
        }
        rpnStack.pop_back();
        return;
    }

    if (op == ")") {
        while (rpnStack.back() != "(") {
            pushOp(getOperation(rpnStack.back()));
            rpnStack.pop_back();
        }
        rpnStack.pop_back();
        return;
    }
    if (priority == 0 || priority == 13) {
        if (op == "??" || op == "@@" || priority == 13)  {
            pushOp(getOperation(op));
        } else {
            pushOp(getValue(op));
        }
        return;
    }
    if (priority == 11) {
        rpnStack.push_back(op);
        return;
    }
    if (op == ";" || op == ",") {
        //std::cout << "hello" << std::endl;
        while (rpnStack.back() != "{") {
            pushOp(getOperation(rpnStack.back()));
            rpnStack.pop_back();
        }
        pushOp(getOperation(op));
        return;
    }

    while (rpnStack.back() != "{" && (priority_table[rpnStack.back()] == 11 ||
    (rightAssoc(op) && priority_table[rpnStack.back()] > priority_table[op]) ||
    (!rightAssoc(op) && priority_table[rpnStack.back()] >= priority_table[op]))) {
        pushOp(getOperation(rpnStack.back()));
        rpnStack.pop_back();
    }
    rpnStack.push_back(op);

}

element polish::getIndex(int indx) {
    element ret;
    ret.indx = indx, ret.what = 2;
    return ret;
}

element polish::getOperation(std::string op) {
    element ret;
    ret.operation = op, ret.what = 0;
    return ret;
}

element polish::getValue(std::string value) {
    element ret;
    ret.value = value, ret.what = 1;
    return ret;
}


int polish::getFree() {
    return free;
}
