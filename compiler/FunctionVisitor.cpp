#include "FunctionVisitor.h"


#define getTypeReturn(f)		\
  switch(ctx->TYPE_RETURN) {	\
  case 'int':			\
    f->typeReturn = Type::INT;		\
    break;				\
  case 'void':			\
    f->typeReturn = Type::VOID;		\
    break;				\
  } 


antlrcpp::Any FunctionVisitor::visitProg(ifccParser::ProgContext *ctx) {

  tab.push_back(new CFG(ctx, debug, symbol));

  Function *f = new Function();
  f->name = "main";

  
  
}


antlrcpp::Any FunctionVisitor::visitDeclaration_function(ifccParser::Declaration_functionContext *ctx) {


}
    


antlrcpp::Any FunctionVisitor::visitDefinition_function(ifccParser::Definition_functionContext *ctx) {

  tab.push_back(new CFG(ctx, debug, symbol));

  Function *f = new Function();
  f->name = ctx->VAR()->getText();
  getTypeReturn(f);


  f->params = this->visit(ctx->parametre());

  
  
}

antlrcpp::Any FunctionVisitor::visitParametre(ifccParser::ParametreContext *ctx) {

  std::vector<Type> params;

  for (int i = 0; i < ctx->VAR().size(); i++) {

  }

}
