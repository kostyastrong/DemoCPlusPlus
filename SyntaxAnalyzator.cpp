//
// Created by maincpp on 21.01.2022.
//

#include "SyntaxAnalyzator.h"

using namespace std::literals::string_literals;

tid* curtid = new tid("global");
typeStack* opStack = nullptr;  // the creation in syntaxAnalyzer::syntaxAnalyzer
bool describingFunc = false;
std::string describingFuncName = "";

void enterScope(tid*& cur=curtid, std::string name=""){
    cur->tidChild(name);
    cur = cur->child;
}

void exitScope(tid*& cur=curtid) {
    cur = cur->par_;
}

bool isAssignment(std::string& a) {
    return a == "=" || a == "+=" || a == "/=" || a == "*=" || a == "-=";
}

std::pair<std::string, int> SyntaxAnalyzator::movLexem() {
    return mainLexer_->movLexem();
}
std::pair<std::string, int> SyntaxAnalyzator::getLexem() {
    return mainLexer_->getLexem();
}

std::string SyntaxAnalyzator::errLastLex() const {
    return mainLexer_->errLastLex();
}

std::string SyntaxAnalyzator::errCurLex() const {
    return mainLexer_->errCurLex();
}

bool SyntaxAnalyzator::isType() {
    auto [cur, num] = getLexem();
    if (cur != "int" && cur != "char" && cur != "double" && cur != "float" && cur != "bool" && cur != "string")
        return false;
    return true;
}

bool SyntaxAnalyzator::isBool() {
    auto [cur, num] = getLexem();
    if (cur != "true" && cur != "false") {
        return false;
    }
    return true;
}

bool SyntaxAnalyzator::isConstNumber() {
    auto [cur, num] = getLexem();
    if (num != 3 || cur[0] > '9' || cur[0] < '0') {
        return false;
    }
    return true;
}

bool SyntaxAnalyzator::isConstString() {
    auto [cur, num] = getLexem();
    if (num != 3 || (cur[0] <= '9' && cur[0] >= '0')) {
        return false;
    }
    return true;
}

bool SyntaxAnalyzator::isConst() {
    return isConstNumber() || isConstString();
}
bool SyntaxAnalyzator::isSign() {
    auto [cur, num] = getLexem();
    if (cur != "+" && cur != "-") {
        return false;
    }
    if (cur == "+") return true;  // how to act with strings?
    opStack->pushop(new std::string("_"));  // binary minus
    return true;
}

bool SyntaxAnalyzator::isIncrement() {
    auto [cur, num] = getLexem();
    if (cur != "--" && cur != "++") {
        return false;
    }
    opStack->pushop(new std::string(std::move(cur)));
    return true;
}

bool SyntaxAnalyzator::isOperation1() {
    auto [cur, num] = getLexem();
    if (cur != "*" && cur != "/" && cur != "%")
        return false;
    opStack->pushop(new std::string(std::move(cur)));
    return true;
}

bool SyntaxAnalyzator::isOperation2() {
    auto [cur, num] = getLexem();
    if (cur != "+" && cur != "-")
        return false;
    opStack->pushop(new std::string(std::move(cur)));
    return true;
}

bool SyntaxAnalyzator::isOperation3() {
    auto [cur, num] = getLexem();
    if (cur != "<" && cur != ">" && cur != "<=" && cur != ">=")
        return false;
    opStack->pushop(new std::string(std::move(cur)));
    return true;
}

bool SyntaxAnalyzator::isOperation4() {
    auto [cur, num] = getLexem();
    if (cur != "==" && cur != "<>")
        return false;
    opStack->pushop(new std::string(std::move(cur)));
    return true;
}


bool SyntaxAnalyzator::isDelimiter() {
    auto [cur, num] = getLexem();
    if (cur != "\n" && cur != "\' \'")
        return false;
    return true;
}

