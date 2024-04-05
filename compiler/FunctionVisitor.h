#pragma once


#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"

#include <vector>
#include <string>

#include "SymbolTable.h"
#include "IR.h"


typedef struct {
  
  std::string name;
  Type typeReturn;
  std::vector<Type> params;

} Function;


class  FunctionVisitor : public ifccBaseVisitor {
public:


  FunctionVisitor(bool debug = false, bool symbol = false) : ifccBaseVisitor(), debug(debug), symbol(symbol) {}


  virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override;

  virtual antlrcpp::Any visitDeclaration_function(ifccParser::Declaration_functionContext *ctx) override;

  virtual antlrcpp::Any visitDefinition_function(ifccParser::Definition_functionContext *ctx) override;

  virtual antlrcpp::Any visitParametre(ifccParser::ParametreContext *ctx) override;
  
private:

  std::vector<CFG*> tab;
  bool debug, symbol;

  std::vector<Function*> functions;
  
};

