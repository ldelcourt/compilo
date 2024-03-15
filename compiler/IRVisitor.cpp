#include "IRVisitor.h"


antlrcpp::Any IRVisitor::visitProg(ifccParser::ProgContext *ctx) 
{
  
  for (ifccParser::Var_stmtContext* var_stmtContext : ctx->var_stmt()) {
    this->visit( var_stmtContext );
  }

  cfg->current_bb = cfg->current_bb->exit_true;
  this->visit( ctx->return_stmt() );

  return 0;
}


antlrcpp::Any IRVisitor::visitReturnConst(ifccParser::ReturnConstContext *ctx)
{

  std::string params[2];
  params[0] = "#reg_ret";
  params[1] = cfg->createConstSymbol(stoi(ctx->CONST()->getText()));
  
  cfg->current_bb->addIRInstr(IRInstr::Operation::ldconst, Type::INT, params);

  return 0;
}


antlrcpp::Any IRVisitor::visitReturnVar(ifccParser::ReturnVarContext *ctx) {

  std::string params[2];
  params[0] = "#reg_ret";
  params[1] = ctx->VAR()->getText();
  
  cfg->current_bb->addIRInstr(IRInstr::Operation::copy, Type::INT, params);

  return 0;

}


antlrcpp::Any IRVisitor::visitReturnExpr(ifccParser::ReturnExprContext *ctx) {

  std::string params[2];
  params[0] = "#reg_ret";
  params[1] = (std::string)visit(ctx->expr());
  
  cfg->current_bb->addIRInstr(IRInstr::Operation::copy, Type::INT, params);

  
  return 0;
  
}


antlrcpp::Any IRVisitor::visitVarToVar(ifccParser::VarToVarContext *ctx) {

  std::string params[2];
  int delta = 0;
  
  if (ctx->expr() != nullptr) {

    params[1]  = (std::string)visit(ctx->expr());
    params[0] = ctx->VAR(ctx->VAR().size() - 1)->getText();

    cfg->current_bb->addIRInstr(IRInstr::Operation::copy, Type::INT, params);

    delta++;

  }

  else if (ctx->CONST() != nullptr) {
    params[0] = ctx->VAR(ctx->VAR().size() - 1)->getText();
    params[1] = cfg->createConstSymbol(stoi(ctx->CONST()->getText()));
  

    
    cfg->current_bb->addIRInstr(IRInstr::Operation::ldconst, Type::INT, params);

    delta++;

  }


  for (int i = ctx->VAR().size()-1; i-1 >= 0; i--) {


    params[0] = ctx->VAR(i-1)->getText();
    params[1] = ctx->VAR(i)->getText();
  
  
    cfg->current_bb->addIRInstr(IRInstr::Operation::copy, Type::INT, params);

  }

  return 0;
  
}
  
antlrcpp::Any IRVisitor::visitVarToConst(ifccParser::VarToConstContext *ctx) {

  std::string params[2];
  params[0] = ctx->VAR()->getText();
  params[1] = cfg->createConstSymbol(stoi(ctx->CONST()->getText()));
  
  
  cfg->current_bb->addIRInstr(IRInstr::Operation::ldconst, Type::INT, params);

  return 0;

}


antlrcpp::Any IRVisitor::visitVarToExpr(ifccParser::VarToExprContext *ctx) {

  std::string params[2];
  params[0] = ctx->VAR()->getText();
  params[1] = (std::string)visit(ctx->expr());

  if (cfg->symbolIsConst(params[1])) {
    cfg->current_bb->addIRInstr(IRInstr::Operation::ldconst, Type::INT, params);
  }

  else {
    cfg->current_bb->addIRInstr(IRInstr::Operation::copy, Type::INT, params);
  }
  

  return 0;
  
}

antlrcpp::Any IRVisitor::visitVarInitVar(ifccParser::VarInitVarContext *ctx) {

  for (int i = 0; i+1 < ctx->VAR().size(); i++) {

    
    std::string params[2];
    params[0] = ctx->VAR(i)->getText();
    params[1] = ctx->VAR(i+1)->getText();
  
  
    cfg->current_bb->addIRInstr(IRInstr::Operation::copy, Type::INT, params);
  }

  return 0;

}


