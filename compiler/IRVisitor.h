#pragma once


#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"

#include "IR.h"

#include <vector>
#include <string>



class  IRVisitor : public ifccBaseVisitor {
public:


  IRVisitor(CFG *cfg) : ifccBaseVisitor(), cfg(cfg) {}

  virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override ;

  //Return 
  virtual antlrcpp::Any visitReturnConst(ifccParser::ReturnConstContext *ctx) override;
  virtual antlrcpp::Any visitReturnVar(ifccParser::ReturnVarContext *ctx) override;
  virtual antlrcpp::Any visitReturnExpr(ifccParser::ReturnExprContext *ctx) override;

  //Variables 
  virtual antlrcpp::Any visitVarToVar(ifccParser::VarToVarContext *ctx) override;
  virtual antlrcpp::Any visitVarToConst(ifccParser::VarToConstContext *ctx) override;
  virtual antlrcpp::Any visitVarToExpr(ifccParser::VarToExprContext *ctx) override;
  
  virtual antlrcpp::Any visitVarInitVar(ifccParser::VarInitVarContext *ctx) override;
  virtual antlrcpp::Any visitVarInitConst(ifccParser::VarInitConstContext *ctx) override;
  virtual antlrcpp::Any visitVarInitExpr(ifccParser::VarInitExprContext *ctx) override;
  
  

  //Expressions
  virtual antlrcpp::Any visitPar(ifccParser::ParContext *ctx) override;
  virtual antlrcpp::Any visitMultdivmod(ifccParser::MultdivmodContext *ctx) override;
  virtual antlrcpp::Any visitUnaire(ifccParser::UnaireContext *ctx) override;
  virtual antlrcpp::Any visitPlusmoins(ifccParser::PlusmoinsContext *ctx) override;
  virtual antlrcpp::Any visitConst(ifccParser::ConstContext *ctx) override;
  virtual antlrcpp::Any visitVar(ifccParser::VarContext *ctx) override;
  virtual antlrcpp::Any visitFunction(ifccParser::FunctionContext *ctx) override;

  //Appel à des fonctions
  virtual antlrcpp::Any visitCall_function(ifccParser::Call_functionContext *ctx) override;

  
private:

  CFG *cfg;
  
};

