#pragma once


#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"

#include <vector>
#include <string>

#include "IR.h"


/**
 * @brief Structure rassemblant les informations contenus dans un prototype de fonction C
 **/
typedef struct {
  
  std::string name;
  Type typeReturn;
  std::vector<Type> params;

} Function;


/**
 * @brief Visiteur de fonction C
 * @details
 *  Parcourt l'AST de l'axiom de la grammaire.
 *  Enregistre une table des fonctions
 * 
 **/
class  FunctionVisitor : public ifccBaseVisitor {
public:


  FunctionVisitor(bool debug = false, bool symbol = false);  
  virtual ~FunctionVisitor();


  virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override;

  virtual antlrcpp::Any visitDeclaration_function(ifccParser::Declaration_functionContext *ctx) override;
  virtual antlrcpp::Any visitDefinition_function(ifccParser::Definition_functionContext *ctx) override;

  virtual antlrcpp::Any visitParametres(ifccParser::ParametresContext *ctx) override;
  virtual antlrcpp::Any visitParametre(ifccParser::ParametreContext *ctx) override;

  virtual antlrcpp::Any visitFunction(ifccParser::FunctionContext *ctx) override;
  virtual antlrcpp::Any visitStatement(ifccParser::StatementContext *ctx) override;
  virtual antlrcpp::Any visitCall_function(ifccParser::Call_functionContext *ctx) override;
  

  bool hasError() {
    return error;
  }

  /**
   * @brief appel gen_asm de chaque cfg présent
   **/
  void gen_asm(std::ostream &o);
  
private:

  std::vector<CFG*> tab; /**<un CFG par fonction**/
  bool debug, symbol, error;

  std::vector<Function*> functions; /**<tableau des fonctions du contenu dans le fichier source**/


  /**
   * @brief renvoie l'indice dans functions si la fonction ayant name comme nom existe
   * @param name nom potentiel d'une fonction
   **/
  int function_exist(const std::string &name);
  
};


inline int FunctionVisitor::function_exist(const std::string &name) {

  for (int i = 0; i < functions.size(); i++) {

    if (functions[i]->name == name) {
      return i;
    }
    
  }

  return -1;

}