bool SyntaxAnalyzator::isName(bool declar=false, std::string t="", bool putInStack = false, bool func = false) {  // why for this implementation when we have num
    auto [cur, num] = getLexem();
    if (num != 2) {
        return false;
    }
    bool check_first = cur[0] == '_' || (cur[0] >= 'a' && cur[0] <= 'z') ||
            (cur[0] >= 'A' && cur[0] <= 'Z');
    bool check_other = true;
    for (int i = 1; i < static_cast<int>(cur.size()); ++i) {
        if ((cur[i] >= '0' && cur[i] <= '9') || (cur[i] >= 'a' && cur[i] <= 'z') ||
        (cur[i] >= 'A' && cur[i] <= 'Z')) {
            continue;
        }
        check_other = false;
    }
    if (!(check_first && check_other))
        return false;

    var a = var(curWhere(), std::move(t), cur, func);
    if (declar) {
        if (func) {
            if (describingFunc) {
                var* _ = curtid->findVar(describingFuncName);
                if (_->type_ != "void" && describingFuncName != "main") {
                    throw "The previous function should return: " + _->type_ + "\n but doesn't return any";
                }
            }
            describingFuncName = cur;
            describingFunc = true;
        }
        curtid->insert(a);
        if (putInStack) opStack->pushtype(new std::string(a.type_));
    } else {
        std::string* _ = new std::string(std::move(curtid->checkid(a)));
        if (putInStack) opStack->pushtype(_);
    }
    return true;
}


bool SyntaxAnalyzator::stReturnOperator() {
    auto [cur, num] = movLexem();
    if (cur != "return") {
        throw "Syntax error: expected return, but "s + (num ? cur : "nothing"s) + "is found\n"s + errCurLex();
    }
    std::tie(cur, num) = getLexem();
    var* a = curtid->findVar(describingFuncName);
    if (cur == ";") {
        if (a->type_ != "void" && describingFuncName != "main") {
            throw "The current function should return: " + a->type_ + "\n but doesn't return any" + errCurLex();
        }

        movLexem();
        return true;
    }
    opStack->pushtype(a->type_);
    opStack->pushop("=");



    if (stExpression()) {
        std::tie(cur, num) = movLexem();
        if (cur == ";") {
            describingFunc = false;
            opStack->checkOp();
            return true;
        } else {
            throw "Syntax error: expected ; after return operator, but "s + (num ? cur : "nothing"s) + "is found\n"s + errCurLex();
        }
    }
    throw "Syntax error: expected simple return or expression in return but neither is found\n"s + errCurLex();
}

bool SyntaxAnalyzator::stInputOperator() {
    auto [cur, num] = movLexem();
    if (cur != "cin") {
        throw "Syntax error: expected cin in input operator\n"s + errCurLex();
    }
    std::tie(cur, num) = movLexem();
    if (cur != ">>") {
        throw "Syntax error: expected >> in input operator\n"s + errCurLex();
    }
    if (!isName()) {
        throw "Syntax error: expected name in input operator\n"s + errCurLex();
    } else {
        movLexem();
    }
    while (true) {
        std::tie(cur, num) = movLexem();
        if (cur == ";") {
            break;
        }
        if (cur != ">>") {
            throw "Syntax error: expected >> in input operator\n"s + errCurLex();
        }
        if (!isName()) {
            throw "Syntax error: expected name in input operator\n"s + errCurLex();
        } else {
            movLexem();
        }
    }
    return true;
}

bool SyntaxAnalyzator::stOutputOperator() {
    auto [cur, num] = movLexem();
    if (cur != "cout") {
        throw "Syntax error: expected cout in output operator\n"s + errCurLex();
    }
    std::tie(cur, num) = movLexem();
    if (cur != "<<") {
        throw "Syntax error: expected << in output operator\n"s + errCurLex();
    }
    if (!isDelimiter()) {
        if (!stExpression()) {
            throw "Syntax error: expected expression or delimiter in output operator but neither is found\n"s + errCurLex();
        } else { // Expression
        }
    } else { // Delimeter
        movLexem();
    }
    while (true) {
        opStack->memClear();
        std::tie(cur, num) = movLexem();
        if (cur == ";") {
            return true;
        }
        if (cur != "<<") {
            throw "Syntax error: expected << in output operator\n"s + errCurLex();
        }
        if (!isDelimiter()) {
            if (!stExpression()) {
                throw "Syntax error: expected expression or delimiter in output operator but neither is found\n"s + errCurLex();
            } else { // Expression
            }
        } else { // Delimeter
            movLexem();
        }
    }
}

