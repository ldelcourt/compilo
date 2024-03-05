#pragma once


#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"
#include <cstdlib>

using namespace std;

class  SymbolTableVisitor : public ifccBaseVisitor {
	public:
    map<string, int> &symbolTable;
    SymbolTableVisitor(map<string, int> &symbolTable) : symbolTable(symbolTable) {}
    
    virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override ;
    virtual antlrcpp::Any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override;
    virtual antlrcpp::Any visitVar_declaration(ifccParser::Var_declarationContext *ctx) override;
};

