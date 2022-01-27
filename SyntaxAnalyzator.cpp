//
// Created by maincpp on 21.01.2022.
//

#include "SyntaxAnalyzator.h"

std::pair<std::string, int> SyntaxAnalyzator::getLexem() {
    return mainLexer_->getLexem();
}

bool SyntaxAnalyzator::stBool() {
    auto [cur, num] = getLexem();  // AOAOAOAO, LOOK HERE, IS IT COPY OF STRING???
    if (cur != "true" && cur != "false") {  // either first or we
        return false;
    }
    return true;
}

/*bool SyntaxAnalyzator::stNumber() {
    auto [cur, num] = getLexem();  // how to call lexer not by name
    int pt = 0;
    if (!(cur[0] >= '0' && cur[0] <= '9')) {
        if (!(cur[0] == '+' || cur[0] == '-')) return false;
        ++pt;
    }
    int n = static_cast<int>(cur.size());
    if (n == 1) {
        if (!(cur[pt] >= '0' && cur[pt] <= '9'))
            return false;
    }
    ++pt;
    //bool flag = true;
    int ind_pt = -1, ind_exp = -1;
    int ind_sign = -1;
    for (int i = pt; i < n; ++i) {
        if (!(cur[i] >= '0' && cur[i] <= '9')) {
            if (cur[i] == '.') {
                if (ind_pt != -1) return false;
                else ind_pt = i;
            } else if (cur[i] == 'e' || cur[i] == 'E') {
                if (ind_exp != -1) return false;
                else ind_exp = i;
            } else if (cur[i] == '+' || cur[i] == '-'){
                if (ind_sign != -1) return false;
                else ind_sign = i;
            } else {
                return false;
            }
        }
    }
    if (ind_pt == -1 && ind_exp == -1 && ind_sign == -1) return true;
    if (ind_pt != -1 && ind_exp != -1) return false;
    if (ind_pt != -1) {
        if (ind_pt == n - 1) return false;
        if (ind_sign != -1) return false;
    }
    if (ind_exp != -1) {
        if (ind_sign == -1) {
            if (ind_exp == n - 1) return false;
        } else {
            if (ind_exp + 1 != ind_sign) return false;
            if (ind_sign == n - 1) return false;
        }
    }
    return true;
}*/

bool SyntaxAnalyzator::stOperation1() {
    auto [cur, num] = getLexem();
    if (cur != "*" && cur != "/" && cur != "%") return false;
    return true;
}

bool SyntaxAnalyzator::stOperation2() {
    auto [cur, num] = getLexem();
    if (cur != "+" && cur != "-") return false;
    return true;
}

bool SyntaxAnalyzator::stOperation3() {
    auto [cur, num] = getLexem();
    if (cur != "<" && cur != ">" && cur != "<=" && cur != ">=") return false;
    return true;
}

bool SyntaxAnalyzator::stOperation4() {
    auto [cur, num] = getLexem();
    if (cur != "==" && cur != "<>") return false;
    return true;
}


bool SyntaxAnalyzator::stDelimiter() {
    auto [cur, num] = getLexem();
    if (cur != "\n" && cur != "' '") return false;
    return true;
}

bool SyntaxAnalyzator::stName() {
    auto [cur, num] = getLexem();
    if (num == 2) return true;
    /*return false;
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
    if (!(check_first && check_other)) return false;
    return true;*/
}

bool SyntaxAnalyzator::stReturn() {
    auto [cur, num] = getLexem();
    if (cur != "return") return false;
    auto beforeTriesCell = mainLexer_->getCurrentLexCell();
    std::tie(cur, num) = getLexem();
    if (cur == ";") return true;
    /*mainLexer_->jumpToCell(beforeTriesCell);
    beforeTriesCell = mainLexer_->getCurrentLexCell();
    if (stName()) return true;
    mainLexer_->jumpToCell(beforeTriesCell);
    beforeTriesCell = mainLexer_->getCurrentLexCell();
    if (stBool()) return true;
    mainLexer_->jumpToCell(beforeTriesCell);
    beforeTriesCell = mainLexer_->getCurrentLexCell();
    if (stNumber()) return true;
    mainLexer_->jumpToCell(beforeTriesCell);
    std::tie(cur, num) = getLexem();
    if (cur != ";") // error
    return true;*/
    mainLexer_->jumpToCell(beforeTriesCell);
    //beforeTriesCell = mainLexer_->getCurrentLexCell();
    if (stExpression()) {
        std::tie(cur, num) = getLexem();
        if (cur == ";") return true;
        else return false;
    }
    return false;
}

