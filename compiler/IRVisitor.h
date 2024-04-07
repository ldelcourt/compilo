#pragma once


#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"

#include "IR.h"

#include <vector>
#include <string>


/**
 * @brief Visite l'AST d'une fonction et créer les instructions IR au fur et à mesure
 *
 *
 * @note Implémente la propagation de constantes version simple
 **/
class  IRVisitor : public ifccBaseVisitor {
public:


  IRVisitor(CFG *cfg) : ifccBaseVisitor(), cfg(cfg) {}

  //Fonction personnelle
  virtual antlrcpp::Any visitParametre(ifccParser::ParametreContext *ctx) override; /**< int a**/
  virtual antlrcpp::Any visitParametres(ifccParser::ParametresContext *ctx) override; /**< int a, int b, int c**/

  
  //Fonction main
  virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override ; /**< int main()**/

  //Block
  virtual antlrcpp::Any visitBlock(ifccParser::BlockContext *ctx) override; /**< {...}**/

  //Return 
  virtual antlrcpp::Any visitReturnConst(ifccParser::ReturnConstContext *ctx) override; /**< return 42;**/
  virtual antlrcpp::Any visitReturnVar(ifccParser::ReturnVarContext *ctx) override; /**< return a;**/ 
  virtual antlrcpp::Any visitReturnExpr(ifccParser::ReturnExprContext *ctx) override; /**< return a + 42;**/

  //Variables 
  virtual antlrcpp::Any visitVarToVar(ifccParser::VarToVarContext *ctx) override; /**< a = b; ou a = b = c = 42 + d;**/
  virtual antlrcpp::Any visitVarToConst(ifccParser::VarToConstContext *ctx) override; /**< a = 42;**/
  virtual antlrcpp::Any visitVarToExpr(ifccParser::VarToExprContext *ctx) override; /**< a = b + 42;**/
  
  virtual antlrcpp::Any visitVarInitVar(ifccParser::VarInitVarContext *ctx) override; /**< int a = b, c = d; **/
  virtual antlrcpp::Any visitVarInitConst(ifccParser::VarInitConstContext *ctx) override; /**< int a = 42, b = 43;**/
  virtual antlrcpp::Any visitVarInitExpr(ifccParser::VarInitExprContext *ctx) override; /**< int a = b + 42, c = d + 43;**/
  virtual antlrcpp::Any visitVarInitAffect(ifccParser::VarInitAffectContext *ctx) override; /**< int a = b = 42 + d;**/
  
  

  //Expressions
  virtual antlrcpp::Any visitPar(ifccParser::ParContext *ctx) override; /**< (expr)**/
  virtual antlrcpp::Any visitMultdivmod(ifccParser::MultdivmodContext *ctx) override; /**< * / % **/
  virtual antlrcpp::Any visitUnaire(ifccParser::UnaireContext *ctx) override; /**< -expr **/
  virtual antlrcpp::Any visitLogiNot(ifccParser::LogiNotContext *ctx) override; /**< !expr**/
  virtual antlrcpp::Any visitPlusmoins(ifccParser::PlusmoinsContext *ctx) override; /**< + -**/
  virtual antlrcpp::Any visitLessgreater(ifccParser::LessgreaterContext *ctx) override; /**< < >**/
  virtual antlrcpp::Any visitEqualnotequal(ifccParser::EqualnotequalContext *ctx) override; /**< == !=**/
  virtual antlrcpp::Any visitBinaryAND(ifccParser::BinaryANDContext *ctx) override; /**< &**/
  virtual antlrcpp::Any visitBinaryXOR(ifccParser::BinaryXORContext *ctx) override; /**< ^**/
  virtual antlrcpp::Any visitBinaryOR(ifccParser::BinaryORContext *ctx) override; /**< |**/
  virtual antlrcpp::Any visitConst(ifccParser::ConstContext *ctx) override; /**< 42**/
  virtual antlrcpp::Any visitVar(ifccParser::VarContext *ctx) override; /**< a**/
  virtual antlrcpp::Any visitFunction(ifccParser::FunctionContext *ctx) override; /**< function()**/

  //Appel à des fonctions
  virtual antlrcpp::Any visitCall_function(ifccParser::Call_functionContext *ctx) override; /**< function()**/

  //If Statement
  virtual antlrcpp::Any visitIf_else_stmt(ifccParser::If_else_stmtContext *ctx) override; /**< if (..) {} else {}**/

  //While Statement
  virtual antlrcpp::Any visitWhile_stmt(ifccParser::While_stmtContext *ctx) override; /**< while(..) {}**/

private:

  CFG *cfg; /**<le CFG en cours de construction**/
  std::string currentBlock = ""; /**<le nom du block en cours**/
  int numBlock = 0; /**<le numero du block en cours**/
  
};

