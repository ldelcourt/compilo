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


  FunctionVisitor(bool debug = false, bool symbol = false) : ifccBaseVisitor(), debug(debug), symbol(symbol), error(false) {

    Function *f = new Function();
    f->name = "putchar";
    f->typeReturn = Type::VOID;
    f->params.push_back(Type::INT);

    functions.push_back(f);

    f = new Function();
    f->name = "getchar";
    f->typeReturn = Type::INT;

    functions.push_back(f);
  }
    
  virtual ~FunctionVisitor() {

    for (CFG* cfg : tab) {
      delete cfg;
    }
    for (Function *f : functions) {
      delete f;
    }
  }


  virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override;

  virtual antlrcpp::Any visitDeclaration_function(ifccParser::Declaration_functionContext *ctx) override;

  virtual antlrcpp::Any visitDefinition_function(ifccParser::Definition_functionContext *ctx) override;


  virtual antlrcpp::Any visitParametres(ifccParser::ParametresContext *ctx) override;
  virtual antlrcpp::Any visitParametre(ifccParser::ParametreContext *ctx) override;

  virtual antlrcpp::Any visitFunction(ifccParser::FunctionContext *ctx) override;
  virtual antlrcpp::Any visitStatement(ifccParser::StatementContext *ctx) override;
  virtual antlrcpp::Any visitCall_function(ifccParser::Call_functionContext *ctx) override;
  

  bool hasError() {
    return error;
  }

  void gen_asm(std::ostream &o);
  
private:

  std::vector<CFG*> tab;
  bool debug, symbol, error;

  std::vector<Function*> functions;


  int function_exist(const std::string &name);
  
};


inline int FunctionVisitor::function_exist(const std::string &name) {

  for (int i = 0; i < functions.size(); i++) {

    if (functions[i]->name == name) {
      return i;
    }
    
  }

  return -1;

}