bool SyntaxAnalyzator::stBreakOperator() {
    auto [cur, num] = movLexem();
    if (cur != "break") {
        throw "Syntax error: expected break\n"s + errCurLex();
    }
    std::tie(cur, num) = movLexem();
    if (cur != ";") {
        throw "Syntax error: expected ; after break\n"s + errCurLex();
    }
    return true;
}

bool SyntaxAnalyzator::stContinueOperator() {
    auto [cur, num] = movLexem();
    if (cur != "continue") {
        throw "Syntax error: expected continue\n"s + errCurLex();
    }
    std::tie(cur, num) = movLexem();
    if (cur != ";") {
        throw "Syntax error: expected ; after continue\n"s + errCurLex();
    }
    return true;
}


bool SyntaxAnalyzator::stInitAtom() {
    if (isConst()) {
        movLexem();
        return true;
    }
    if (isBool()) {
        movLexem();
        return true;
    }
    auto [cur, num] = movLexem();
    if (cur != "!") {
        throw "Syntax error: expected ! when tried to find initial atom but "s + (num ? cur : "nothing"s) + "is found\n"s + errCurLex();
    }
    if (stAtom())
        return true;
    // else exception
}

bool SyntaxAnalyzator::stAtom() {
    auto [cur, num] = getLexem();
    if (cur == "!" ) {
        if (stInitAtom()) {
            opStack->pushop(new std::string("!"));
            return true;
        } // else exception
    }
    if (isConstNumber()) {
        if (stInitAtom()) {
            opStack->pushtype(new std::string("int"));
            return true;
        }
    }
    if (isConstString()) {
        if (stInitAtom()) {
            opStack->pushtype(new std::string("string"));
            return true;
        }
    }
    if (isBool()) {
        if (stInitAtom()) {
            opStack->pushtype(new std::string("bool"));
            return true;
        }
    }
    if (cur == "(") {
        movLexem();
        if (stExpression()) {
            std::tie(cur, num) = movLexem();
            if (cur != ")") {
                throw "Syntax error: expected ) after expression\n"s + errCurLex();
            } else {
                return true;
            }
        } // else exception
    }
    if (isName()) {
        var a = var(curWhere(), "", cur, false);
        std::string funcName = getLexem().first;
        movLexem();
        std::tie(cur, num) = getLexem();
        if (cur == "(") {
            if (stFunctionTail()) {
                var* _ = curtid->findVar(funcName);
                if (!_) {
                    throw "Semantic error: No declared before: " + errCurLex();
                }
                if (_->isfunc) {
                    opStack->pushtype(_->type_);
                    return true; // function call
                }
                else {
                    throw "The last declaration of var is not function: " + errCurLex();
                }
            } // else exception
        }
        if (cur == "[") {
            var* _ = curtid->findVar(funcName);
            opStack->pushtype(_->type_);
            if (stArrayTail()) {
                return true; // array access call
            }
        }
        var* _ = curtid->findVar(a.name_);
        opStack->pushtype(new std::string(_->type_));
        if (_->isfunc) {
            throw "Semantic error: using function as a variable\n"s + errCurLex();
        }
        return true;
    }
    throw "Syntax error: incorrect atom\n"s + errCurLex();
}

bool SyntaxAnalyzator::stFunctionTail() {
    auto [cur, num] = movLexem();
    if (cur != "(") {
        throw "Syntax error: expected ( before function call\n"s + errCurLex();
    }
    std::tie(cur, num) = getLexem();
    if (cur == ")") {
        movLexem();
        return true;
    }
    stExpression();
    while (true) {
        std::tie(cur, num) = movLexem();
        if (cur == ")") {
            opStack->mem_.pop_back();
            return true;
        }
        else if (cur == ",") {
            stExpression();
            opStack->mem_.pop_back();
            continue;
        } else {
            throw "Syntax error: expected ) or , in function call\n"s + errCurLex();
        }
    }
}
bool SyntaxAnalyzator::stArrayTail() {
    auto [cur, num] = movLexem();
    if (cur != "[") {
        throw "Syntax error: expected [ before array\'s element access\n"s + errCurLex();
    }
    opStack->pushtype("int");
    opStack->pushop("=");
    stExpression();
    opStack->checkOp();
    std::tie(cur, num) = movLexem();
    if (cur != "]") {
        throw "Syntax error: expected ] after array\'s element access\n"s + errCurLex();
    }
    return true;
}

