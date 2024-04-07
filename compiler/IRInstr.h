#ifndef IRINSTR_H
#define IRINSTR_H

#include <initializer_list>
#include <iostream>
#include <string>
#include <vector>

#include "SymbolTable.h"

class BasicBlock;




/**
 * @brief Classe mère des instructions IR
 **/
class IRInstr {

public:
  typedef enum {
    ldconst,
    copy,
    add,
    sub,
    mul,
    div,
    mod,
    neg,
    logiNot,
    call,
    cmp_eq,
    cmp_neq,
    cmp_lt,
    cmp_gt,
    binary_and,
    binary_xor,
    binary_or,
    ifelse,
    while_,
    jmp,
    getparams,
    ret,
    getret
  } Operation; /**<Les différentes instructions connus**/


  IRInstr(BasicBlock* bb_, Operation op, Type t); /**<Constructeur**/
  virtual ~IRInstr() {} /**<Destructeur**/

  
  void gen_asm(std::ostream &o) const; /**<Gets to the right assembly langage */
  virtual void gen_x86_asm(std::ostream &o) const = 0; /**< x86 assembly code generation for this IR instruction */
  virtual void gen_arm(std::ostream &o) const = 0; /**< ARM assembly code generation for this IR instruction */

  
  static IRInstr* createInstr(BasicBlock *bb, Operation op, Type t, const std::string *params, int nb); /**<Create instance of good IRInstr subclass according to op **/

protected:
  BasicBlock *bb; /**< The BB this instruction belongs to, which provides a
                     pointer to the CFG this instruction belong to */
  Operation op;
  Type t; /**<Type de données que manipule l'instruction**/

  
  
};



class AddInstr : public IRInstr /**<Instruction d'addition (dest = x+y)**/
{

public:
  AddInstr(BasicBlock *bb, Type t, const std::string &dest,
           const std::string &x, const std::string &y);

  virtual void gen_x86_asm(std::ostream &o) const;
  virtual void gen_arm(std::ostream &o) const;

private:
  std::string dest;
  std::string x;
  std::string y;
};

class SubInstr : public IRInstr /**<Instruction de soustraction (dest = x-y)**/
{

public:
  SubInstr(BasicBlock *bb, Type t, const std::string &dest,
           const std::string &x, const std::string &y);

  virtual void gen_x86_asm(std::ostream &o) const;
  virtual void gen_arm(std::ostream &o) const;

private:
  std::string dest, x, y;
};


class MulInstr : public IRInstr /**<Instruction de multiplication (dest = x*y)**/
{

public:
  MulInstr(BasicBlock *bb, Type t, const std::string &dest,
           const std::string &x, const std::string &y);

  virtual void gen_x86_asm(std::ostream &o) const;
  virtual void gen_arm(std::ostream &o) const;

private:
  std::string dest, x, y;
};

class DivInstr : public IRInstr /**<Instruction de division (dest = x/y)**/
{

public:
  DivInstr(BasicBlock *bb, Type t, const std::string &dest,
           const std::string &x, const std::string &y);

  virtual void gen_x86_asm(std::ostream &o) const;
  virtual void gen_arm(std::ostream &o) const;

private:
  std::string dest;
  std::string x;
  std::string y;
};

class ModInstr : public IRInstr /**<Instruction de modulo (dest = x%y)**/
{

public:
  ModInstr(BasicBlock *bb, Type t, const std::string &dest,
           const std::string &x, const std::string &y);

  virtual void gen_x86_asm(std::ostream &o) const;
  virtual void gen_arm(std::ostream &o) const;

private:
  std::string dest, x, y;
};




class LdconstInstr : public IRInstr /**<Instruction de copie de constante (dest = val_constante)**/
{

public:
  LdconstInstr(BasicBlock *bb, Type t, const std::string &dest,
               const std::string &val);

  virtual void gen_x86_asm(std::ostream &o) const;
  virtual void gen_arm(std::ostream &o) const;

  std::string &getVal() { return val; }

private:
  std::string dest, val;
};


class CopyInstr : public IRInstr /**<Instruction de copie de variables (dest = variable)**/
{

public:
  CopyInstr(BasicBlock *bb, Type t, const std::string &dest,
            const std::string &var);

  virtual void gen_x86_asm(std::ostream &o) const;
  virtual void gen_arm(std::ostream &o) const;

private:
  std::string dest, var;
};


class NegInstr : public IRInstr /**<Instruction de negation (dest = -var)**/
{

public:
  NegInstr(BasicBlock *bb, Type t, const std::string &dest,
           const std::string &var);

  virtual void gen_x86_asm(std::ostream &o) const;
  virtual void gen_arm(std::ostream &o) const;

private:
  std::string dest, var;
};

class LogiNotInstr : public IRInstr /**<Instruction du non logique (dest = !var)**/
{

public:
  LogiNotInstr(BasicBlock *bb, Type t, const std::string &dest,
               const std::string &var);

  virtual void gen_x86_asm(std::ostream &o) const;
  virtual void gen_arm(std::ostream &o) const;

private:
  std::string dest, var;
};

class CallInstr : public IRInstr /**<Instruction d'appel de fonction (fonction(a, b, ...))**/
{

public:
  CallInstr(BasicBlock *bb, Type t, const std::string *params, int nb);
  virtual ~CallInstr() { delete[] params; }

