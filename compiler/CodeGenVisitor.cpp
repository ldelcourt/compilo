#include "CodeGenVisitor.h"

antlrcpp::Any CodeGenVisitor::visitProg(ifccParser::ProgContext *ctx) {
  std::cout << ".globl main\n";
  std::cout << " main: \n";
  std::cout << " \tpushq %rbp\n"
            << " \tmovq %rsp, %rbp\n"; // prologue

  for (ifccParser::Var_stmtContext *var_stmtContext : ctx->var_stmt()) {
    this->visit(var_stmtContext);
  }
  this->visit(ctx->return_stmt());

  std::cout << " \tpopq %rbp\n"
            << " \tret\n"; // epilogue

  return 0;
}

antlrcpp::Any
CodeGenVisitor::visitReturnConst(ifccParser::ReturnConstContext *ctx) {
  int retval = stoi(ctx->CONST()->getText());

  std::cout << " \tmovl $" << retval << ", %eax\n";

  return 0;
}

antlrcpp::Any
CodeGenVisitor::visitReturnVar(ifccParser::ReturnVarContext *ctx) {

  std::cout << " \tmovl " << table->getIndex(ctx->VAR()->getText())
            << "(%rbp), %eax\n";

  return 0;
}

antlrcpp::Any
CodeGenVisitor::visitReturnExpr(ifccParser::ReturnExprContext *ctx) {

  int index = (int)visit(ctx->expr());

  if (index >= 0) {
    return 1;
  }

  std::cout << " \tmovl " << index << "(%rbp), %eax\n";

  return 0;
}

antlrcpp::Any CodeGenVisitor::visitVarToVar(ifccParser::VarToVarContext *ctx) {

  int var0 = table->getIndex(ctx->VAR(0)->getText());
  int var1 = table->getIndex(ctx->VAR(1)->getText());

  std::cout << " \tmovl " << var1 << "(%rbp), %eax\n"
            << " \tmovl %eax, " << var0 << "(%rbp)\n";

  return 0;
}

antlrcpp::Any
CodeGenVisitor::visitVarToConst(ifccParser::VarToConstContext *ctx) {

  int var = table->getIndex(ctx->VAR()->getText());

  std::cout << " \tmovl $" << ctx->CONST()->getText() << ", " << var
            << "(%rbp)\n";

  return 0;
}

antlrcpp::Any
CodeGenVisitor::visitVarToExpr(ifccParser::VarToExprContext *ctx) {

  int indexExpr = (int)visit(ctx->expr());
  int var0 = table->getIndex(ctx->VAR()->getText());

  if (indexExpr >= 0) {
    return 1;
  }

  std::cout << " \tmovl " << indexExpr << "(%rbp), %eax\n"
            << " \tmovl %eax, " << var0 << "(%rbp)\n";

  return 0;
}

antlrcpp::Any
CodeGenVisitor::visitVarInitVar(ifccParser::VarInitVarContext *ctx) {

  int var0 = table->getIndex(ctx->VAR(0)->getText());
  int var1 = table->getIndex(ctx->VAR(1)->getText());

  std::cout << " \tmovl " << var1 << "(%rbp), %eax\n"
            << " \tmovl %eax, " << var0 << "(%rbp)\n";

  return 0;
}

antlrcpp::Any
CodeGenVisitor::visitVarInitConst(ifccParser::VarInitConstContext *ctx) {

  int var = table->getIndex(ctx->VAR()->getText());

  std::cout << " \tmovl $" << ctx->CONST()->getText() << ", " << var
            << "(%rbp)\n";

  return 0;
}

antlrcpp::Any
CodeGenVisitor::visitVarInitExpr(ifccParser::VarInitExprContext *ctx) {

  int var0 = table->getIndex(ctx->VAR()->getText());
  int expr = (int)visit(ctx->expr());

  if (expr >= 0) {
    return 1;
  }

  std::cout << " \tmovl " << expr << "(%rbp), %eax\n"
            << " \tmovl %eax, " << var0 << "(%rbp)\n";

  return 0;
}