bool SyntaxAnalyzator::stInput() {
    auto [cur, num] = getLexem();
    if (cur != "cin") return false;
    std::tie(cur, num) = getLexem();
    if (cur != ">>") return false;
    if (!stName()) return false;
    while (true) {
        std::tie(cur, num) = getLexem();
        if (cur == ";") break;
        if (cur != ">>") return false;
        if (!stName()) return false;
    }
    return true;
}

bool SyntaxAnalyzator::stBreak() {
    auto [cur, num] = getLexem();
    if (cur != "break") return false;
    std::tie(cur, num) = getLexem();
    if (cur != ";") return false;
    return true;
}

bool SyntaxAnalyzator::stContinue() {
    auto [cur, num] = getLexem();
    if (cur != "continue") return false;
    std::tie(cur, num) = getLexem();
    if (cur != ";") return false;
    return true;
}

bool SyntaxAnalyzator::stType() {
    auto [cur, num] = getLexem();
    if (cur != "int" && cur != "char" && cur != "double" && cur != "float" && cur != "bool") return false;
    return true;
}

bool SyntaxAnalyzator::stOutput() {
    auto [cur, num] = getLexem();
    if (cur != "cout") return false;
    std::tie(cur, num) = getLexem();
    if (cur != "<<") return false;
    auto beforeTriesCell = mainLexer_->getCurrentLexCell();
    if (!stDelimiter()) {
        mainLexer_->jumpToCell(beforeTriesCell);
        if (!stExpression()) {
            return false;
        } else { // Expression
        }
    } else { // Delimeter
    }
    while (true) {
        beforeTriesCell = mainLexer_->getCurrentLexCell();
        std::tie(cur, num) = getLexem();
        std::cerr << cur << std::endl;
        if (cur != "<<") {
            mainLexer_->jumpToCell(beforeTriesCell);
            break;
        }
        beforeTriesCell = mainLexer_->getCurrentLexCell();
        if (!stDelimiter()) {
            mainLexer_->jumpToCell(beforeTriesCell);
            if (!stExpression()) {
                mainLexer_->jumpToCell(beforeTriesCell);
                break;
            } else { // Expression
            }
        } else { // Delimeter
        }
    }
    std::tie(cur, num) = getLexem();
    if (cur != ";")
        return false;
    return true;
}

bool SyntaxAnalyzator::stInitAtom() {
    //std::cout << "init atom" << std::endl;
    auto beforeTriesCell = mainLexer_->getCurrentLexCell();
    //std::cout << "a1" << std::endl;
    auto [cur, num] = getLexem();
    if (num == 3) return true;
    mainLexer_->jumpToCell(beforeTriesCell);
    //std::cout << "a2" << std::endl;
//    beforeTriesCell = mainLexer_->getCurrentLexCell();
    std::tie(cur, num) = getLexem();
    if (cur != "!") return false;
    if (!stAtom()) return false;
    return true;
}

bool SyntaxAnalyzator::stAtom() {
    //std::cout << "atom" << std::endl;
    auto beforeTriesCell = mainLexer_->getCurrentLexCell();
    if (stInitAtom()) return true; // InitAtom
    mainLexer_->jumpToCell(beforeTriesCell);
    if (stName()) { // Name[Expression]
        auto [cur, num] = getLexem();
        if (cur == "[") {
            if (stExpression()) {
                std::tie(cur, num) = getLexem();
                if (cur == "]") return true;
            }
        }
    }
    mainLexer_->jumpToCell(beforeTriesCell);
    if (stName()) return true; // Name
    mainLexer_->jumpToCell(beforeTriesCell);
    auto [cur, num] = getLexem();
    if (cur == "(") {
        if (stExpression()) {
            std::tie(cur, num) = getLexem();
            if (cur == ")") return true; // (Expression)
        }
    }
    return false;
}