bool SyntaxAnalyzator::stPriority0(){
    if (isSign()) {
        movLexem();
        stAtom();
        return true;
    }
    if (isIncrement()) {
        movLexem();
        stAtom();
        return true;
    }
    stAtom();
    if (isIncrement()) {
        movLexem();
    }
    return true;
}

bool SyntaxAnalyzator::stPriority1() {
    if (stPriority0()) {
        if (isOperation1()) {
            movLexem();
            stPriority1();
            opStack->checkOp();
        }
        return true;
    } // else exception
}

bool SyntaxAnalyzator::stPriority2() {
    if (stPriority1()) {
        if (isOperation2()) {
            movLexem();
            stPriority2();
            opStack->checkOp();
        }
        return true;
    } // else exception
}

bool SyntaxAnalyzator::stPriority3() {
    if (stPriority2()) {
        if (isOperation3()) {
            movLexem();
            stPriority3();
            opStack->checkOp();
        }
        return true;
    } // else exception
}

bool SyntaxAnalyzator::stPriority4() {
    if (stPriority3()) {
        if (isOperation4()) {
            movLexem();
            stPriority4();
            opStack->checkOp();
        }
        return true;
    } // else exception
}

bool SyntaxAnalyzator::stPriority5() {
    if (stPriority4()) {
        auto [cur, num] = getLexem();
        if (cur == "&") {
            movLexem();
            opStack->pushop(new std::string("&"));
            stPriority5();
            opStack->checkOp();
        }
        return true;
    } // else exception
}

bool SyntaxAnalyzator::stPriority6() {
    if (stPriority5()) {
        auto [cur, num] = getLexem();
        if (cur == "|") {
            movLexem();
            opStack->pushop(new std::string("|"));
            stPriority6();
            opStack->checkOp();
        }
        return true;
    } // else exception
}

bool SyntaxAnalyzator::stPriority7() {
    if (stPriority6()) {
        auto [cur, num] = getLexem();
        if (cur == "^") {
            movLexem();
            opStack->pushop(new std::string("^"));
            stPriority7();
            opStack->checkOp();
        }
        return true;
    } // else exception
}

bool SyntaxAnalyzator::stPriority8() {
    if (stPriority7()) {
        auto [cur, num] = getLexem();
        if (cur == "&&") {
            movLexem();
            opStack->pushop(new std::string("&&"));
            stPriority8();
            opStack->checkOp();
        }
        return true;
    } // else exception
}

bool SyntaxAnalyzator::stPriority9() {
    if (stPriority8()) {
        auto [cur, num] = getLexem();
        if (cur == "||") {
            movLexem();
            opStack->pushop(new std::string("||"));
            stPriority9();
            opStack->checkOp();
        }
        return true;
    } // else exception
}

bool SyntaxAnalyzator::stPriority10() {
    if (stPriority9()) {
        auto [cur, num] = getLexem();
        if (isAssignment(cur)) {  // cur == "="
            movLexem();
            opStack->pushop(new std::string(cur));
            stPriority10();
            opStack->checkOp();
        }
        return true;
    } // else exception
}

bool SyntaxAnalyzator::stExpression() {
    stPriority10();
    return true;
}

bool SyntaxAnalyzator::stExpressionOperator() {
    opStack->memClear();
    stExpression();
    auto [cur, num] = movLexem();
    if (cur != ";") {
        throw "Syntax error: expected ; at the end of expression operator\n"s + errCurLex();
    }
    opStack->memClear();
    return true;
}