antlrcpp::Any CodeGenVisitor::visitPar(ifccParser::ParContext *ctx) {

  return visit(ctx->expr());
}

antlrcpp::Any
CodeGenVisitor::visitMultdivmod(ifccParser::MultdivmodContext *ctx) {

  int indexSymbol1 = (int)visit(ctx->expr(0));
  int indexSymbol2 = (int)visit(ctx->expr(1));

  if (indexSymbol1 >= 0 || indexSymbol2 >= 0) {
    return 1;
  }

  int indexRes = table->createTemp();

  if (ctx->OP->getText() == "*") {

    std::cout << " \tmovl " << indexSymbol1 << "(%rbp), %eax\n";
    std::cout << " \timull " << indexSymbol2 << "(%rbp), %eax\n";
    std::cout << " \tmovl %eax, " << indexRes << "(%rbp)\n";
  }

  else if (ctx->OP->getText() == "/") {
    std::cout << " \tmovl " << indexSymbol1 << "(%rbp), %eax\n";
    std::cout << " \tcltd\n";
    std::cout << " \tidivl " << indexSymbol2 << "(%rbp)\n";

    std::cout << " \tmovl %eax, " << indexRes << "(%rbp)\n";
  }

  else if (ctx->OP->getText() == "%") {
    std::cout << " \tmovl " << indexSymbol1 << "(%rbp), %eax\n";
    std::cout << " \tcltd\n";
    std::cout << " \tidivl " << indexSymbol2 << "(%rbp)\n";

    std::cout << " \tmovl %edx, " << indexRes << "(%rbp)\n";
  }

  else {
    std::cerr << "error : unknown operator " << ctx->OP->getText() << "\n";
    return 1;
  }

  return indexRes;
}

antlrcpp::Any CodeGenVisitor::visitUnaire(ifccParser::UnaireContext *ctx) {

  int index = (int)visit(ctx->expr());
  int indexRes = table->createTemp();

  if (index >= 0) {
    return 1;
  }

  std::cout << " \tmovl " << index << "(%rbp), %eax\n";
  std::cout << " \tnegl %eax\n";
  std::cout << " \tmovl %eax, " << indexRes << "(%rbp)\n";

  return indexRes;
}

antlrcpp::Any
CodeGenVisitor::visitPlusmoins(ifccParser::PlusmoinsContext *ctx) {

  int indexSymbol1 = (int)visit(ctx->expr(0));
  int indexSymbol2 = (int)visit(ctx->expr(1));

  if (indexSymbol1 >= 0 || indexSymbol2 >= 0) {
    return 1;
  }

  int indexRes = table->createTemp();

  if (ctx->OP->getText() == "+") {

    std::cout << " \tmovl " << indexSymbol1 << "(%rbp), %edx\n";
    std::cout << " \tmovl " << indexSymbol2 << "(%rbp), %eax\n";
    std::cout << " \taddl %edx, %eax\n";
    std::cout << " \tmovl %eax, " << indexRes << "(%rbp)\n";
  }

  else if (ctx->OP->getText() == "-") {
    std::cout << " \tmovl " << indexSymbol1 << "(%rbp), %eax\n";
    std::cout << " \tsubl " << indexSymbol2 << "(%rbp), %eax\n";
    std::cout << " \tmovl %eax, " << indexRes << "(%rbp)\n";
  }

  else {
    std::cerr << "error : unknown operator " << ctx->OP->getText() << "\n";
    return 1;
  }

  return indexRes;
}

antlrcpp::Any CodeGenVisitor::visitConst(ifccParser::ConstContext *ctx) {

  int index = table->createTemp();

  std::cout << " \tmovl $" << ctx->CONST()->getText() << ", " << index
            << "(%rbp)\n";

  return index;
}

antlrcpp::Any CodeGenVisitor::visitVar(ifccParser::VarContext *ctx) {

  return table->getIndex(ctx->VAR()->getText());
}