antlrcpp::Any IRVisitor::visitVarInitConst(ifccParser::VarInitConstContext *ctx) {

  for (int i = 0; i < ctx->VAR().size(); i++) {
    
    std::string params[2];
    params[0] = ctx->VAR(i)->getText();
    params[1] = cfg->createConstSymbol(stoi(ctx->CONST(i)->getText()));
  
  
    cfg->current_bb->addIRInstr(IRInstr::Operation::ldconst, Type::INT, params);

  }

  return 0;

}


antlrcpp::Any IRVisitor::visitVarInitExpr(ifccParser::VarInitExprContext *ctx) {

  for (int i = 0; i < ctx->VAR().size(); i++) {
    
    std::string params[2];
    params[0] = ctx->VAR(i)->getText();
    params[1] = (std::string)visit(ctx->expr(i));
  
  
    if (cfg->symbolIsConst(params[1])) {
      cfg->current_bb->addIRInstr(IRInstr::Operation::ldconst, Type::INT, params);
    }

    else {
      cfg->current_bb->addIRInstr(IRInstr::Operation::copy, Type::INT, params);
    }

  }

  return 0;

  
}

  


antlrcpp::Any IRVisitor::visitPar(ifccParser::ParContext *ctx) {

  return (std::string)visit(ctx->expr());

}



antlrcpp::Any IRVisitor::visitMultdivmod(ifccParser::MultdivmodContext *ctx) {


  std::string params[3];
  
  params[1] = (std::string)visit(ctx->expr(0));
  params[2] = (std::string)visit(ctx->expr(1));


  //Constantes ?
  int a, b;
  bool opConst = false;
  if (cfg->symbolIsConst(params[1], &a) && cfg->symbolIsConst(params[2], &b)) {
    opConst = true;
  }

  //element neutre ?
  else if (cfg->symbolIsConst(params[1], &a) && a == 1 && ctx->OP->getText() == "*") {
    return (std::string)params[2];
  }
  else if (cfg->symbolIsConst(params[2], &b) && b == 1) {
    return (std::string)params[1];
  }

  //division par zero ?
  else if (cfg->symbolIsConst(params[2], &b) && b == 0 && (ctx->OP->getText() == "/" || ctx->OP->getText() == "%")) {
    std::cerr << "error: divide by zero is forbidden\n";
    return 1;
  }

  else {
    params[0] = cfg->createTempVar();
  }
  

  if (ctx->OP->getText() == "*") {

    if (!opConst) {
      cfg->current_bb->addIRInstr(IRInstr::Operation::mul, Type::INT, params);
    }
    else {
      params[0] = cfg->createConstSymbol(a*b);
    }
  }

  else if (ctx->OP->getText() == "/") {
    if (!opConst) {
      cfg->current_bb->addIRInstr(IRInstr::Operation::div, Type::INT, params);
    }
    else {
      params[0] = cfg->createConstSymbol(a/b);
    }
  }

  else if (ctx->OP->getText() == "%") {
    if (!opConst) {
      cfg->current_bb->addIRInstr(IRInstr::Operation::mod, Type::INT, params);
    }
    else {
      params[0] = cfg->createConstSymbol(a%b);
    }
  }

  else {
    std::cerr << "error : unknown operator " << ctx->OP->getText() << "\n";
    return 1;
  }

  return (std::string)params[0];

}


antlrcpp::Any IRVisitor::visitUnaire(ifccParser::UnaireContext *ctx) {

  std::string params[2];
  
  params[1] = (std::string)visit(ctx->expr());

  int a;
  if (cfg->symbolIsConst(params[1], &a)) {
    params[0] = cfg->createConstSymbol(-a);
  }

  else {
    params[0] = cfg->createTempVar();
    cfg->current_bb->addIRInstr(IRInstr::Operation::neg, Type::INT, params);
  }
  
  return (std::string)params[0];
  
}


