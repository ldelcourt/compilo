#include "VarVisitor.h"

#include <iostream>

#define printPosSymbol(symbol)				\
  symbol->getSymbol()->getLine() << ":" << symbol->getSymbol()->getCharPositionInLine() << " "

#define checkExpr(element)			\
  bool copieError = error;		\
  error = false;			\
  visit(element);			\
  if (error) {			\
    return 1;			\
  }				\
  else {				\
    error = copieError;			\
  }




antlrcpp::Any VarVisitor::visitVarToVar(ifccParser::VarToVarContext *ctx) {

  for (int i = 0; i+1 < ctx->VAR().size(); i++) {
    
    std::string var1 (ctx->VAR(i)->getText());
    std::string var2 (ctx->VAR(i+1)->getText());

    //Verif variables déclarés
    if (! table->contains(var1)) {
      std::cerr << "error : " << printPosSymbol(ctx->VAR(i)) <<  var1 << " was never declared" << std::endl;
      error = true;

      
    }

    if (! table->contains(var2)) {
      std::cerr << "error : " << printPosSymbol(ctx->VAR(i+1)) << var2 << " was never declared" << std::endl;
      error = true;

    }

  
    //creation d'un index pour var 2 si pas encore
    table->giveIndex(var2);
  

    //creation d'un index pour var1 si pas encore
    table->giveIndex(var1);

    used[var1] = true;
    used[var2] = true;


    //fin par expr
    if (ctx->expr() != nullptr) {
      checkExpr(ctx->expr());
    }
    
  }


  return (error ? 1 : 0);
    
}


antlrcpp::Any VarVisitor::visitVarToConst(ifccParser::VarToConstContext *ctx) {

    
  std::string var1 (ctx->VAR()->getText());

  //Verif variables déclarés
  if (! table->contains(var1)) {
    std::cerr << "error : " << printPosSymbol(ctx->VAR()) << var1 << " was never declared" << std::endl;
    error = true;
    
  }

  //creation d'un index pour var1 si pas encore
  table->giveIndex(var1);

  used[var1] = true;

  
  return (error ? 1 :0);

}


antlrcpp::Any VarVisitor::visitVarToExpr(ifccParser::VarToExprContext *ctx) {

  
  std::string var1 (ctx->VAR()->getText());

  //Verif variables déclarés
  if (! table->contains(var1)) {
    std::cerr << "error : " << printPosSymbol(ctx->VAR()) << var1 << " was never declared" << std::endl;
    error = true;
    
    return 1;
  }

  //verif variables dans expr
  checkExpr(ctx->expr());

  
  //creation d'un index pour var1 si pas encore
  table->giveIndex(var1);

  used[var1] = true;

  return 0;
  
}


antlrcpp::Any VarVisitor::visitDeclaration_var(ifccParser::Declaration_varContext *ctx) {

  std::vector<antlr4::tree::TerminalNode *> nodesVar = ctx->VAR();

  for (antlr4::tree::TerminalNode * nodeVar : nodesVar) {
  
    std::string var (nodeVar->getText());

    //verif var pas existante
    if (table->contains(var)) {
      std::cerr << "erreur : " << printPosSymbol(nodeVar) << var << " already declared" << std::endl;
      error = true;
    
    }

    table->add(var);
    used[var] = false;

  }

  return (error ? 1 : 0);

}



antlrcpp::Any VarVisitor::visitReturnVar(ifccParser::ReturnVarContext *ctx) {

  std::string var (ctx->VAR()->getText());

  //verif var pas existante
  if (! table->contains(var)) {

    std::cerr << "error : " << printPosSymbol(ctx->VAR()) << var << " was never declared" << std::endl;
    error = true;
    
    return 1;

  }

  //creation d'un index pour var si besoin
  table->giveIndex(var);

  used[var] = true;
  

  return 0;
  
  
}

antlrcpp::Any VarVisitor::visitReturnExpr(ifccParser::ReturnExprContext *ctx) {

  //verif variables dans expr
  checkExpr(ctx->expr());

  return 0;

}

antlrcpp::Any VarVisitor::visitVarInitVar(ifccParser::VarInitVarContext *ctx) {

  for (int i = 0; i+1 < ctx->VAR().size(); i++) {

    
    std::string var1 (ctx->VAR(i)->getText());
    std::string var2 (ctx->VAR(i+1)->getText());

    //verif var1 pas existante
    if (table->contains(var1)) {

      std::cerr << "error : " << printPosSymbol(ctx->VAR(i)) << var1 << " already declared" << std::endl;
      error = true;

    }

    //verif var2 existe
    if (! table->contains(var2)) {
      std::cerr << "error : " << printPosSymbol(ctx->VAR(i+1)) << var2 << " was never declared" << std::endl;
      error = true;
    
    }

    //creation d'un index pour var2 si besoin
    table->giveIndex(var2);

    table->giveIndex(var1);

    used[var2] = true;
    used[var1] = false;

  }

  return (error ? 1 : 0);
  
}
    

antlrcpp::Any VarVisitor::visitVarInitConst(ifccParser::VarInitConstContext *ctx) {


  for (int i = 0; i < ctx->VAR().size(); i++) {    
  
    std::string var1 (ctx->VAR(i)->getText());

    //verif var1 pas existante
    if (table->contains(var1)) {

      std::cerr << "error : " << printPosSymbol(ctx->VAR(i)) << var1 << " already declared" << std::endl;
      error = true;

    }


    table->giveIndex(var1);
  
    used[var1] = false;

  }

  return (error ? 1 : 0);

}


antlrcpp::Any VarVisitor::visitVarInitExpr(ifccParser::VarInitExprContext *ctx) {


  for (int i = 0; i < ctx->VAR().size(); i++) {
    
    std::string var1 (ctx->VAR(i)->getText());

    //verif var1 pas existante
    if (table->contains(var1)) {

      std::cerr << "error : " << printPosSymbol(ctx->VAR(i)) << var1 << " already declared" << std::endl;
      error = true;
    
    }

    //verif variables dans expr
    checkExpr(ctx->expr(i));

    table->giveIndex(var1);
  
    used[var1] = false;

  }

  return (error ? 1 : 0);

  
}


antlrcpp::Any VarVisitor::visitVar(ifccParser::VarContext *ctx) {

  
  //verif var dans une expr existe
  if (! table->contains(ctx->VAR()->getText())) {
    std::cerr << "error : " << printPosSymbol(ctx->VAR()) << ctx->VAR()->getText() << " was never declared" << std::endl;
    error = true;
    
    return 1;
  }

  //creation d'un index si besoin
  table->giveIndex(ctx->VAR()->getText());

  used[ctx->VAR()->getText()] = true;

  return 0;

}
  


void VarVisitor::checkUnusedDecla() {

  for ( auto it = used.cbegin(); it != used.cend(); ++it ) {

    if (it->second == false) {
      std::cerr << "error : variable " << it->first << " was declared but not used" << std::endl;
      error = true;
    }
  }

}

