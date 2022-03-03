//
// Created by maincpp on 21.01.2022.
//

#ifndef COMPILER_SYNTAXANALYZATOR_H
#define COMPILER_SYNTAXANALYZATOR_H
#include "LexicalAnalyzator.h"
#include "tid.h"

class SyntaxAnalyzator {
public:
    // memorize the string lexer on and index
    explicit SyntaxAnalyzator(LexicalAnalyzator *& mainLexer);
    std::pair<std::string, int> movLexem();
    std::pair<std::string, int> getLexem();
    [[nodiscard]] std::string errLastLex() const;
    [[nodiscard]] std::string errCurLex() const;
    [[nodiscard]] std::pair<int, int> curWhere() const;
    std::string work();
    LexicalAnalyzator* mainLexer_;
private:
    bool isType();
    bool isBool();
    bool isNumber();
    bool isSign();
    bool isIncrement();
    bool isOperation1();
    bool isOperation2();
    bool isOperation3();
    bool isOperation4();
    bool isDelimiter();
    bool isName(bool declar);
    bool stExpression();
    bool stReturnOperator();
    bool stInputOperator();
    bool stOutputOperator();
    bool stBreakOperator();
    bool stContinueOperator();
    bool stInitAtom();
    bool stAtom();
    bool stFunctionTail();
    bool stArrayTail();
    bool stPriority0();
    bool stPriority1();
    bool stPriority2();
    bool stPriority3();
    bool stPriority4();
    bool stPriority5();
    bool stPriority6();
    bool stPriority7();
    bool stPriority8();
    bool stPriority9();
    bool stPriority10();
    bool stExpressionOperator();
    bool stOperator();
    bool stCompoundOperator();
    bool stConditionalOperator();
    bool stWhileOperator();
    bool stDoWhileOperator();
    bool stForOperator();
    bool stDeclaration();
    bool stCycleOperator();
    bool stSection(bool declar);
    bool stFunction(bool declar);
    bool stFunctionCallOperator();
    bool stProgram();
};


#endif //COMPILER_SYNTAXANALYZATOR_H
