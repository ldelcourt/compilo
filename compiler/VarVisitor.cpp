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
    std::pair<bool, std::string> res1 = checkVarInAllBlocks(var1 + currentBlock);
    std::pair<bool, std::string> res2 = checkVarInAllBlocks(var2 + currentBlock);

    //Verif variables déclarés
    if (! res1.first) {
      std::cerr << "error : " << printPosSymbol(ctx->VAR(i)) <<  var1 + currentBlock << " was never declared" << std::endl;
      error = true;
      
    }

    if (! res2.first) {
      std::cerr << "error : " << printPosSymbol(ctx->VAR(i+1)) << var2 + currentBlock << " was never declared" << std::endl;
      error = true;

    }

  
    //creation d'un index pour var1 si pas encore
    table->giveIndex(res1.second);
  

    //creation d'un index pour var 2 si pas encore
    table->giveIndex(res2.second);

    used[res1.second] = true;
    used[res2.second] = true;


    //fin par expr
    if (ctx->expr() != nullptr) {
      checkExpr(ctx->expr());
    }
    
  }


  return (error ? 1 : 0);
    
}


antlrcpp::Any VarVisitor::visitVarToConst(ifccParser::VarToConstContext *ctx) {

    
  std::string var1 (ctx->VAR()->getText());
  std::pair<bool, std::string> res1 = checkVarInAllBlocks(var1 + currentBlock);

  //Verif variables déclarés
  if (! res1.first) {
    std::cerr << "error in visitVarToConst : " << printPosSymbol(ctx->VAR()) << var1 + currentBlock << " was never declared" << std::endl;
    error = true;
    
  }

  //creation d'un index pour var1 si pas encore
  table->giveIndex(res1.second);

  used[res1.second] = true;

  
  return (error ? 1 :0);

}


antlrcpp::Any VarVisitor::visitVarToExpr(ifccParser::VarToExprContext *ctx) {

  
  std::string var1 (ctx->VAR()->getText());
  std::pair<bool, std::string> res1 = checkVarInAllBlocks(var1 + currentBlock);

  //Verif variables déclarés
  if (! res1.first) {
    std::cerr << "error : " << printPosSymbol(ctx->VAR()) << var1 + currentBlock << " was never declared" << std::endl;
    error = true;
    
    return 1;
  }

  //verif variables dans expr
  checkExpr(ctx->expr());

  
  //creation d'un index pour var1 si pas encore
  table->giveIndex(res1.second);

  used[res1.second] = true;

  return 0;
  
}


antlrcpp::Any VarVisitor::visitDeclaration_var(ifccParser::Declaration_varContext *ctx) {

  std::vector<antlr4::tree::TerminalNode *> nodesVar = ctx->VAR();

  for (antlr4::tree::TerminalNode * nodeVar : nodesVar) {
  
    std::string var (nodeVar->getText() + currentBlock);

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

  std::pair<bool, std::string> res = checkVarInAllBlocks(var + currentBlock);

  //verif var pas existante
  if (! res.first) {

    std::cerr << "error : " << printPosSymbol(ctx->VAR()) << var << " was never declared" << std::endl;
    error = true;
    
    return 1;

  }

  //creation d'un index pour var si besoin
  table->giveIndex(res.second);

  used[res.second] = true;
  

  return 0;
  
  
}

antlrcpp::Any VarVisitor::visitReturnExpr(ifccParser::ReturnExprContext *ctx) {

  //verif variables dans expr
  checkExpr(ctx->expr());

  return 0;

}

antlrcpp::Any VarVisitor::visitVarInitVar(ifccParser::VarInitVarContext *ctx) {

  for (int i = 0; i+1 < ctx->VAR().size(); i++) {

    
    std::string var1 (ctx->VAR(i)->getText() + currentBlock);
    std::string var2 (ctx->VAR(i+1)->getText() + currentBlock);
    std::pair<bool, std::string> res1 = checkVarInAllBlocks(var1);
    std::pair<bool, std::string> res2 = checkVarInAllBlocks(var2);

    //verif var1 pas existante
    if (res1.first && i == 0) {

      std::cerr << "error : " << printPosSymbol(ctx->VAR(i)) << res1.second << " already declared" << std::endl;
      error = true;

    }
    else if (! res1.first && i != 0) {
      std::cerr << "error : " << printPosSymbol(ctx->VAR(i)) << res1.second << " was never declared" << std::endl;
      error = true;
    }

    //verif var2 existe
    if (! res2.first) {
      std::cerr << "error : " << printPosSymbol(ctx->VAR(i+1)) << res2.second << " was never declared" << std::endl;
      error = true;
    
    }

    //creation d'un index pour var2 si besoin
    table->giveIndex(res2.second);
    table->giveIndex(res1.second);

    used[res2.second] = true;
    used[res1.second] = false;

  }

  return (error ? 1 : 0);
  
}
    

antlrcpp::Any VarVisitor::visitVarInitConst(ifccParser::VarInitConstContext *ctx) {


  for (int i = 0; i < ctx->VAR().size(); i++) {    
  
    std::string var1 (ctx->VAR(i)->getText() + currentBlock);
    std::pair<bool, std::string> res = checkVarInAllBlocks(var1);


    //verif var1 pas existante
    if (res.first) {

      std::cerr << "error : " << printPosSymbol(ctx->VAR(i)) << res.second << " already declared" << std::endl;
      error = true;

    }

    table->giveIndex(res.second);
    used[res.second] = false;

  }

  return (error ? 1 : 0);

}


antlrcpp::Any VarVisitor::visitVarInitExpr(ifccParser::VarInitExprContext *ctx) {


  for (int i = 0; i < ctx->VAR().size(); i++) {
    
    std::string var1 (ctx->VAR(i)->getText() + currentBlock);

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

  std::pair<bool, std::string> res = checkVarInAllBlocks(ctx->VAR()->getText() + currentBlock);

  //verif var dans une expr existe
  if (! res.first) {
    std::cerr << "error : " << printPosSymbol(ctx->VAR()) << ctx->VAR()->getText() + currentBlock << " was never declared" << std::endl;
    error = true;
    
    return 1;
  }

  //creation d'un index si besoin
  table->giveIndex(res.second);

  used[res.second] = true;

  return 0;

}

antlrcpp::Any VarVisitor::visitBlock(ifccParser::BlockContext *ctx) {

  blockCount++;
  currentBlock = currentBlock + ":" + std::to_string(blockCount);

  for (auto it = ctx->children.begin(); it != ctx->children.end(); ++it) {
    checkExpr(*it);
  }

  size_t pos = currentBlock.find_last_of(":");
  currentBlock = currentBlock.substr(0, pos);

  return 0;
  
}
  

antlrcpp::Any VarVisitor::visitCall_function(ifccParser::Call_functionContext *ctx) {

  for (int i = 0 ; i < ctx->expr().size(); i++) {

    checkExpr(ctx->expr(i));

  }

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

std::pair<bool, std::string> VarVisitor::checkVarInAllBlocks(std::string name) {
  std::string varname = name;
  size_t pos = 0;
  while(pos != std::string::npos) {
    if (table->contains(varname)) {
      return std::make_pair(true, varname);
    }
    pos = varname.find_last_of(":");
    varname = varname.substr(0, pos);
  }

  return std::make_pair(false, name);
}