bool SyntaxAnalyzator::stConditionalOperator() {
    auto [cur, num] = movLexem();
    if (cur != "if") {
        throw "Syntax error: expected if in conditional (if) operator\n"s + errCurLex();
    }
    std::tie(cur, num) = movLexem();
    if (cur != "(") {
        throw "Syntax error: expected ( in conditional (if) operator\n"s + errCurLex();
    }
    stExpression();
    std::tie(cur, num) = movLexem();
    if (cur != ")") {
        throw "Syntax error: expected ) in conditional (if) operator\n"s + errCurLex();
    }
    enterScope();
    stOperator();
    exitScope();
    while (true) {
        std::tie(cur, num) = getLexem();
        if (cur != "elif") {
            break;
        }
        movLexem();
        std::tie(cur, num) = movLexem();
        if (cur != "(") {
            throw "Syntax error: expected ( in conditional (elif) operator\n"s + errCurLex();
        }
        stExpression();
        std::tie(cur, num) = movLexem();
        if (cur != ")") {
            throw "Syntax error: expected ) in conditional (elif) operator\n"s + errCurLex();
        }
        enterScope();
        stOperator();
        exitScope();
    }
    std::tie(cur, num) = getLexem();
    if (cur != "else") {
        return true;
    }
    movLexem();
    enterScope();
    stOperator();
    exitScope();
    return true;
}

bool SyntaxAnalyzator::stWhileOperator() {
    auto [cur, num] = movLexem();
    if (cur != "while") {
        throw "Syntax error: expected while in while-cycle operator\n"s + errCurLex();
    }
    std::tie(cur, num) = movLexem();
    if (cur != "(") {
        throw "Syntax error: expected ( in while-cycle operator\n"s + errCurLex();
    }
    stExpression();
    std::tie(cur, num) = movLexem();
    if (cur != ")") {
        throw "Syntax error: expected ) in while-cycle operator\n"s + errCurLex();
    }
    stOperator();
    return true;
}

bool SyntaxAnalyzator::stDoWhileOperator() {
    auto [cur, num] = movLexem();
    if (cur != "do") {
        throw "Syntax error: expected do in do-while-cycle operator\n"s + errCurLex();
    }
    stOperator();
    std::tie(cur, num) = movLexem();
    if (cur != "while") {
        throw "Syntax error: expected while in do-while-cycle operator\n"s + errCurLex();
    }
    std::tie(cur, num) = movLexem();
    if (cur != "(") {
        throw "Syntax error: expected ( in do-while-cycle operator\n"s + errCurLex();
    }
    stExpression();
    std::tie(cur, num) = movLexem();
    if (cur != ")") {
        throw "Syntax error: expected ) in do-while-cycle operator\n"s + errCurLex();
    }
    return true;
}


bool SyntaxAnalyzator::stForOperator() {
    auto [cur, num] = movLexem();
    if (cur != "for") {
        throw "Syntax error: expected for in for-cycle operator\n"s + errCurLex();
    }
    std::tie(cur, num) = movLexem();
    if (cur != "(") {
        throw "Syntax error: expected ( in for-cycle operator\n"s + errCurLex();
    }
    stDeclarationFor();
    stExpression();
    std::tie(cur, num) = movLexem();
    if (cur != ";") {
        throw "Syntax error: expected ; after expression in for-cycle operator\n"s + errCurLex();
    }
    stExpression();
    std::tie(cur, num) = movLexem();
    if (cur != ")") {
        throw "Syntax error: expected ) after expression in for-cycle operator\n"s + errCurLex();
    }
    stOperator(true);
    return true;
}

bool SyntaxAnalyzator::stCycleOperator() {
    auto [cur, num] = getLexem();
    if (cur == "for") {
        enterScope();
        stForOperator();
    } else if (cur == "while") {
        stWhileOperator();
    } else if (cur == "do") {
        stDoWhileOperator();
    } else {
        throw "Syntax error: expected cycle\n"s + errCurLex();
    }
}

bool isEq(std::string& a) {
    return a == "=" || a == "+=" || a == "/=" || a == "*=" || a == "-=";
}