bool SyntaxAnalyzator::stIncrement() {
    auto [cur, num] = getLexem();
    if (cur != "--" && cur != "++") return false;
    return true;
}
bool SyntaxAnalyzator::stPriority0(){
    auto beforeTriesCell = mainLexer_->getCurrentLexCell();
    auto [cur, num] = getLexem();
    if (cur == "+" || cur == "-") {
        if (stAtom()) // Where else?
            return true;
        else
            return false;
    }
    mainLexer_->jumpToCell(beforeTriesCell);
    if (stAtom()) {
        auto beforeTriesCell1 = mainLexer_->getCurrentLexCell();
        if (stIncrement())
            return true;
        else {
            mainLexer_->jumpToCell(beforeTriesCell1);
            return true;
        }
    }
    mainLexer_->jumpToCell(beforeTriesCell);
    if (stIncrement()) {
        if (stAtom()) return true;
    }
    //mainLexer_->jumpToCell(beforeTriesCell);
    return false;
}

bool SyntaxAnalyzator::stPriority1() {
    auto beforeTriesCell = mainLexer_->getCurrentLexCell();
    if (stPriority0())
        if (stOperation1())
            if (stPriority1()) return true;
    mainLexer_->jumpToCell(beforeTriesCell);
    if (stPriority0()) return true;
    return false;
}

bool SyntaxAnalyzator::stPriority2() {
    auto beforeTriesCell = mainLexer_->getCurrentLexCell();
    if (stPriority1()) {
        if (stOperation2()) {
            if (stPriority2()) return true;
        }
    }
    mainLexer_->jumpToCell(beforeTriesCell);
    if (stPriority1()) return true;
    return false;
}

bool SyntaxAnalyzator::stPriority3() {
    auto beforeTriesCell = mainLexer_->getCurrentLexCell();
    if (stPriority2())
        if (stOperation3())
            if (stPriority3()) return true;
    mainLexer_->jumpToCell(beforeTriesCell);
    if (stPriority2()) return true;
    return false;
}

bool SyntaxAnalyzator::stPriority4() {
    auto beforeTriesCell = mainLexer_->getCurrentLexCell();
    if (stPriority3())
        if (stOperation4())
            if (stPriority4()) return true;
    mainLexer_->jumpToCell(beforeTriesCell);
    if (stPriority3()) return true;
    return false;
}

bool SyntaxAnalyzator::stPriority5() {
    auto beforeTriesCell = mainLexer_->getCurrentLexCell();
    if (stPriority4()) {
        auto [cur, num] = getLexem();
        if (cur == "&")
            if (stPriority5()) return true;
    }
    mainLexer_->jumpToCell(beforeTriesCell);
    if (stPriority4()) return true;
    return false;
}

bool SyntaxAnalyzator::stPriority6() {
    auto beforeTriesCell = mainLexer_->getCurrentLexCell();
    if (stPriority5()) {
        auto [cur, num] = getLexem();
        if (cur == "|")
            if (stPriority6()) return true;
    }
    mainLexer_->jumpToCell(beforeTriesCell);
    if (stPriority5()) return true;
    return false;
}

bool SyntaxAnalyzator::stPriority7() {
    auto beforeTriesCell = mainLexer_->getCurrentLexCell();
    if (stPriority6()) {
        auto [cur, num] = getLexem();
        if (cur == "^")
            if (stPriority7()) return true;
    }
    mainLexer_->jumpToCell(beforeTriesCell);
    if (stPriority6()) return true;
    return false;
}

bool SyntaxAnalyzator::stInitAtomBool() {
    auto beforeTriesCell = mainLexer_->getCurrentLexCell();
    if (stBool()) return true;
    mainLexer_->jumpToCell(beforeTriesCell);
    auto [cur, num] = getLexem();
    if (cur != "!") return false;
    if (!stAtomBool()) return false;
    return true;
}

bool SyntaxAnalyzator::stAtomBool() {
    auto beforeTriesCell = mainLexer_->getCurrentLexCell();
    if (stInitAtomBool()) return true;
    mainLexer_->jumpToCell(beforeTriesCell);
    if (stName()) return true;
    mainLexer_->jumpToCell(beforeTriesCell);
    auto [cur, num] = getLexem();
    if (cur == "(")
        if (stExpression()) {
            std::tie(cur, num) = getLexem();
            if (cur == ")") return true;
        }
    mainLexer_->jumpToCell(beforeTriesCell);
    if (stPriority7()) return true;
    return false;
}

