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
    if(symbolTable.addSymbol(declaredVarName) == -1) error = true;
}

// a = 5;
antlrcpp::Any SymbolTableVisitor::visitVar_affectation(ifccParser::Var_affectationContext *ctx)
{
  string declaredVarName = ctx->VARNAME(0)->getText();
  if(symbolTable.checkDeclaration(declaredVarName) == -1) error = true;

  if(ctx->CONST() != nullptr) {
  }

  if(ctx->VARNAME(1) != nullptr) {
    string assignedVarName = ctx->VARNAME(1)->getText();
    if(symbolTable.isUsed(assignedVarName) == -1) error = true;
  }

  return 0;
}

// int a = 5;
antlrcpp::Any SymbolTableVisitor::visitVar_initialisation(ifccParser::Var_initialisationContext *ctx)
{
  string declaredVarName = ctx->VARNAME(0)->getText();
  if(symbolTable.addSymbol(declaredVarName) == -1) error = true;
  
  // Cas VARNAME = VARNAME
  if (ctx->VARNAME(1) != nullptr) {
    string assignedVarName = ctx->VARNAME(1)->getText();
    if(symbolTable.isUsed(assignedVarName) == -1) error = true;
  }

  // Cas VARNAME = CONST
  if(ctx->CONST() != nullptr) {
  }

  return 0;
}