bool SyntaxAnalyzator::stSection(bool declar=false, std::string t="") {
    if (isName(declar, std::move(t), true)) {
        movLexem();
        auto [cur, num] = getLexem();
        if (isAssignment(cur)) {
            opStack->pushop(new std::string(cur));
            movLexem();
            stExpression();
            opStack->checkOp();
            return true;
            //std::cout << "a!!!!" << std::endl;
        } else if (cur == "[") {
            movLexem();
            opStack->pushtype("int");
            opStack->pushop("=");
            stExpression();
            opStack->checkOp();
            std::tie(cur, num) = movLexem();
            if (cur != "]") {
                throw "Syntax error: expected ] at the end of section\n"s + errCurLex();
            }
            return true;
        } else {
            return true;
        }
    }
    throw "Syntax error: expected section but haven't found\n"s + errCurLex();
}

bool SyntaxAnalyzator::stDeclaration() {
    if (!isType()) {
        throw "Syntax error: expected typename in declaration\n"s + errCurLex();
    }
    auto [cur, num] = getLexem();
    movLexem();
    if (!stSection(true, cur))
        return false;
    while (true) {
        std::tie(cur, num) = getLexem();
        if (cur == ";") {
            opStack->memClear();
            movLexem();
            return true;
        }
        if (cur == ",") {
            movLexem();
            stSection();
            continue;
        } else {
            throw "Syntax error: expected another section or end of declaration but neither is found\n"s + errCurLex();
        }
    }
}

bool SyntaxAnalyzator::stDeclarationFor() {
    if (!isType()) {
        if (!stSection())
            return false;
        while (true) {
            auto [cur, num] = getLexem();
            if (cur == ";") {
                opStack->memClear();
                movLexem();
                return true;
            }
            if (cur == ",") {
                movLexem();
                stSection();
                opStack->memClear();
                continue;
            } else {
                throw "Syntax error: expected another section or end of declaration but neither is found\n"s + errCurLex();
            }
        }
    }
    auto [cur, num] = getLexem();
    movLexem();
    if (!stSection(true, cur))
        return false;
    while (true) {
        auto [cur, num] = getLexem();
        if (cur == ";") {
            opStack->memClear();
            movLexem();
            return true;
        }
        if (cur == ",") {
            movLexem();
            stSection();
            opStack->memClear();
            continue;
        } else {
            throw "Syntax error: expected another section or end of declaration but neither is found\n"s + errCurLex();
        }
    }
}


bool SyntaxAnalyzator::stFunction(bool declar=false) {
    auto [cur, num] = getLexem();
    if (!isType() && cur != "void") {
        throw "Syntax error: didn't find return's type of function\n"s + errCurLex();
    }

    movLexem();
    if (!isName(declar, cur, false, true)) {
        throw "Syntax error: didn't find name of function\n"s + errCurLex();
    }
    movLexem();
    std::tie(cur, num) = movLexem();
    if (cur != "(") {
        throw "Syntax error: expected ( in declaration of function\n"s + errCurLex();
    }
    enterScope();
    if (!isType()) {
        std::tie(cur, num) = movLexem();
        if (cur != ")") {
            throw "Syntax error: expected ) in declaration of function\n"s + errCurLex();
        } else {
            return stCompoundOperator(true);
        }
    } else {
        std::tie(cur, num) = getLexem();
        movLexem();
        if (!isName(true, cur)) {
            throw "Syntax error: expected name in declaration (input) of function\n"s + errCurLex();
        }
        movLexem();
    }
    while (true) {
        std::tie(cur, num) = movLexem();
        if (cur == ")") {
            return stCompoundOperator(true);
        }
        if (cur == ",") {
            if (!isType())
                throw "Syntax error: expected type in declaration (input) of function\n"s + errCurLex();
            std::tie(cur, num) = getLexem();
            movLexem();
            if (!isName(true, cur))
                throw "Syntax error: expected name in declaration (input) of function\n"s + errCurLex();
            movLexem();
        } else {
            throw "Syntax error: expected , or ) in declaration of function\n"s + errCurLex();
        }
    }
}

