//
// Created by maincpp on 21.01.2022.
//

#ifndef COMPILER_SYNTAXANALYZATOR_H
#define COMPILER_SYNTAXANALYZATOR_H
#include "LexicalAnalyzator.h"

class SyntaxAnalyzator {
public:
    // memorize the string lexer on and index
    explicit SyntaxAnalyzator(LexicalAnalyzator *& mainLexer);
    std::pair<std::string, int> getLexem();
    bool work();
    LexicalAnalyzator* mainLexer_;
private:
    bool stBool();
    bool stNumber();
    bool stDelimiter();
    bool stName();
    bool stType();
    bool stBreak();
    bool stContinue();
    bool stOutput();
    bool stInput();
    bool stReturn();
    bool stOperation1();
    bool stOperation2();
    bool stOperation3();
    bool stOperation4();
    bool stAtom();
    bool stInitAtom();
    bool stExpression();
    bool stPriority0();
    bool stPriority1();
    bool stPriority2();
    bool stPriority3();
    bool stPriority4();
    bool stIncrement();
    bool stPriority5();
    bool stPriority6();
    bool stPriority7();
    bool stInitAtomBool();
    bool stAtomBool();
    bool stPriority1Bool();
    bool stPriority2Bool();
    bool stPriority3Bool();
    bool stPriority4Bool();
    bool stExpressionOperator();
    bool stCycleOperator();
    bool stForOperator();
    bool stForAutoOperator();
    bool stWhileOperator();
    bool stDoWhileOperator();
    bool stOperator();
    bool stDeclaration();
    bool stConditionalOperator();
    bool stSection();
    bool stFunction();
    bool stFunctionCall();
    bool stProgram();
    bool stCompoundOperator();



};


#endif //COMPILER_SYNTAXANALYZATOR_H