  virtual void gen_x86_asm(std::ostream &o) const;
  virtual void gen_arm(std::ostream &o) const;

private:
  std::string *params;
  int nbParams;

  static const std::string regParams[6];
};

class CmpEqInstr : public IRInstr /**<Instruction de test d'egalité (dest = (x==y))**/
{

public:
  CmpEqInstr(BasicBlock *bb, Type t, const std::string &dest,
             const std::string &x, const std::string &y);

  virtual void gen_x86_asm(std::ostream &o) const;
  virtual void gen_arm(std::ostream &o) const;

private:
  std::string dest, x, y;
};

class CmpNeqInstr : public IRInstr /**<Instruction de test pas d'egalité (dest = (x != y))**/
{

public:
  CmpNeqInstr(BasicBlock *bb, Type t, const std::string &dest,
              const std::string &x, const std::string &y);

  virtual void gen_x86_asm(std::ostream &o) const;
  virtual void gen_arm(std::ostream &o) const;

private:
  std::string dest, x, y;
};

class CmpLtInstr : public IRInstr /**<Instruction de comparaison plus petit que (dest = (x < y))**/
{

public:
  CmpLtInstr(BasicBlock *bb, Type t, const std::string &dest,
             const std::string &x, const std::string &y);

  virtual void gen_x86_asm(std::ostream &o) const;
  virtual void gen_arm(std::ostream &o) const;

private:
  std::string dest, x, y;
};

class CmpGtInstr : public IRInstr /**<Instruction de comparaison plus grand que (dest = (x > y))**/
{

public:
  CmpGtInstr(BasicBlock *bb, Type t, const std::string &dest,
             const std::string &x, const std::string &y);

  virtual void gen_x86_asm(std::ostream &o) const;
  virtual void gen_arm(std::ostream &o) const;

private:
  std::string dest, x, y;
};

class BinaryAndInstr : public IRInstr /**<Instruction du ET binaire (dest = (x & y))**/
{

public:
  BinaryAndInstr(BasicBlock *bb, Type t, const std::string &dest,
                 const std::string &x, const std::string &y);

  virtual void gen_x86_asm(std::ostream &o) const;
  virtual void gen_arm(std::ostream &o) const;

private:
  std::string dest, x, y;
};

class BinaryXorInstr : public IRInstr /**<Instruction du XOR binaire (dest = (x ^ y))**/
{

public:
  BinaryXorInstr(BasicBlock *bb, Type t, const std::string &dest,
                 const std::string &x, const std::string &y);

  virtual void gen_x86_asm(std::ostream &o) const;
  virtual void gen_arm(std::ostream &o) const;

private:
  std::string dest, x, y;
};

class BinaryOrInstr : public IRInstr /**<Instruction du OU binaire (dest = (x | y))**/
{

public:
  BinaryOrInstr(BasicBlock *bb, Type t, const std::string &dest,
                const std::string &x, const std::string &y);

  virtual void gen_x86_asm(std::ostream &o) const;
  virtual void gen_arm(std::ostream &o) const;

private:
  std::string dest, x, y;
};

class IfElseInstr : public IRInstr /**<Instruction du if else**/
{

public:
  IfElseInstr(BasicBlock *bb, Type t, const std::string &cond);

  virtual void gen_x86_asm(std::ostream &o) const;
  virtual void gen_arm(std::ostream &o) const;

private:
  std::string cond;
};

class WhileInstr : public IRInstr /**<Instruction du while**/
{

public:
  WhileInstr(BasicBlock *bb, Type t, const std::string &cond);

  virtual void gen_x86_asm(std::ostream &o) const;
  virtual void gen_arm(std::ostream &o) const;

private:
  std::string cond;
};

class JmpInstr : public IRInstr /**<Instruction du jump vers un autre block**/
{

public:
  JmpInstr(BasicBlock *bb, Type t, const std::string &dest);

  virtual void gen_x86_asm(std::ostream &o) const;
  virtual void gen_arm(std::ostream &o) const;

private:
  std::string dest;
};

// A partir d'ici voir pour le code ARM


class GetParamsInstr : public IRInstr /**<Instruction de recupération des paramètres passés à une fonction**/
{


public:

  GetParamsInstr (BasicBlock *bb, Type t, const std::string *params, int nb);
  virtual ~GetParamsInstr() {
    delete [] params;
  }


  virtual void gen_x86_asm(std::ostream &o) const;
  virtual void gen_arm(std::ostream &o) const {}

private:

  std::string *params;
  int nbParams;

  
  static const std::string regParams[6];

};


class RetInstr : public IRInstr /**<Instruction de passage d'une valeur en return d'une fonction**/
{

public :
  RetInstr (BasicBlock *bb, Type t, const std::string &var);

  virtual void gen_x86_asm(std::ostream &o) const;
  virtual void gen_arm(std::ostream &o) const;

private: 
  std::string var;
};

class GetRetInstr : public IRInstr /**<Instruction de recupération d'une valeur renvoyé par une fonction**/
{

public:

  GetRetInstr (BasicBlock *bb, Type t, const std::string &dest);

  virtual void gen_x86_asm(std::ostream &o) const;
  virtual void gen_arm(std::ostream &o) const {}

private:

  std::string dest;
    
};


#endif