bool SyntaxAnalyzator::stPriority1Bool() {
    auto beforeTriesCell = mainLexer_->getCurrentLexCell();
    if (stAtomBool()) return true;
    mainLexer_->jumpToCell(beforeTriesCell);
    if (stAtomBool())
        if (stOperation3())
            if (stPriority1Bool()) return true;
    return false;
}

bool SyntaxAnalyzator::stPriority2Bool() {
    auto beforeTriesCell = mainLexer_->getCurrentLexCell();
    if (stPriority1Bool()) return true;
    mainLexer_->jumpToCell(beforeTriesCell);
    if (stPriority1Bool())
        if (stOperation4())
            if (stPriority2Bool()) return true;
    return false;
}

bool SyntaxAnalyzator::stPriority3Bool() {
    auto beforeTriesCell = mainLexer_->getCurrentLexCell();
    if (stPriority2Bool()) return true;
    mainLexer_->jumpToCell(beforeTriesCell);
    if (stPriority2Bool()) {
        auto [cur, num] = getLexem();
        if (cur == "&&")
            if (stPriority3Bool()) return true;
    }
    return false;
}

bool SyntaxAnalyzator::stPriority4Bool() {
    auto beforeTrieCell = mainLexer_->getCurrentLexCell();
    if (stPriority3Bool()) return true;
    mainLexer_->jumpToCell(beforeTrieCell);
    if (stPriority3Bool()) {
        auto [cur, num] = getLexem();
        if (cur == "||")
            if (stPriority4Bool()) return true;
    }
    return false;
}

bool SyntaxAnalyzator::stExpression() {
    //std::cout << "expression" << std::endl;
    auto beforeTrieCell = mainLexer_->getCurrentLexCell();
    if (stPriority7()) return true;
    mainLexer_->jumpToCell(beforeTrieCell);
    if (stPriority4Bool()) return true;
    mainLexer_->jumpToCell(beforeTrieCell);
    if (stName()) {
        auto [cur, num] = getLexem();
        if (cur == "=") {
            beforeTrieCell = mainLexer_->getCurrentLexCell();
            if (stPriority7()) return true;
            mainLexer_->jumpToCell(beforeTrieCell);
            if (stPriority4Bool()) return true;
        }
    }
    return false;
}

bool SyntaxAnalyzator::stExpressionOperator() {
    if (!stExpression()) return false;
    auto [cur, num] = getLexem();
    if (cur != ";") return false;
    return true;
}

bool SyntaxAnalyzator::stCycleOperator() {
    auto beforeTrieCell = mainLexer_->getCurrentLexCell();
    if (stForOperator()) return true;
    mainLexer_->jumpToCell(beforeTrieCell);
    if (stForAutoOperator()) return true;
    mainLexer_->jumpToCell(beforeTrieCell);
    if (stWhileOperator()) return true;
    mainLexer_->jumpToCell(beforeTrieCell);
    if (stDoWhileOperator()) return true;
    return false;
}

bool SyntaxAnalyzator::stWhileOperator() {
    auto [cur, num] = getLexem();
    if (cur != "while") return false;
    std::tie(cur, num) = getLexem();
    if (cur != "(") return false;
    if (!stExpression()) return false;
    std::tie(cur, num) = getLexem();
    if (cur != ")") return false;
    if (!stOperator()) return false;
    return true;
}

bool SyntaxAnalyzator::stDoWhileOperator() {
    auto [cur, num] = getLexem();
    if (cur != "do") return false;
    if (!stOperator()) return false;
    std::tie(cur, num) = getLexem();
    if (cur != "while") return false;
    std::tie(cur, num) = getLexem();
    if (cur != "(") return false;
    if (!stExpression()) return false;
    std::tie(cur, num) = getLexem();
    if (cur != ")") return false;
    return true;
}

