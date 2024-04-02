#pragma once


#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"


#include <unordered_map>
#include <string>

#include "SymbolTable.h"

class  VarVisitor : public ifccBaseVisitor {
public:

  VarVisitor(SymbolTable *table) : ifccBaseVisitor(), table(table), error(false) {}
  ~VarVisitor() {}
  
  virtual antlrcpp::Any visitVarToVar(ifccParser::VarToVarContext *ctx) override;
  virtual antlrcpp::Any visitVarToConst(ifccParser::VarToConstContext *ctx) override;
  virtual antlrcpp::Any visitVarToExpr(ifccParser::VarToExprContext *ctx) override;
  
  virtual antlrcpp::Any visitDeclaration_var(ifccParser::Declaration_varContext *ctx) override;

  virtual antlrcpp::Any visitReturnVar(ifccParser::ReturnVarContext *ctx) override;
  virtual antlrcpp::Any visitReturnExpr(ifccParser::ReturnExprContext *ctx) override;
  
  virtual antlrcpp::Any visitVarInitVar(ifccParser::VarInitVarContext *ctx) override;
  virtual antlrcpp::Any visitVarInitConst(ifccParser::VarInitConstContext *ctx) override;
  virtual antlrcpp::Any visitVarInitExpr(ifccParser::VarInitExprContext *ctx) override;
  virtual antlrcpp::Any visitVarInitAffect(ifccParser::VarInitAffectContext *ctx) override;

  virtual antlrcpp::Any visitVar(ifccParser::VarContext *ctx) override;
  virtual antlrcpp::Any visitBlock(ifccParser::BlockContext *ctx) override;

  virtual antlrcpp::Any visitCall_function(ifccParser::Call_functionContext *ctx) override;

  bool hasError() const;

  void checkUnusedDecla();
  std::pair<bool, std::string> checkVarInAllBlocks(std::string name);
  

private:

  SymbolTable *table;
  std::unordered_map<std::string, bool> used;
  bool error;
  std::string currentBlock;
  int blockCount = 0;
  
  
  
};


inline bool VarVisitor::hasError() const {
  return error;
}