bool SyntaxAnalyzator::stFunctionCallOperator() {
    if (!isName()) {
        throw "Syntax error: expected name of function in function call\n"s + errCurLex();
    }
    movLexem();
    stFunctionTail();
    auto [cur, num] = movLexem();
    if (cur != ";") {
        throw "Syntax error: expected ; after function call\n"s + errCurLex();
    }
    return true;
}

bool SyntaxAnalyzator::stProgram() {
    while (true) {
        auto beforeTriesCell = mainLexer_->getCurrentLexCell();
        auto [cur, num] = movLexem();
        if (cur == "int") {
            std::tie(cur, num) = movLexem();
            if (cur == "main") {
                if (describingFunc) {
                    var* _ = curtid->findVar(describingFuncName);
                    if (_->type_ != "void") {
                        throw "The previous function should return: " + _->type_ + "\nbut doesn't return any\n" + errCurLex();
                    }
                }
                mainLexer_->movLexem(true);  // step back
                isName(true, "int", false, true);
                movLexem();

                std::tie(cur, num) = movLexem();
                if (cur != "(") {
                    throw "Syntax error: expected ( in main call\n"s + errLastLex();
                }
                std::tie(cur, num) = movLexem();
                if (cur != ")") {
                    throw "Syntax error: expected ) in main call\n"s + errLastLex();
                }
                stCompoundOperator();
                return true;
            }
        }
        mainLexer_->jumpToCell(beforeTriesCell);
        movLexem();
        movLexem();
        std::tie(cur, num) = getLexem();
        mainLexer_->jumpToCell(beforeTriesCell);
        if (cur == "(") {
            stFunction(true);
        } else {
            stDeclaration();
        }
    }
}

bool SyntaxAnalyzator::stOperator(bool enteredScope) {
    opStack->memClear();
    auto [cur, num] = getLexem();
    if (isType()) {
        stDeclaration();
        return true;
    }
    if (cur == "{") {
        stCompoundOperator(enteredScope);
        return true;
    }
    if (cur == "cin") {
        stInputOperator();
        return true;
    }
    if (cur == "cout") {
        stOutputOperator();
        return true;
    }
    if (cur == "return") {
        stReturnOperator();
        return true;
    }
    if (cur == "for" || cur == "while" || cur == "do") {
        stCycleOperator();
        return true;
    }
    if (cur == "if") {
        stConditionalOperator();
        return true;
    }
    if (cur == "break") {
        stBreakOperator();
        return true;
    }
    if (cur == "continue") {
        stContinueOperator();
        return true;
    }
    auto beforeTriesCell = mainLexer_->getCurrentLexCell();
    movLexem();
    std::tie(cur, num) = getLexem();
    mainLexer_->jumpToCell(beforeTriesCell);
    if (cur == "(") {
        stFunctionCallOperator();
        return true;
    }
    stExpressionOperator();
    return true;
}

bool SyntaxAnalyzator::stCompoundOperator(bool enteredScope) {
    auto [cur, num] = movLexem();
    if (cur != "{") {
        throw "Syntax error: expected { at the beginning of compound operator\n"s + errCurLex();
    }
    if (!enteredScope) enterScope();  // for example, we start sycle for earlier than brackets

    while (true) {
        std::tie(cur, num) = getLexem();
        if (cur == "}") {  // we always close block with exit of scope no matter what
            movLexem();
            exitScope();
            return true;
        }
        opStack->memClear();
        stOperator();
    }
    // else exception in stOperator()
}

std::string SyntaxAnalyzator::work() {
    try {
        auto s = stProgram();
        if (getLexem().second != 0)
            throw "Syntax error: something after main detected\n"s + errCurLex();
        return s ? "OK" : "How are we here? This is error";
    } catch (std::string s) {
        return s;
    }
}

SyntaxAnalyzator::SyntaxAnalyzator(LexicalAnalyzator *&mainLexer): mainLexer_(mainLexer) {
    opStack = new typeStack(mainLexer_);
}

std::pair<int, int> SyntaxAnalyzator::curWhere() const {
    return mainLexer_->getPosCurCell();
}
