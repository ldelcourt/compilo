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
    binary_and,
    binary_xor,
    binary_or,
    rmem, //Arret ici dans l'implementation
    wmem,
    call, 
    cmp_eq,
    cmp_lt,
    cmp_le
  } Operation;


  /**  constructor */
  IRInstr(BasicBlock* bb_, Operation op, Type t);
	
  /** Actual code generation */
  virtual void gen_asm(std::ostream &o) const = 0; /**< x86 assembly code generation for this IR instruction */
  

  /** Create instance of good IRInstr subclass according to op **/
  static IRInstr* createInstr(BasicBlock *bb, Operation op, Type t, std::string *params);

  
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


#endif
