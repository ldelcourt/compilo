#ifndef IR_H
#define IR_H

#include <iostream>
#include <string>
#include <vector>

#include "SymbolTable.h"
#include "antlr4-runtime.h"

#include "IRInstr.h"

// AST
typedef antlr4::tree::ParseTree DefFunction;

class CFG;

/* A few important comments.
   IRInstr has no jump instructions.
   cmp_* instructions behaves as an arithmetic two-operand instruction (add or
   mult), returning a boolean value (as an int)

   Assembly jumps are generated as follows:
   BasicBlock::gen_asm() first calls IRInstr::gen_asm() on all its instructions,
   and then if  exit_true  is a  nullptr, the epilogue is generated else if
   exit_false is a nullptr, an unconditional jmp to the exit_true branch is
   generated else (we have two successors, hence a branch) an instruction
   comparing the value of test_var_name to true is generated, followed by a
   conditional branch to the exit_false branch, followed by an unconditional
   branch to the exit_true branch The attribute test_var_name itself is defined
   when converting the if, while, etc of the AST  to IR.

   Possible optimization:
   a cmp_* comparison instructions, if it is the last instruction of its block,
   generates an actual assembly comparison
   followed by a conditional jump to the exit_false branch
*/

/**
 * @brief  The class for a basic block
 **/
class BasicBlock {
public:
  /**
   * @brief Constructeur
   * @param cfg parent du block
   * @param entry_label nom du block
   **/
  BasicBlock(CFG *cfg, std::string entry_label);

  ~BasicBlock(); /**<Destructeur, supprime les instructions du block**/

  /**
   * @brief Genere le code asm du block
   *
   * @param o stream d'écriture du code
   **/
  void gen_asm(std::ostream &o) const;

  void gen_x86(std::ostream &o) const; /**< x86 assembly code generation for
                                          this basic block (very simple) */
  void gen_arm(std::ostream &o) const; /**< ARM assembly code generation for
                                          this basic block (very simple) */

  /**
   * @brief Ajoute un instruction au block
   *
   * @param[in] op type d'instruction
   * @param[in] type type des paramètres
   * @param[in] nb nombre de paramètres
   * @param params paramètres de l'instruction
   *
   * @post vector instrs contient une nouvelle instruction (size +1)
   **/
  void addIRInstr(IRInstr::Operation op, Type t, const std::string *params,
                  int nb = 0);

  BasicBlock *exit_true =
      nullptr; /**< pointer to the next basic block, true branch. If nullptr,
                  return from procedure */
  BasicBlock *exit_false =
      nullptr; /**< pointer to the next basic block, false branch. If null_ptr,
                  the basic block ends with an unconditional jump */

  CFG *getCFG() const /**<Getter de l'attribut cfg**/
  {
    return cfg;
  }

  const std::string &getLabel() const /**<Getter de l'attribut label**/
  {
    return label;
  }

private:
  std::vector<IRInstr *> instrs; /** < the instructions themselves. */

  std::string label; /**< label of the BB, also will be the label in the
                        generated code */

  CFG *cfg; /** < the CFG where this block belongs */
};

/* A few important comments:
   The entry block is the one with the same label as the AST function name.
   (it could be the first of bbs, or it could be defined by an attribute value)
   The exit block is the one with both exit pointers equal to nullptr.
   (again it could be identified in a more explicit way)

*/

enum AssemblyLangage { x86, ARM };

/**
 * @brief The class for the control flow graph
 **/
