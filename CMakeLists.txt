cmake_minimum_required(VERSION 3.20)
project(DemoCPlusPlus)

set(CMAKE_CXX_STANDARD 20)

add_executable(DemoCPlusPlus main.cpp LexicalAnalyzator.cpp LexicalAnalyzator.h SyntaxAnalyzator.cpp SyntaxAnalyzator.h Exception.cpp Exception.h tid.cpp tid.h typeStack.cpp typeStack.h polish.cpp polish.h)
add_executable(test test.cpp)
include_directories("/LexicalInfo")