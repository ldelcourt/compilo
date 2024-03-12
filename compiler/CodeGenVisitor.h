#pragma once


#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"
#include "SymbolTable.h"


class  CodeGenVisitor : public ifccBaseVisitor {
	public:
        SymbolTable &symbolTable;
        CodeGenVisitor(SymbolTable &symbolTable) : symbolTable(symbolTable) {}
        virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override ;
        virtual antlrcpp::Any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override;
        virtual antlrcpp::Any visitVar_affectation(ifccParser::Var_affectationContext *ctx) override; //a = 5;
        virtual antlrcpp::Any visitVar_initialisation(ifccParser::Var_initialisationContext *ctx) override; // int a = 5;
        virtual antlrcpp::Any visitAddsub(ifccParser::AddsubContext *ctx) override; // expr '+/-' expr
        virtual antlrcpp::Any visitMultdiv(ifccParser::MultdivContext *ctx) override; // expr '*/' expr
        virtual antlrcpp::Any visitPar(ifccParser::ParContext *ctx) override; // '(' expr ')'
        virtual antlrcpp::Any visitConst(ifccParser::ConstContext *ctx) override; // CONST
        virtual antlrcpp::Any visitVar(ifccParser::VarContext *ctx) override; // VARNAME
};

