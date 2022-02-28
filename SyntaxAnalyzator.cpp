//
// Created by maincpp on 21.01.2022.
//

#include "SyntaxAnalyzator.h"

using namespace std::literals::string_literals;

std::pair<std::string, int> SyntaxAnalyzator::movLexem() {
    return mainLexer_->movLexem();
}
std::pair<std::string, int> SyntaxAnalyzator::getLexem() {
    return mainLexer_->getLexem();
}

bool SyntaxAnalyzator::isType() {
    auto [cur, num] = getLexem();
    if (cur != "int" && cur != "char" && cur != "double" && cur != "float" && cur != "bool")
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

bool SyntaxAnalyzator::isNumber() {
    auto [cur, num] = getLexem();
    if (num != 3) {
        return false;
    }
    return true;
}

bool SyntaxAnalyzator::isSign() {
    auto [cur, num] = getLexem();
    if (cur != "+" && cur != "-") {
        return false;
    }
    return true;
}

bool SyntaxAnalyzator::isIncrement() {
    auto [cur, num] = getLexem();
    if (cur != "--" && cur != "++") {
        return false;
    }
    return true;
}

bool SyntaxAnalyzator::isOperation1() {
    auto [cur, num] = getLexem();
    if (cur != "*" && cur != "/" && cur != "%")
        return false;
    return true;
}

bool SyntaxAnalyzator::isOperation2() {
    auto [cur, num] = getLexem();
    if (cur != "+" && cur != "-")
        return false;
    return true;
}

bool SyntaxAnalyzator::isOperation3() {
    auto [cur, num] = getLexem();
    if (cur != "<" && cur != ">" && cur != "<=" && cur != ">=")
        return false;
    return true;
}

bool SyntaxAnalyzator::isOperation4() {
    auto [cur, num] = getLexem();
    if (cur != "==" && cur != "<>")
        return false;
    return true;
}


bool SyntaxAnalyzator::isDelimiter() {
    auto [cur, num] = getLexem();
    if (cur != "\n" && cur != "\' \'")
        return false;
    return true;
}

bool SyntaxAnalyzator::isName() {
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
    return true;
}

bool SyntaxAnalyzator::stReturnOperator() {
    auto [cur, num] = movLexem();
    if (cur != "return") {
        throw "Syntax error: expected return, but "s + (num ? cur : "nothing"s) + "is found"s;
    }
    std::tie(cur, num) = getLexem();
    if (cur == ";") {
        movLexem();
        return true;
    }
    if (stExpression()) {
        std::tie(cur, num) = movLexem();
        if (cur == ";") {
            return true;
        } else {
            throw "Syntax error: expected ; after return operator, but "s + (num ? cur : "nothing"s) + "is found"s;
        }
    }
    throw "Syntax error: expected simple return or expression in return but neither is found"s;
}

bool SyntaxAnalyzator::stInputOperator() {
    auto [cur, num] = movLexem();
    if (cur != "cin") {
        throw "Syntax error: expected cin in input operator"s;
    }
    std::tie(cur, num) = movLexem();
    if (cur != ">>") {
        throw "Syntax error: expected >> in input operator"s;
    }
    if (!isName()) {
        throw "Syntax error: expected name in input operator"s;
    } else {
        movLexem();
    }
    while (true) {
        std::tie(cur, num) = movLexem();
        if (cur == ";") {
            break;
        }
        if (cur != ">>") {
            throw "Syntax error: expected >> in input operator"s;
        }
        if (!isName()) {
            throw "Syntax error: expected name in input operator"s;
        } else {
            movLexem();
        }
    }
    return true;
}

bool SyntaxAnalyzator::stOutputOperator() {
    auto [cur, num] = movLexem();
    if (cur != "cout") {
        throw "Syntax error: expected cout in output operator"s;
    }
    std::tie(cur, num) = movLexem();
    if (cur != "<<") {
        throw "Syntax error: expected << in output operator"s;
    }
    if (!isDelimiter()) {
        if (!stExpression()) {
            throw "Syntax error: expected expression or delimiter in output operator but neither is found"s;
        } else { // Expression
        }
    } else { // Delimeter
        movLexem();
    }
    while (true) {
        std::tie(cur, num) = movLexem();
        if (cur == ";") {
            return true;
        }
        if (cur != "<<") {
            throw "Syntax error: expected << in output operator"s;
        }
        if (!isDelimiter()) {
            if (!stExpression()) {
                throw "Syntax error: expected expression or delimiter in output operator but neither is found"s;
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
        throw "Syntax error: expected break"s;
    }
    std::tie(cur, num) = movLexem();
    if (cur != ";") {
        throw "Syntax error: expected ; after break"s;
    }
    return true;
}

bool SyntaxAnalyzator::stContinueOperator() {
    auto [cur, num] = movLexem();
    if (cur != "continue") {
        throw "Syntax error: expected continue"s;
    }
    std::tie(cur, num) = movLexem();
    if (cur != ";") {
        throw "Syntax error: expected ; after continue"s;
    }
    return true;
}


bool SyntaxAnalyzator::stInitAtom() {
    if (isNumber()) {
        movLexem();
        return true;
    }
    if (isBool()) {
        movLexem();
        return true;
    }
    auto [cur, num] = movLexem();
    if (cur != "!") {
        throw "Syntax error: expected ! when tried to find initial atom but "s + (num ? cur : "nothing"s) + "is found"s;
    }
    if (stAtom())
        return true;
    // else exception
}

bool SyntaxAnalyzator::stAtom() {
    auto [cur, num] = getLexem();
    if (cur == "!" || isNumber() || isBool()) {
        if (stInitAtom()) {
            return true;
        } // else exception
    }
    if (cur == "(") {
        movLexem();
        if (stExpression()) {
            std::tie(cur, num) = movLexem();
            if (cur != ")") {
                throw "Syntax error: expected ) after expression"s;
            } else {
                return true;
            }
        } // else exception
    }
    if (isName()) {
        movLexem();
        std::tie(cur, num) = getLexem();
        if (cur == "(") {
            if (stFunctionTail()) {
                return true; // function call
            } // else exception
        }
        if (cur == "[") {
            if (stArrayTail()) {
                return true; // array access call
            }
        }
        return true;
    }
    throw "Syntax error: incorrect atom"s;
}

bool SyntaxAnalyzator::stFunctionTail() {
    auto [cur, num] = movLexem();
    if (cur != "(") {
        throw "Syntax error: expected ( before function call"s;
    }
    std::tie(cur, num) = getLexem();
    if (cur == ")") {
        movLexem();
        return true;
    }
    stExpression();
    while (true) {
        std::tie(cur, num) = movLexem();
        if (cur == ")")
            return true;
        else if (cur == ",") {
            stExpression();
            continue;
        } else {
            throw "Syntax error: expected ) or , in function call"s;
        }
    }
}
bool SyntaxAnalyzator::stArrayTail() {
    auto [cur, num] = movLexem();
    if (cur != "[") {
        throw "Syntax error: expected [ before array\'s element access"s;
    }
    stExpression();
    std::tie(cur, num) = movLexem();
    if (cur != "]") {
        throw "Syntax error: expected ] after array\'s element access"s;
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
        //        if (!isName()) {
        //            throw "Syntax error: expected name after increment"s;
        //        }
        stAtom();
        return true;
    }
    //    auto beforeTriesCell = mainLexer_->getCurrentLexCell();
    //    movLexem();
    //    if (isIncrement()) {
    //        mainLexer_->jumpToCell(beforeTriesCell);
    //        if (isName()) {
    //            movLexem();
    //            if (isIncrement()) {
    //                return true;
    //            } else {
    //                throw "Syntax error: expected increment after name"s;
    //            }
    //        } else {
    //            throw "Syntax error: expected name before increment"s;
    //        }
    //    }
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
            if (stPriority1()) {
                return true;
            }
        }
        return true;
    } // else exception
}

bool SyntaxAnalyzator::stPriority2() {
    if (stPriority1()) {
        if (isOperation2()) {
            movLexem();
            if (stPriority2()) {
                return true;
            }
        }
        return true;
    } // else exception
}

bool SyntaxAnalyzator::stPriority3() {
    if (stPriority2()) {
        if (isOperation3()) {
            movLexem();
            if (stPriority3()) {
                return true;
            }
        }
        return true;
    } // else exception
}

bool SyntaxAnalyzator::stPriority4() {
    if (stPriority3()) {
        if (isOperation4()) {
            movLexem();
            if (stPriority4()) {
                return true;
            }
        }
        return true;
    } // else exception
}

bool SyntaxAnalyzator::stPriority5() {
    if (stPriority4()) {
        auto [cur, num] = getLexem();
        if (cur == "&") {
            movLexem();
            if (stPriority5()) {
                return true;
            }
        }
        return true;
    } // else exception
}

bool SyntaxAnalyzator::stPriority6() {
    if (stPriority5()) {
        auto [cur, num] = getLexem();
        if (cur == "|") {
            movLexem();
            if (stPriority6()) {
                return true;
            }
        }
        return true;
    } // else exception
}

bool SyntaxAnalyzator::stPriority7() {
    if (stPriority6()) {
        auto [cur, num] = getLexem();
        if (cur == "^") {
            movLexem();
            if (stPriority7()) {
                return true;
            }
        }
        return true;
    } // else exception
}

bool SyntaxAnalyzator::stPriority8() {
    if (stPriority7()) {
        auto [cur, num] = getLexem();
        if (cur == "&&") {
            movLexem();
            if (stPriority8()) {
                return true;
            }
        }
        return true;
    } // else exception
}

bool SyntaxAnalyzator::stPriority9() {
    if (stPriority8()) {
        auto [cur, num] = getLexem();
        if (cur == "||") {
            movLexem();
            if (stPriority9()) {
                return true;
            }
        }
        return true;
    } // else exception
}

bool SyntaxAnalyzator::stPriority10() {
    if (stPriority9()) {
        auto [cur, num] = getLexem();
        if (cur == "=") {
            movLexem();
            if (stPriority10()) {
                return true;
            }
        }
        return true;
    } // else exception
}

bool SyntaxAnalyzator::stExpression() {
    //    auto beforeTriesCell = mainLexer_->getCurrentLexCell();
    //    movLexem();
    //    auto [cur, num] = getLexem();
    //    mainLexer_->jumpToCell(beforeTriesCell);
    //    if (cur == "=") {
    //        if (isName()) {
    //            movLexem();
    //            std::tie(cur, num) = movLexem();
    //            if (cur == "=") {
    //                stPriority9();
    //                return true;
    //            } else {
    //                throw "Syntax error: expected = in assignent"s;
    //            }
    //        } else {
    //            throw "Syntax error: expected name in assignent"s;
    //        }
    //    }
    stPriority10();
    return true;
}

bool SyntaxAnalyzator::stExpressionOperator() {
    stExpression();
    auto [cur, num] = movLexem();
    if (cur != ";") {
        throw "Syntax error: expected ; at the end of expression operator"s;
    }
    return true;
}


bool SyntaxAnalyzator::stConditionalOperator() {
    auto [cur, num] = movLexem();
    if (cur != "if") {
        throw "Syntax error: expected if in conditional (if) operator"s;
    }
    std::tie(cur, num) = movLexem();
    if (cur != "(") {
        throw "Syntax error: expected ( in conditional (if) operator"s;
    }
    stExpression();
    std::tie(cur, num) = movLexem();
    if (cur != ")") {
        throw "Syntax error: expected ) in conditional (if) operator"s;
    }
    stOperator();
    while (true) {
        std::tie(cur, num) = getLexem();
        if (cur != "elif") {
            break;
        }
        movLexem();
        std::tie(cur, num) = movLexem();
        if (cur != "(") {
            throw "Syntax error: expected ( in conditional (elif) operator"s;
        }
        stExpression();
        std::tie(cur, num) = movLexem();
        if (cur != ")") {
            throw "Syntax error: expected ) in conditional (elif) operator"s;
        }
        stOperator();
    }
    std::tie(cur, num) = getLexem();
    if (cur != "else") {
        return true;
    }
    movLexem();
    stOperator();
}
bool SyntaxAnalyzator::stWhileOperator() {
    auto [cur, num] = movLexem();
    if (cur != "while") {
        throw "Syntax error: expected while in while-cycle operator"s;
    }
    std::tie(cur, num) = movLexem();
    if (cur != "(") {
        throw "Syntax error: expected ( in while-cycle operator"s;
    }
    stExpression();
    std::tie(cur, num) = movLexem();
    if (cur != ")") {
        throw "Syntax error: expected ) in while-cycle operator"s;
    }
    stOperator();
    return true;
}

bool SyntaxAnalyzator::stDoWhileOperator() {
    auto [cur, num] = movLexem();
    if (cur != "do") {
        throw "Syntax error: expected do in do-while-cycle operator"s;
    }
    stOperator();
    std::tie(cur, num) = movLexem();
    if (cur != "while") {
        throw "Syntax error: expected while in do-while-cycle operator"s;
    }
    std::tie(cur, num) = movLexem();
    if (cur != "(") {
        throw "Syntax error: expected ( in do-while-cycle operator"s;
    }
    stExpression();
    std::tie(cur, num) = movLexem();
    if (cur != ")") {
        throw "Syntax error: expected ) in do-while-cycle operator"s;
    }
    return true;
}


bool SyntaxAnalyzator::stForOperator() {
    auto [cur, num] = movLexem();
    if (cur != "for") {
        throw "Syntax error: expected for in for-cycle operator"s;
    }
    std::tie(cur, num) = movLexem();
    if (cur != "(") {
        throw "Syntax error: expected ( in for-cycle operator"s;
    }
    stDeclaration();
    stExpression();
    std::tie(cur, num) = movLexem();
    if (cur != ";") {
        throw "Syntax error: expected ; after expression in for-cycle operator"s;
    }
    stExpression();
    std::tie(cur, num) = movLexem();
    if (cur != ")") {
        throw "Syntax error: expected ) after expression in for-cycle operator"s;
    }
    stOperator();
    return true;
}

bool SyntaxAnalyzator::stCycleOperator() {
    auto [cur, num] = getLexem();
    if (cur == "for") {
        stForOperator();
    } else if (cur == "while") {
        stWhileOperator();
    } else if (cur == "do") {
        stDoWhileOperator();
    } else {
        throw "Syntax error: expected cycle"s;
    }
}

bool SyntaxAnalyzator::stSection() {
    if (isName()) {
        movLexem();
        auto [cur, num] = getLexem();
        if (cur == "=") {
            movLexem();
            stExpression();
            return true;
            //std::cout << "a!!!!" << std::endl;
        } else if (cur == "[") {
            movLexem();
            stExpression();
            std::tie(cur, num) = movLexem();
            if (cur != "]") {
                throw "Syntax error: expected ] at the end of section"s;
            }
            return true;
        } else {
            return true;
        }
    }
    throw "Syntax error: expected section but haven't found"s;
}

bool SyntaxAnalyzator::stDeclaration() {
    if (!isType()) {
        throw "Syntax error: expected typename in declaration"s;
    }
    movLexem();
    if (!stSection())
        return false;
    while (true) {
        auto [cur, num] = getLexem();
        if (cur == ";") {
            movLexem();
            return true;
        }
        if (cur == ",") {
            movLexem();
            stSection();
            continue;
        } else {
            throw "Syntax error: expected another section or end of declaration but neither is found"s;
        }
    }
}

bool SyntaxAnalyzator::stFunction() {
    auto [cur, num] = getLexem();
    if (!isType() && cur != "void") {
        throw "Syntax error: didn't find return's type of function"s;
    }
    movLexem();
    if (!isName()) {
        throw "Syntax error: didn't find name of function"s;
    }
    movLexem();
    std::tie(cur, num) = movLexem();
    if (cur != "(") {
        throw "Syntax error: expected ( in declaration of function"s;
    }
    if (!isType()) {
        std::tie(cur, num) = movLexem();
        if (cur != ")") {
            "Syntax error: expected ) in declaration of function"s;
        } else {
            return stCompoundOperator();
        }
    } else {
        movLexem();
        if (!isName()) {
            throw "Syntax error: expected name in declaration (input) of function"s;
        }
        movLexem();
    }
    while (true) {
        std::tie(cur, num) = movLexem();
        if (cur == ")")
            return stCompoundOperator();
        if (cur == ",") {
            if (!isType())
                throw "Syntax error: expected type in declaration (input) of function"s;
            movLexem();
            if (!isName())
                throw "Syntax error: expected name in declaration (input) of function"s;
            movLexem();
        } else {
            throw "Syntax error: expected , or ) in declaration of function"s;
        }
    }
}

bool SyntaxAnalyzator::stFunctionCallOperator() {
    if (!isName()) {
        throw "Syntax error: expected name of function in function call"s;
    }
    movLexem();
    stFunctionTail();
    auto [cur, num] = movLexem();
    if (cur != ";") {
        throw "Syntax error: expected ; after function call"s;
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
                std::tie(cur, num) = movLexem();
                if (cur != "(") {
                    throw "Syntax error: expected ( in main call"s;
                }
                std::tie(cur, num) = movLexem();
                if (cur != ")") {
                    throw "Syntax error: expected ) in main call"s;
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
            stFunction();
        } else {
            stDeclaration();
        }
    }
}

bool SyntaxAnalyzator::stOperator() {
    auto [cur, num] = getLexem();
    if (isType()) {
        stDeclaration();
        return true;
    }
    if (cur == "{") {
        stCompoundOperator();
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

bool SyntaxAnalyzator::stCompoundOperator() {
    auto [cur, num] = movLexem();
    if (cur != "{") {
        throw "Syntax error: expected { at the beginning of compound operator"s;
    }
    while (true) {
        std::tie(cur, num) = getLexem();
        if (cur == "}") {
            movLexem();
            return true;
        }
        stOperator();
    }
    // else exception in stOperator()
}

std::string SyntaxAnalyzator::work() {
    try {
        auto s = stProgram();
        if (getLexem().second != 0)
            throw "Syntax error: something after main detected"s;
        return s ? "OK" : "How are we here? This is error";
    } catch (std::string s) {
        return s;
    }
}

SyntaxAnalyzator::SyntaxAnalyzator(LexicalAnalyzator *&mainLexer): mainLexer_(mainLexer) {
}
