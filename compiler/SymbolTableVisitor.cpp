#include "SymbolTableVisitor.h"

antlrcpp::Any SymbolTableVisitor::visitProg(ifccParser::ProgContext *ctx) 
{
  this->visitChildren(ctx);
  if(symbolTable.checkUsage() == -1) error = true;

  return 0;
}

// int a;
antlrcpp::Any SymbolTableVisitor::visitVar_declaration(ifccParser::Var_declarationContext *ctx)
{
    string declaredVarName = ctx->VARNAME()->getText();
    if(symbolTable.addSymbol(declaredVarName) == 1) error = true;

    return 0;
}

// a = 5;
antlrcpp::Any SymbolTableVisitor::visitVar_affectation(ifccParser::Var_affectationContext *ctx)
{
  string declaredVarName = ctx->VARNAME()->getText();
  if(symbolTable.checkDeclaration(declaredVarName) == -1) error = true;
  this->visitChildren(ctx->expression());
  return 0;
}

// int a = 5;
antlrcpp::Any SymbolTableVisitor::visitVar_initialisation(ifccParser::Var_initialisationContext *ctx)
{
  string declaredVarName = ctx->VARNAME()->getText();
  if(symbolTable.addSymbol(declaredVarName) == -1) error = true;
  this->visitChildren(ctx->expression());
  return 0;
}

antlrcpp::Any SymbolTableVisitor::visitVar(ifccParser::VarContext *ctx)
{
  string varName = ctx->VARNAME()->getText();
  if(symbolTable.isUsed(varName) == -1) error = true;

  return 0;
}