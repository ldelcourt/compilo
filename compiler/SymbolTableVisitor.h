#pragma once


#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"
#include "SymbolTable.h"

using namespace std;

class  SymbolTableVisitor : public ifccBaseVisitor {
	public:
    SymbolTable &symbolTable;
    SymbolTableVisitor(SymbolTable &symbolTable) : symbolTable(symbolTable) {}
    bool error = false;
    virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override ;
    virtual antlrcpp::Any visitVar_declaration(ifccParser::Var_declarationContext *ctx) override;

    private:
};

