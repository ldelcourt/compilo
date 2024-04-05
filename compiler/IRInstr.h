#ifndef IRINSTR_H
#define IRINSTR_H


#include <vector>
#include <string>
#include <iostream>
#include <initializer_list>

#include "SymbolTable.h"


class BasicBlock;




//! The class for one 3-address instruction
class IRInstr {
 
public:
  /** The instructions themselves -- feel free to subclass instead */
  typedef enum {
    ldconst,
    copy,
    add,
    sub,
    mul,
    div,
    mod,
    neg,
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
    rmem, //Arret ici dans l'implementation
    wmem,
  } Operation;


  /**  constructor */
  IRInstr(BasicBlock* bb_, Operation op, Type t);
  virtual ~IRInstr() {}
	
  /** Actual code generation */
  virtual void gen_asm(std::ostream &o) const = 0; /**< x86 assembly code generation for this IR instruction */
  

  /** Create instance of good IRInstr subclass according to op **/
  static IRInstr* createInstr(BasicBlock *bb, Operation op, Type t, const std::string *params, int nb);

  
protected:
  
  BasicBlock* bb; /**< The BB this instruction belongs to, which provides a pointer to the CFG this instruction belong to */
  Operation op;
  Type t;

  
  
};



class AddInstr : public IRInstr {

public:

  AddInstr (BasicBlock *bb, Type t, const std::string &dest, const std::string &x, const std::string &y);

  virtual void gen_asm(std::ostream &o) const;


private:
  std::string dest;
  std::string x;
  std::string y;
  
};

class SubInstr : public IRInstr {

public:

  SubInstr (BasicBlock *bb, Type t, const std::string &dest, const std::string &x, const std::string &y);

  virtual void gen_asm(std::ostream &o) const;


private:
  std::string dest, x, y;
  
};


class MulInstr : public IRInstr {

public:

  MulInstr (BasicBlock *bb, Type t, const std::string &dest, const std::string &x, const std::string &y);

  virtual void gen_asm(std::ostream &o) const;


private:
  std::string dest, x, y;
  
};

class DivInstr : public IRInstr {

public:

  DivInstr (BasicBlock *bb, Type t, const std::string &dest, const std::string &x, const std::string &y);

  virtual void gen_asm(std::ostream &o) const;


private:
  std::string dest;
  std::string x;
  std::string y;
  
};

class ModInstr : public IRInstr {

public:

  ModInstr (BasicBlock *bb, Type t, const std::string &dest, const std::string &x, const std::string &y);

  virtual void gen_asm(std::ostream &o) const;


private:
  std::string dest, x, y;
  
};




class LdconstInstr : public IRInstr {

public:

  LdconstInstr (BasicBlock *bb, Type t, const std::string &dest, const std::string &val);

  virtual void gen_asm(std::ostream &o) const;

  std::string & getVal() {return val;}

private:

  std::string dest, val;
    
};


class CopyInstr : public IRInstr {

public:

  CopyInstr (BasicBlock *bb, Type t, const std::string &dest, const std::string &var);

  virtual void gen_asm(std::ostream &o) const;

private:

  std::string dest, var;

  
};


class NegInstr : public IRInstr {

public:

  NegInstr (BasicBlock *bb, Type t, const std::string &dest, const std::string &var);

  virtual void gen_asm(std::ostream &o) const;

private:

  std::string dest, var;

  
};

class CallInstr : public IRInstr {


public:

  CallInstr (BasicBlock *bb, Type t, const std::string *params, int nb);
  virtual ~CallInstr() {
    delete [] params;
  }

  virtual void gen_asm(std::ostream &o) const;

private:

  std::string *params;
  int nbParams;

  
  static const std::string regParams[6];

};

class CmpEqInstr : public IRInstr {

public:

  CmpEqInstr (BasicBlock *bb, Type t, const std::string &dest, const std::string &x, const std::string &y);

  virtual void gen_asm(std::ostream &o) const;


private:
  std::string dest, x, y;
  
};

class CmpNeqInstr : public IRInstr {

public:

  CmpNeqInstr (BasicBlock *bb, Type t, const std::string &dest, const std::string &x, const std::string &y);

  virtual void gen_asm(std::ostream &o) const;


private:
  std::string dest, x, y;
  
};

class CmpLtInstr : public IRInstr {

public:

  CmpLtInstr (BasicBlock *bb, Type t, const std::string &dest, const std::string &x, const std::string &y);

  virtual void gen_asm(std::ostream &o) const;


private:
  std::string dest, x, y;
  
};

class CmpGtInstr : public IRInstr {

public:

  CmpGtInstr (BasicBlock *bb, Type t, const std::string &dest, const std::string &x, const std::string &y);

  virtual void gen_asm(std::ostream &o) const;


private:
  std::string dest, x, y;
  
};

class BinaryAndInstr : public IRInstr {

public:

  BinaryAndInstr (BasicBlock *bb, Type t, const std::string &dest, const std::string &x, const std::string &y);

  virtual void gen_asm(std::ostream &o) const;

private:

  std::string dest, x, y;

  
};

class BinaryXorInstr : public IRInstr {

public:

  BinaryXorInstr (BasicBlock *bb, Type t, const std::string &dest, const std::string &x, const std::string &y);

  virtual void gen_asm(std::ostream &o) const;

private:

  std::string dest, x, y;

  
};

class BinaryOrInstr : public IRInstr {

public:

  BinaryOrInstr (BasicBlock *bb, Type t, const std::string &dest, const std::string &x, const std::string &y);

  virtual void gen_asm(std::ostream &o) const;

private:

  std::string dest, x, y;

  
};

class IfElseInstr : public IRInstr {

  public :
    IfElseInstr (BasicBlock *bb, Type t, const std::string &cond);

    virtual void gen_asm(std::ostream &o) const;

  private: 
    std::string cond;
};

class WhileInstr : public IRInstr {

  public :
    WhileInstr (BasicBlock *bb, Type t, const std::string &cond);

    virtual void gen_asm(std::ostream &o) const;

  private: 
    std::string cond;
};

class JmpInstr : public IRInstr {

  public :
    JmpInstr (BasicBlock *bb, Type t, const std::string &dest);

    virtual void gen_asm(std::ostream &o) const;

  private: 
    std::string dest;
};


class GetParamsInstr : public IRInstr {


public:

  GetParamsInstr (BasicBlock *bb, Type t, const std::string *params, int nb);
  virtual ~GetParamsInstr() {
    delete [] params;
  }

  virtual void gen_asm(std::ostream &o) const;

private:

  std::string *params;
  int nbParams;

  
  static const std::string regParams[6];

};

#endif
