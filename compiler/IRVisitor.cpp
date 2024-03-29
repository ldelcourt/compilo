#include "IRVisitor.h"


antlrcpp::Any IRVisitor::visitProg(ifccParser::ProgContext *ctx) 
{

  this->visitChildren(ctx);
  cfg->current_bb = cfg->current_bb->exit_true;

  return 0;
}


antlrcpp::Any IRVisitor::visitReturnConst(ifccParser::ReturnConstContext *ctx)
{

  std::string params[2];
  params[0] = "#reg_ret";
  params[1] = cfg->createConstSymbol(stoi(ctx->CONST()->getText()));
  
  cfg->current_bb->addIRInstr(IRInstr::Operation::ldconst, Type::INT, params);


  params[0] = "output";
  cfg->current_bb->addIRInstr(IRInstr::Operation::jmp, Type::INT, params);


  return 0;
}


antlrcpp::Any IRVisitor::visitReturnVar(ifccParser::ReturnVarContext *ctx) {

  std::string params[2];
  params[0] = "#reg_ret";
  params[1] = ctx->VAR()->getText();
  
  cfg->current_bb->addIRInstr(IRInstr::Operation::copy, Type::INT, params);

  params[0] = "output";
  cfg->current_bb->addIRInstr(IRInstr::Operation::jmp, Type::INT, params);


  return 0;

}


antlrcpp::Any IRVisitor::visitReturnExpr(ifccParser::ReturnExprContext *ctx) {

  std::string params[2];
  params[0] = "#reg_ret";
  params[1] = (std::string)visit(ctx->expr());
  
  cfg->current_bb->addIRInstr(IRInstr::Operation::copy, Type::INT, params);

  params[0] = "output";
  cfg->current_bb->addIRInstr(IRInstr::Operation::jmp, Type::INT, params);


  
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

antlrcpp::Any IRVisitor::visitLessgreater(ifccParser::LessgreaterContext *ctx) {


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
  

  if (ctx->OP->getText() == "<") {

    if (!opConst) {
      cfg->current_bb->addIRInstr(IRInstr::Operation::cmp_lt, Type::INT, params);
    }
    else {
      params[0] = cfg->createConstSymbol(a<b);
    }
  }

  else if (ctx->OP->getText() == ">") {
    if (!opConst) {
      cfg->current_bb->addIRInstr(IRInstr::Operation::cmp_gt, Type::INT, params);
    }
    else {
      params[0] = cfg->createConstSymbol(a>b);
    }
  }

  else {
    std::cerr << "error : unknown operator " << ctx->OP->getText() << "\n";
    return 1;
  }

  return (std::string)params[0];

}

antlrcpp::Any IRVisitor::visitEqualnotequal(ifccParser::EqualnotequalContext *ctx) {


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
  

  if (ctx->OP->getText() == "==") {

    if (!opConst) {
      cfg->current_bb->addIRInstr(IRInstr::Operation::cmp_eq, Type::INT, params);
    }
    else {
      params[0] = cfg->createConstSymbol(a==b);
    }
  }

  else if (ctx->OP->getText() == "!=") {
    if (!opConst) {
      cfg->current_bb->addIRInstr(IRInstr::Operation::cmp_neq, Type::INT, params);
    }
    else {
      params[0] = cfg->createConstSymbol(a!=b);
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


antlrcpp::Any IRVisitor::visitIf_else_stmt(ifccParser::If_else_stmtContext *ctx) {

  BasicBlock* thenBB = cfg->addBasicBlock(cfg->newBBName());
  BasicBlock* elseBB = nullptr;

  if(ctx->else_stmt() != nullptr) {
    elseBB = cfg->addBasicBlock(cfg->newBBName());
    cfg->current_bb->exit_false = elseBB;
  } 

  
  BasicBlock* endBB = cfg->addBasicBlock(cfg->newBBName());

  endBB->exit_true = cfg->current_bb->exit_true;
  cfg->current_bb->exit_true = thenBB;
  thenBB->exit_true = endBB;

  if(ctx->else_stmt() != nullptr) {
    elseBB->exit_true = endBB;
  }else {
    cfg->current_bb->exit_false = endBB;
  }


  std::string param[1];
  param[0] = (std::string)visit(ctx->expr());
  cfg->current_bb->addIRInstr(IRInstr::Operation::ifelse, Type::INT, param);

  cfg->current_bb = thenBB;
  for(auto stmt : ctx->statement()) {
    visit(stmt);
  }
  if(elseBB != nullptr) {
    cfg->current_bb = elseBB;
    visit(ctx->else_stmt());
  }
  cfg->current_bb = endBB;

  return 0;
  
}

antlrcpp::Any IRVisitor::visitWhile_stmt(ifccParser::While_stmtContext *ctx) {

  BasicBlock* testBB = cfg->addBasicBlock(cfg->newBBName());
  BasicBlock* whileBB = cfg->addBasicBlock(cfg->newBBName());
  BasicBlock* endBB = cfg->addBasicBlock(cfg->newBBName());
  endBB->exit_true = cfg->current_bb->exit_true;
  cfg->current_bb->exit_true = testBB;
  testBB->exit_false = endBB;
  
  testBB->exit_true = whileBB;
  whileBB->exit_true = testBB;

  cfg->current_bb = testBB;
  std::string param[1];
  param[0] = (std::string)visit(ctx->expr());
  cfg->current_bb->addIRInstr(IRInstr::Operation::while_, Type::INT, param);

  cfg->current_bb = whileBB;
  for(auto stmt : ctx->statement()) {
    visit(stmt);
  }

  cfg->current_bb = endBB;

  return 0;
  
}