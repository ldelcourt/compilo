#include "FunctionVisitor.h"


#define getType(varTest, type)		\
  if (varTest == "int") {		\
    type = Type::INT;			\
  }				\
  else if (varTest == "void") {		\
    type = Type::VOID;			\
  } 


#define printPosSymbol(symbol)				\
  symbol->getSymbol()->getLine() << ":" << symbol->getSymbol()->getCharPositionInLine() << " "

antlrcpp::Any FunctionVisitor::visitProg(ifccParser::ProgContext *ctx) {

  Function *f = new Function();
  f->name = "main";

  if (function_exist(f->name) != -1) {
    error = true;
    std::cout << "error: function " << f->name << " is already declared\n";

    return 1;
  }

  try {
      tab.push_back(new CFG(ctx, debug, symbol));
  } catch (int e) {

    error = true;
    return e;
  }

  this->visit(ctx->block());

  return 0;
  
}


antlrcpp::Any FunctionVisitor::visitDeclaration_function(ifccParser::Declaration_functionContext *ctx) {


  Function *f = new Function();
  f->name = ctx->VAR()->getText();

  if (function_exist(f->name) != -1) {
    error = true;
    std::cout << "error: " << printPosSymbol(ctx->VAR()) << " function " << f->name << " is already declared\n";

    return 1;
  }
  
  getType(ctx->TYPE_RETURN->getText(), f->typeReturn);


  if (ctx->parametres() != nullptr) {
    f->params = (std::vector<Type>)(this->visit(ctx->parametres()));
  }

  functions.push_back(f);

  return 0;

}
    


antlrcpp::Any FunctionVisitor::visitDefinition_function(ifccParser::Definition_functionContext *ctx) {

  

  Function *f = new Function();
  f->name = ctx->VAR()->getText();

  if (function_exist(f->name) != -1) {
    error = true;
    std::cout << "error: " << printPosSymbol(ctx->VAR()) << " function " << f->name << " is already declared\n";

    return 1;
  }
  
  getType(ctx->TYPE_RETURN->getText(), f->typeReturn);


  if (ctx->parametres() != nullptr) {
    f->params = (std::vector<Type>)(this->visit(ctx->parametres()));
  }

  functions.push_back(f);

  try {
    tab.push_back(new CFG(ctx, debug, symbol));
  } catch (int e) {
    error = true;
    return e;
  }

  this->visit(ctx->block());

  return 0;
  
}

antlrcpp::Any FunctionVisitor::visitParametres(ifccParser::ParametresContext *ctx) {

  std::vector<Type> params;

  for (int i = 0; i < ctx->parametre().size(); i++) {

    params.push_back((Type)(this->visit(ctx->parametre(i))));

  }

  return params;
  
}

antlrcpp::Any FunctionVisitor::visitParametre(ifccParser::ParametreContext *ctx) {

  Type t;
  getType(ctx->TYPE->getText(), t);
  return t;

}


antlrcpp::Any FunctionVisitor::visitFunction(ifccParser::FunctionContext *ctx) {

  int i = (int)visit(ctx->call_function());

  if (i >= 0 && functions[i]->typeReturn == Type::VOID) {

    std::cout << "error : " << printPosSymbol(ctx->call_function()->VAR()) << " function " << functions[i]->name << " doesn't have return statement\n";

    error = true;

    return 1;
  }

  return 0;
    
}



antlrcpp::Any FunctionVisitor::visitStatement(ifccParser::StatementContext *ctx) {

  if (ctx->call_function() != nullptr) {
  

    int i = (int)visit(ctx->call_function());

    if (i >= 0 && functions[i]->typeReturn != Type::VOID) {

      std::cerr << "warning : " << printPosSymbol(ctx->call_function()->VAR()) << " function " << functions[i]->name <<" has probably return statement\n";

      return 2;
    
    }

    return 0;

  }

  else {

    return visitChildren(ctx);
  }
  
}


antlrcpp::Any FunctionVisitor::visitCall_function(ifccParser::Call_functionContext *ctx) {


  //Recuperation de la Function appelé
  int i = function_exist(ctx->VAR()->getText());
  if ( i == -1) {
    std::cout << "error (implicit declaration) : " << printPosSymbol(ctx->VAR()) << " function " << ctx->VAR()->getText() << " is called but never declared\n";

    error = true;

    return -2;
  }
  
  
  //Verification du bon nombre de paramètres (pas de types pour le moment)
  if (ctx->expr().size() != functions[i]->params.size()) {
    error = true;
    std::cout << "error: " << printPosSymbol(ctx->VAR()) <<  " function " << ctx->VAR()->getText() << " requires " << functions[i]->params.size() << " parameters\n";

    return -1;
  }

  return i;
  

}
  


void FunctionVisitor::gen_asm(std::ostream &o) {

  for (CFG* cfg : tab) {
    cfg->gen_asm(o);
  }
}
