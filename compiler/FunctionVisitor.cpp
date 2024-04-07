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



FunctionVisitor::FunctionVisitor(bool debug, bool symbol) : ifccBaseVisitor(), debug(debug), symbol(symbol), error(false) {

  //Ajout de getchar et putchar comme fonction connu
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

FunctionVisitor::~FunctionVisitor() {

  //Suppression des cfg et des fonctions
  for (CFG* cfg : tab) {
    delete cfg;
  }
  for (Function *f : functions) {
    delete f;
  }
  
}


antlrcpp::Any FunctionVisitor::visitProg(ifccParser::ProgContext *ctx) {

  //fonction main
  Function *f = new Function();
  f->name = "main";

  if (function_exist(f->name) != -1) {
    error = true;
    std::cout << "error: function " << f->name << " is already declared\n";

    return 1;
  }

  try {
    tab.push_back(new CFG(ctx, f->name, debug, symbol));
  } catch (int e) {

    error = true;
    return e;
  }

  //visite des block pour trouver les call
  this->visit(ctx->block());

  return 0;
  
}


antlrcpp::Any FunctionVisitor::visitDeclaration_function(ifccParser::Declaration_functionContext *ctx) {

  //Declaration de fonction => uniquement ajout du prototype, pas de cfg
  Function *f = new Function();
  f->name = ctx->VAR()->getText();

  if (function_exist(f->name) != -1) {
    error = true;
    std::cout << "error: " << printPosSymbol(ctx->VAR()) << " function " << f->name << " is already declared\n";

    return 1;
  }
  
  getType(ctx->TYPE_RETURN->getText(), f->typeReturn);


  //visite des paramètres pour trouver les types
  if (ctx->parametres() != nullptr) {
    f->params = (std::vector<Type>)(this->visit(ctx->parametres()));
  }

  functions.push_back(f);

  return 0;

}
    


antlrcpp::Any FunctionVisitor::visitDefinition_function(ifccParser::Definition_functionContext *ctx) {

  
  //Définition de fonction => structure function + cfg
  Function *f = new Function();
  f->name = ctx->VAR()->getText();

  if (function_exist(f->name) != -1) {
    error = true;
    std::cout << "error: " << printPosSymbol(ctx->VAR()) << " function " << f->name << " is already declared\n";

    return 1;
  }
  
  getType(ctx->TYPE_RETURN->getText(), f->typeReturn);


  //visite des paramètres pour trouver les types
  if (ctx->parametres() != nullptr) {
    f->params = (std::vector<Type>)(this->visit(ctx->parametres()));
  }

  functions.push_back(f);

  try {
    tab.push_back(new CFG(ctx, f->name, debug, symbol));
  } catch (int e) {
    error = true;
    return e;
  }

  //visite dans le block pour trouver les call
  this->visit(ctx->block());

  return 0;
  
}

antlrcpp::Any FunctionVisitor::visitParametres(ifccParser::ParametresContext *ctx) {

  std::vector<Type> params;

  for (int i = 0; i < ctx->parametre().size(); i++) {

    params.push_back((Type)(this->visit(ctx->parametre(i))));

  }

  //un vector contenant les types des paramètres d'une fonction
  return params;
  
}

antlrcpp::Any FunctionVisitor::visitParametre(ifccParser::ParametreContext *ctx) {

  //le type du paramètre
  Type t;
  getType(ctx->TYPE->getText(), t);
  return t;

}


antlrcpp::Any FunctionVisitor::visitFunction(ifccParser::FunctionContext *ctx) {

  //appel de fonction dans une expression 
  
  int i = (int)visit(ctx->call_function());

  //en cas de type de retour void on signale une erreur
  if (i >= 0 && functions[i]->typeReturn == Type::VOID) {

    std::cout << "error : " << printPosSymbol(ctx->call_function()->VAR()) << " function " << functions[i]->name << " doesn't have return statement\n";

    error = true;

    return 1;
  }

  return 0;
    
}



antlrcpp::Any FunctionVisitor::visitStatement(ifccParser::StatementContext *ctx) {

  if (ctx->call_function() != nullptr) {

    //appel d'une fonction dans une instruction

    int i = (int)visit(ctx->call_function());

    //en cas de retour void on fait un warning
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
