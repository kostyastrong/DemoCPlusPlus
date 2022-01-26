//
// Created by maincpp on 21.12.2021.
//

#include <sstream>
#include "LexicalAnalyzator.h"

const std::string LEXPATH = "../LexicalInfo/";
const std::string reservedDefPath = LEXPATH + "reservedWords.txt";
const std::string inputDefPath = LEXPATH + "inputLexical.txt";
const std::string outputDefPath = LEXPATH + "outputLexical.txt";
const std::string devidersDefPath = LEXPATH + "wordDeviders.txt";
const std::string statesType = LEXPATH + "statesType.txt";
const std::string statesTransactions = LEXPATH + "lexicalSettings.txt";
const int stateNotExist = -100;
const int stateNotEnd = -1;

std::unordered_set<std::string>* LexicalAnalyzator::getWords(const std::string& reserved) {  // get reserved words
    std::unordered_set<std::string>* ret = new std::unordered_set<std::string>;
    std::ifstream inFile;
    inFile.open(reserved);
    std::string x;
    if (!inFile) {
        std::cout << "Unable to open file with ReservedWords\n";
        return nullptr;
    }
    while (inFile >> x) {
        ret->insert(x);
    }
    inFile.close();
    return ret;
}

std::unordered_set<char>* LexicalAnalyzator::getChars(const std::string& reserved) {
    std::unordered_set<char>* ret = new std::unordered_set<char>;
    std::ifstream inFile;
    inFile.open(reserved);
    char x;
    if (!inFile) {
        std::cout << "Unable to open file with ReservedWords\n";
        return nullptr;
    }
    while (inFile >> x) {
        //std::cout << x;
        ret->insert(x);
    }
    //std::cout << '\n';
    inFile.close();
    return ret;
}

void LexicalAnalyzator::splitFile(const std::string& reserved,
                                  const std::string& inPath,
                                  const std::string& outPath) {
    std::unordered_set<std::string>* reservedWords = getWords(reserved);
    std::unordered_set<char>* deviders = getChars(devidersDefPath);
    deviders->insert(' ');
    std::ifstream inFile;
    inFile.open(inPath);
    int indexOfString = 1;
    while (std::getline(inFile, curString_)) {
        ind_ = 0;
        //std::cout << curString_[0];
        //exit(0);
        while (ind_ < curString_.size()) {
            State* tmp = states_[0]->jump_[curString_[ind_]];
            int deb = curString_[ind_];
            try {
                if (! tmp) throw curString_[ind_];
                curType_ = tmp->type_;
            } catch (char e) {
                std::cout << "Unreadable symbol:" << e << std::endl;
            }
            if (curType_ == 0) {
                ++ind_;
                continue;
            } else if (curType_ == 8) {
                ind_ = curString_.size();
                break;  // commentaries
            }
            if (ind_ >= curString_.size()) break;
            while (tmp != nullptr && tmp->type_ == curType_) {
                lexem_ += curString_[ind_];
                ++ind_;
                if (ind_ <= curString_.size() - 1) {
                    int indLetter = curString_[ind_];
                    char deb = curString_[ind_];
                    tmp = tmp->jump_[indLetter];
                } else break;
            }
            if (curType_ == 2) {
                if (reservedWords->find(lexem_) != reservedWords->end()) {
                    curType_ = 1;
                }
            }
            if (tmp && tmp->type_ == 10) {
                lexem_ += curString_[ind_];
                ++ind_;
            }
            if (curType_ < 0) {
                if (tmp->type_ == 10) {
                    curType_ = -curType_;
                } else {
                    std::cout << "An unfinished lexem in the end of the line: " << indexOfString << std::endl;
                    exit(0);
                }
            }
            if (! memory_) {
                memory_ = new Cell(lexem_, curType_);
                lastWrite_ = memory_;
            } else {
                lastWrite_->next = new Cell(lexem_, curType_);
                lastWrite_->next->prev = lastWrite_;
                lastWrite_ = lastWrite_->next;
            }
            std::cout << lexem_ << ' ' << curType_ << '\n';
            curType_ = 0;
            lexem_ = "";
        }
        ++indexOfString;
    }
}

std::pair<std::string, int> LexicalAnalyzator::getLexem() {
    if (! memory_) return {nullptr, 0};
    std::pair<std::string, int> ret = {memory_->lexem_, memory_->t_};
    memory_ = memory_->next;
    return ret;
}

Cell* LexicalAnalyzator::getCurrentLexCell() {
    return memory_;
}

void LexicalAnalyzator::jumpToCell(Cell *memorizedCell) {
    memory_ = memorizedCell;
}

LexicalAnalyzator::LexicalAnalyzator(const std::string& sourceTrans, const std::string& sourceTypes) {
    for (State*& i : states_) {
        i = new State;
    }
    for (int& i : statesType_) {  // why not *?
        i = 0;
    }
    statesType_[0] = 0;
    fillStates(statesTransactions);  // shouldn't be that way
    fillTypes(statesType);

    deviders_ = getChars(devidersDefPath);
    deviders_->insert(' ');
    states_[20]->jump_[' '] = states_[20];
    reservedWords_ = getWords(reservedDefPath);
}

void LexicalAnalyzator::fillStates(const std::string &source) {
    int current, further;
    std::string symbols;
    std::ifstream inFile;
    std::cerr << source << std::endl;
    inFile.open(source);
    //std::cout << statesTransactions << std::endl;
    if (!inFile) {
        std::cout << "Unable to open file with states\n";
        throw "The file for filling states is garbage!";
    }
    while (inFile >> current) {
        inFile >> further;
        inFile >> symbols;
        for (int i : symbols) {
            states_[current]->jump_[i] = states_[further];
        }
    }
    states_[0]->jump_[' '] = states_[0];
    states_[0]->jump_['\t'] = states_[0];
}

std::pair<int, int> coupleFromString(const std::string& s) {
    int x1 = stateNotExist, x2 = x1;
    std::stringstream ss;
    ss << s;
    std::string temp;
    int cur;
    while (!ss.eof()) {
        ss >> temp;
        if (std::stringstream(temp) >> cur) {
            if (x1 == stateNotExist) x1 = cur;
            else x2 = cur;
        }
        temp = "";
    }
    return {x1, x2};
}

void LexicalAnalyzator::fillTypes(const std::string &source) {
    int state, type;
    std::string info;
    std::ifstream inFile;
    inFile.open(source);
    if (!inFile) {
        std::cout << "Unable to open file with ReservedWords\n";
        throw "The file for filling types is garbage!";
    }
    while (std::getline(inFile, info)) {
        std::pair<int, int> cur = coupleFromString(info);
        if (cur.first == -100 && cur.second == -100) continue;
        statesType_[cur.first] = cur.second;
        states_[cur.first]->type_ = cur.second;
    }
}

void LexicalAnalyzator::retLex() {
    memory_ = memory_->prev;
}