antlrcpp::Any IRVisitor::visitPlusmoins(ifccParser::PlusmoinsContext *ctx) {

    std::string params[3];
  
  params[1] = (std::string)visit(ctx->expr(0));
  params[2] = (std::string)visit(ctx->expr(1));


  //Constantes ?
  int a, b;
  bool opConst = false;
  if (cfg->symbolIsConst(params[1], &a) && cfg->symbolIsConst(params[2], &b)) {
    opConst = true;
  }

  //element neutre ?
  else if (cfg->symbolIsConst(params[1], &a) && a == 0) {
    return (std::string)params[2];
  }
  else if (cfg->symbolIsConst(params[2], &b) && b == 0) {
    return (std::string)params[1];
  }

  else {
    params[0] = cfg->createTempVar();
  }
  

  if (ctx->OP->getText() == "+") {

    if (!opConst) {
      cfg->current_bb->addIRInstr(IRInstr::Operation::add, Type::INT, params);
    }
    else {
      params[0] = cfg->createConstSymbol(a+b);
    }
  }

  else if (ctx->OP->getText() == "-") {
    if (!opConst) {
      cfg->current_bb->addIRInstr(IRInstr::Operation::sub, Type::INT, params);
    }
    else {
      params[0] = cfg->createConstSymbol(a-b);
    }
  }

  else {
    std::cerr << "error : unknown operator " << ctx->OP->getText() << "\n";
    return 1;
  }

  return (std::string)params[0];

 
}

antlrcpp::Any IRVisitor::visitBinaryAND(ifccParser::BinaryANDContext *ctx) {

    std::string params[3];
  
  params[1] = (std::string)visit(ctx->expr(0));
  params[2] = (std::string)visit(ctx->expr(1));


  //Constantes ?
  int a, b;
  bool opConst = false;
  if (cfg->symbolIsConst(params[1], &a) && cfg->symbolIsConst(params[2], &b)) {
    opConst = true;
  }
  else {
    params[0] = cfg->createTempVar();
  }
  
  if (!opConst) {
    cfg->current_bb->addIRInstr(IRInstr::Operation::binary_and, Type::INT, params);
  }
  else {
    params[0] = cfg->createConstSymbol(a&b);
  }
  
  return (std::string)params[0];

 
}

antlrcpp::Any IRVisitor::visitBinaryXOR(ifccParser::BinaryXORContext *ctx) {

    std::string params[3];
  
  params[1] = (std::string)visit(ctx->expr(0));
  params[2] = (std::string)visit(ctx->expr(1));


  //Constantes ?
  int a, b;
  bool opConst = false;
  if (cfg->symbolIsConst(params[1], &a) && cfg->symbolIsConst(params[2], &b)) {
    opConst = true;
  }

  //element neutre ?
  else if (cfg->symbolIsConst(params[1], &a) && a == 0) {
    return (std::string)params[2];
  }
  else if (cfg->symbolIsConst(params[2], &b) && b == 0) {
    return (std::string)params[1];
  }

  else {
    params[0] = cfg->createTempVar();
  }
  
  if (!opConst) {
    cfg->current_bb->addIRInstr(IRInstr::Operation::binary_xor, Type::INT, params);
  }
  else {
    params[0] = cfg->createConstSymbol(a^b);
  }
  
  return (std::string)params[0];

 
}

antlrcpp::Any IRVisitor::visitBinaryOR(ifccParser::BinaryORContext *ctx) {

    std::string params[3];
  
  params[1] = (std::string)visit(ctx->expr(0));
  params[2] = (std::string)visit(ctx->expr(1));


  //Constantes ?
  int a, b;
  bool opConst = false;
  if (cfg->symbolIsConst(params[1], &a) && cfg->symbolIsConst(params[2], &b)) {
    opConst = true;
  }

  //element neutre ?
  else if (cfg->symbolIsConst(params[1], &a) && a == 0) {
    return (std::string)params[2];
  }
  else if (cfg->symbolIsConst(params[2], &b) && b == 0) {
    return (std::string)params[1];
  }

  else {
    params[0] = cfg->createTempVar();
  }
  
  if (!opConst) {
    cfg->current_bb->addIRInstr(IRInstr::Operation::binary_or, Type::INT, params);
  }
  else {
    params[0] = cfg->createConstSymbol(a|b);
  }
  
  return (std::string)params[0];

 
}


antlrcpp::Any IRVisitor::visitConst(ifccParser::ConstContext *ctx) {

  return (std::string)cfg->createConstSymbol(stoi(ctx->CONST()->getText()));
  
}


antlrcpp::Any IRVisitor::visitVar(ifccParser::VarContext *ctx) {

  return ctx->VAR()->getText();
  
}