bool SyntaxAnalyzator::stForOperator() {
    auto [cur, num] = getLexem();
    if (cur != "for") return false;
    //std::cout << "a1" << std::endl;
    std::tie(cur, num) = getLexem();
    if (cur != "(") return false;
    //std::cout << "a2" << std::endl;
    if (!stDeclaration()) return false;
    //std::cout << "a3" << std::endl;
    //std::tie(cur, num) = getLexem();
    //if (cur != ";") return false;
    //std::cout << "a4" << std::endl;
    if (!stExpression()) return false;
    //std::cout << "a5" << std::endl;
    std::tie(cur, num) = getLexem();
    if (cur != ";") return false;
    //std::cout << "a6" << std::endl;
    if (!stExpression()) return false;
    //std::cout << "a7" << std::endl;
    std::tie(cur, num) = getLexem();
    if (cur != ")") return false;
    //std::cout << "a8" << std::endl;
    if (!stOperator()) return false;
    //std::cout << "a9" << std::endl;
    return true;
}

bool SyntaxAnalyzator::stForAutoOperator() {
    auto [cur, num] = getLexem();
    if (cur != "for") return false;
    std::tie(cur, num) = getLexem();
    if (cur != "(") return false;
    auto beforeTrieCell = mainLexer_->getCurrentLexCell();
    if (!stType()) {
        mainLexer_->jumpToCell(beforeTrieCell);
        if (!stName()) return false;
    } else {
        if (!stName()) return false;
    }
    std::tie(cur, num) = getLexem();
    if (cur != ":") return false;
    if (!stName()) return false;
    std::tie(cur, num) = getLexem();
    if (cur != ")") return false;
    if (!stOperator()) return false;
    return true;
}
bool SyntaxAnalyzator::stConditionalOperator() {
    auto [cur, num] = getLexem();
    if (cur != "if") return false;
    std::tie(cur, num) = getLexem();
    if (cur != "(") return false;
    if (!stExpression()) return false;
    std::tie(cur, num) = getLexem();
    if (cur != ")") return false;
    if (!stOperator()) return false;
    while (true) {
        auto beforeTriesCell = mainLexer_->getCurrentLexCell();
        std::tie(cur, num) = getLexem();
        if (cur != "elif") {
            mainLexer_->jumpToCell(beforeTriesCell);
            break;
        }
        std::tie(cur, num) = getLexem();
        if (cur != "(") return false;
        if (!stExpression()) return false;
        std::tie(cur, num) = getLexem();
        if (cur != ")") return false;
        if (!stOperator()) return false;
    }
    auto beforeTriesCell = mainLexer_->getCurrentLexCell();
    std::tie(cur, num) = getLexem();
    if (cur != "else") {
        mainLexer_->jumpToCell(beforeTriesCell);
        return true;
    }
    if (!stOperator())
        return false;
    return true;
}

bool SyntaxAnalyzator::stSection() {

    if (stName()) {
        auto beforeTriesCell = mainLexer_->getCurrentLexCell();
        auto [cur, num] = getLexem();
        auto beforeTriesCell2 = mainLexer_->getCurrentLexCell();
        auto [cur2, num2] = getLexem();
        std::cerr << cur2 << " ?" << std::endl;
        mainLexer_->jumpToCell(beforeTriesCell2);
        if (cur == "=") {
            if (!stExpression())
                return false;
            return true;
            //std::cout << "a!!!!" << std::endl;
        } else if (cur == "[") {
            if (!stExpression())
                return false;
            std::tie(cur, num) = getLexem();
            if (cur != "]")
                return false;
            return true;
        } else {
            mainLexer_->jumpToCell(beforeTriesCell);
            return true;
        }
    }
    return false;
}

bool SyntaxAnalyzator::stDeclaration() {
    if (!stType())
        return false;
    if (!stSection())
        return false;
    while (true) {
        auto [cur, num] = getLexem();
        std::cerr << cur << "!  " << std::endl;
        if (cur == ";")
            return true;
        if (cur == ",") {
            if (!stSection())
                return false;
        } else {
            return false;
        }
    }
}

