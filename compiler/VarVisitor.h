#pragma once


#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"


#include <unordered_map>
#include <string>

#include "SymbolTable.h"


/**
 * @brief Visite l'AST d'une fonction et complète la table des symboles
 **/
class  VarVisitor : public ifccBaseVisitor {
public:

  VarVisitor(SymbolTable *table) : ifccBaseVisitor(), table(table), error(false) {} /**<Constructeur**/
  ~VarVisitor() {} /**<Destructeur**/

  
  virtual antlrcpp::Any visitDefinition_function(ifccParser::Definition_functionContext *ctx) override; /**< int function() {}**/

  virtual antlrcpp::Any visitParametre(ifccParser::ParametreContext *ctx) override; /**< int a**/
  
  virtual antlrcpp::Any visitVarToVar(ifccParser::VarToVarContext *ctx) override; /**< a = b; ou a = b = c = 42 + d; **/
  virtual antlrcpp::Any visitVarToConst(ifccParser::VarToConstContext *ctx) override; /**< a = 42;**/
  virtual antlrcpp::Any visitVarToExpr(ifccParser::VarToExprContext *ctx) override; /**< a = b + 42;**/
  
  virtual antlrcpp::Any visitDeclaration_var(ifccParser::Declaration_varContext *ctx) override; /**< int a, b, c; **/

  virtual antlrcpp::Any visitReturnVar(ifccParser::ReturnVarContext *ctx) override; /**< return a;**/
  virtual antlrcpp::Any visitReturnExpr(ifccParser::ReturnExprContext *ctx) override; /**< return a + 42;**/
  
  virtual antlrcpp::Any visitVarInitVar(ifccParser::VarInitVarContext *ctx) override; /**< int a = b, c = d;**/
  virtual antlrcpp::Any visitVarInitConst(ifccParser::VarInitConstContext *ctx) override; /**< int a = 42, b = 43;**/
  virtual antlrcpp::Any visitVarInitExpr(ifccParser::VarInitExprContext *ctx) override; /**< int a = b + 42, c = d + 43;**/
  virtual antlrcpp::Any visitVarInitAffect(ifccParser::VarInitAffectContext *ctx) override; /**< int a = b = 42 + d;**/

  virtual antlrcpp::Any visitVar(ifccParser::VarContext *ctx) override; /**< a**/
  virtual antlrcpp::Any visitBlock(ifccParser::BlockContext *ctx) override; /**< { statements }**/

  virtual antlrcpp::Any visitCall_function(ifccParser::Call_functionContext *ctx) override; /**< function()**/

  
  bool hasError() const; /**<getter sur l'attribut error**/

  void checkUnusedDecla();/**<verifie les symboles dans la map used qui ne sont pas utilisés => set error en fonction**/

  
  std::pair<bool, std::string> checkVarInAllBlocks(std::string name); /**<Check for the variable name in all blocks (from the innermost to the outermost block)**/
  

private:

  SymbolTable *table; /**<Table des symboles à compléter**/
  std::unordered_map<std::string, bool> used; /**<Nom du symbole en clé et état d'utilisation en valeur**/
  bool error; /**<true si le parcours de l'AST entraine une erreur**/
  std::string currentBlock; /**<nom du block courrant**/
  int blockCount = 0; /**<numéro de block**/
  
};


inline bool VarVisitor::hasError() const {
  return error;
}