class CFG {
public:
  /**
   * @brief Constructeur
   * @details
   *  Analyse des symboles avec VarVisitor de l'AST
   *  Construction de l'IR avec IRVisitor
   *
   * @param[in] ast l'AST d'une fonction
   * @param[in] nameFunction le nom de la fonction
   * @param[in] assemblyLangage asm dans lequel sera généré le code
   * @param[in] debug true si on souhaite afficher les instructions IR que
   *contient ce CFG
   * @param[in] symbol true si on souhaite afficher la table des symboles
   *
   * @throw int en cas d'erreur avec VarVisitor => IR pas générable
   *
   * @pre un AST correct syntaxiquement
   * @post un Control Flow Graph contenant les Basicblock contenant eux mêmes
   *les instructions IR représentant l'AST
   *
   **/
  CFG(DefFunction *ast, const std::string &nameFunction,
      AssemblyLangage assemblyLangage = x86, bool debug = false,
      bool symbol = false);
  ~CFG(); /**<Destructeur, detruit les blocks**/

  /**
   * @brief Créer et ajoute un BasicBlock au CFG
   *
   * @param[in] name le nom du block
   *
   * @return BasicBlock*
   **/
  BasicBlock *addBasicBlock(const std::string &name);

  /**
   * @brief Genere le code asm x86 du CFG
   * @param o stream d'écriture du code
   **/
  void gen_asm(std::ostream &o) const;

  void gen_asm_prologue(std::ostream &o)
      const; /**<Genere le code asm du prologue d'une fonction**/
  void gen_asm_epilogue(std::ostream &o)
      const; /**<Genere le code asm de l'epilogue d'une fonction**/
  void gen_x86_prologue(std::ostream &o) const;
  void gen_x86_epilogue(std::ostream &o) const;
  void gen_arm_prologue(std::ostream &o) const;
  void gen_arm_epilogue(std::ostream &o) const;

  /**
   * @brief Convertit un symbol de la table en asm x86
   *
   * @param[in] reg le symbole de la table à convertir
   * @return std::string
   **/
  std::string symbol_to_asm(const std::string &reg) const;

  // symbol table management
  std::string createTempVar(); /**<Créer une variable temporaire dans la table
                                  des symboles et renvoie son nom**/
  std::string
  createConstSymbol(int v); /**<Créer une constante de valeur v dans la table
                               des symboles et renvoie son nom**/

  /**
   * @brief Teste si un symbole est une constante
   * @details
   *  un symbole est une constante s'il commence par #const
   *
   * @param[in] symbol le symbole à vérifier
   * @param[out] value la valeur de la constante si s'en est une
   *
   * @return true si le symbole est une constante
   **/
  bool symbolIsConst(const std::string &symbol, int *value = nullptr) const;

  /**
   * @brief Extrait le vrai nom d'un symbole (=sans les numéros de blocks)
   * @details
   *  Pour gérer les portés de variables, un symbole est enregistré dans la
   *table en ajoutant ':' suivi du numéro de block Ici, on enlève cet ajout
   *
   * @param[in] symbol
   *
   * @throw int en cas d'erreur
   *
   * @return le vrai nom d'un symbole
   **/
  std::string getRealVarname(const std::string &symbol);

  // basic block management
  std::string newBBName() /**<Retourne un nom possible pour un nouveau block**/
  {
    return nameFunction + "_bb" + std::to_string(nextBBnumber++);
  }

  BasicBlock *current_bb; /**<pointeur sur le block actuellement en cours de
                             construction**/

  // GETTERS
  const std::string &getNameFunction() const { return nameFunction; }
  bool debug() const { return _debug; }
  bool symbol() const { return _symbol; }

  AssemblyLangage getAssemblyLangage() const { return assemblyLangage; }
  SymbolTable getSymbolTable() const { return table; }

private:
  int nextBBnumber; /**< just for naming basicblock */

  std::vector<BasicBlock *> bbs; /**< all the basic blocks of this CFG*/

  DefFunction *ast; /**< AST correct**/

  SymbolTable table; /**<Table des symboles**/

  std::string nameFunction; /**<Nom de la fonction représenté par ce CFG**/

  bool _debug;  /**<boolean définissant l'affichage des instructions IR**/
  bool _symbol; /**<boolean définissant l'affichage de la table des symboles**/

  AssemblyLangage assemblyLangage; /**<language d'asm à utiliser**/
};

#endif