bool SyntaxAnalyzator::stFunction() {
    auto beforeTriesCell = mainLexer_->getCurrentLexCell();
    if (!stType()) {
        mainLexer_->jumpToCell(beforeTriesCell);
        auto [cur, num] = getLexem();
        if (cur != "void")
            return false;
    }
    beforeTriesCell = mainLexer_->getCurrentLexCell();
    auto [cur, num] = getLexem();
    if (cur == "main")
        return false;
    mainLexer_->jumpToCell(beforeTriesCell);
    if (!stName())
        return false;
    std::tie(cur, num) = getLexem();
    if (cur != "(")
        return false;
    beforeTriesCell = mainLexer_->getCurrentLexCell();
    if (!stType()) {
        mainLexer_->jumpToCell(beforeTriesCell);
        std::tie(cur, num) = getLexem();
        if (cur != ")")
            return false;
        else
            return stOperator();
    } else {
        if (!stName())
            return false;
    }
    while (true) {
        beforeTriesCell = mainLexer_->getCurrentLexCell();
        std::tie(cur, num) = getLexem();
        if (cur == ")")
            return stOperator();
        if (cur == ",") {
            if (!stType())
                return false;
            if (!stName())
                return false;
        } else {
            return false;
        }
    }
}

bool SyntaxAnalyzator::stFunctionCall() {
    if (!stName())
        return false;
    auto [cur, num] = getLexem();
    if (cur != "(")
        return false;
    auto beforeTriesCell = mainLexer_->getCurrentLexCell();
    std::tie(cur, num) = getLexem();
    if (cur == ")") {
        std::tie(cur, num) = getLexem();
        return cur == ";";
    }
    mainLexer_->jumpToCell(beforeTriesCell);
    if (!stExpression())
        return false;
    while (true) {
        beforeTriesCell = mainLexer_->getCurrentLexCell();
        std::tie(cur, num) = getLexem();
        if (cur == ")") {
            std::tie(cur, num) = getLexem();
            return cur == ";";
        }
        if (cur == ",") {
            if (!stExpression())
                return false;
        } else
            return false;
    }
}

bool SyntaxAnalyzator::stProgram() {
    while (true) {
        auto beforeTriesCell = mainLexer_->getCurrentLexCell();
        if (!stDeclaration()) {
            mainLexer_->jumpToCell(beforeTriesCell);
        } else
            continue;
        if (!stFunction()) {
            mainLexer_->jumpToCell(beforeTriesCell);
            break;
        } else {
            continue;
        }
    }
    auto [cur, num] = getLexem();
    if (cur != "int")
        return false;
    std::tie(cur, num) = getLexem();
    if (cur != "main")
        return false;
    std::tie(cur, num) = getLexem();
    if (cur != "(")
        return false;
    std::tie(cur, num) = getLexem();
    if (cur != ")")
        return false;
    if (!stCompoundOperator())
        return false;
    return true;
}

bool SyntaxAnalyzator::stOperator() {
    auto beforeTriesCell = mainLexer_->getCurrentLexCell();
    if (stCompoundOperator()) return true;
    mainLexer_->jumpToCell(beforeTriesCell);
    if (stOutput()) return true;
    mainLexer_->jumpToCell(beforeTriesCell);
    if (stInput()) return true;
    mainLexer_->jumpToCell(beforeTriesCell);
    if (stReturn()) return true;
    mainLexer_->jumpToCell(beforeTriesCell);
    if (stDeclaration()) return true;
    mainLexer_->jumpToCell(beforeTriesCell);
    if (stExpressionOperator()) return true;
    mainLexer_->jumpToCell(beforeTriesCell);
    if (stCycleOperator()) return true;
    mainLexer_->jumpToCell(beforeTriesCell);
    if (stConditionalOperator()) return true;
    mainLexer_->jumpToCell(beforeTriesCell);
    if (stBreak()) return true;
    mainLexer_->jumpToCell(beforeTriesCell);
    if (stContinue()) return true;
    mainLexer_->jumpToCell(beforeTriesCell);
    if (stFunctionCall()) return true;
    return false;
}

bool SyntaxAnalyzator::stCompoundOperator() {
    auto [cur, num] = getLexem();
    if (cur != "{") return false;
    while (true) {
        auto beforeTriesCell = mainLexer_->getCurrentLexCell();
        std::tie(cur, num) = getLexem();
        if (cur == "}") return true;
        mainLexer_->jumpToCell(beforeTriesCell);
        if (!stOperator()) return false;
    }
}

bool SyntaxAnalyzator::work() {
    return stProgram();
}

SyntaxAnalyzator::SyntaxAnalyzator(LexicalAnalyzator *&mainLexer): mainLexer_(mainLexer) {
}
