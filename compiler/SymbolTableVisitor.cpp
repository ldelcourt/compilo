#include "SymbolTableVisitor.h"

antlrcpp::Any SymbolTableVisitor::visitProg(ifccParser::ProgContext *ctx) 
{
  this->visitChildren(ctx);
  if(symbolTable.checkUsage() == -1) error = true;

  return 0;
}

antlrcpp::Any SymbolTableVisitor::visitVar_declaration(ifccParser::Var_declarationContext *ctx)
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
    if(symbolTable.isUsed(declaredVarName) == -1) error = true;
  }

  return 0;
}
