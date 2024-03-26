#include "IRInstr.h"
#include "IR.h"


IRInstr::IRInstr( BasicBlock *bb_, Operation op, Type t) :
  bb (bb_), op(op), t(t)
{

}

IRInstr* IRInstr::createInstr(BasicBlock *bb, Operation op, Type t, const std::string *params, int nb) {

  switch(op) {

  case ldconst:
    return new LdconstInstr(bb, t, params[0], params[1]);

  case copy:
    return new CopyInstr(bb, t, params[0], params[1]);

  case add:
    return new AddInstr(bb, t, params[0], params[1], params[2]);

  case sub:
    return new SubInstr(bb, t, params[0], params[1], params[2]);

  case mul:
    return new MulInstr(bb, t, params[0], params[1], params[2]);

  case div:
    return new DivInstr(bb, t, params[0], params[1], params[2]);

  case mod:
    return new ModInstr(bb, t, params[0], params[1], params[2]);

  case neg:
    return new NegInstr(bb, t, params[0], params[1]);

  case call:
    return new CallInstr(bb, t, params, nb);

  }

  return nullptr;

}


AddInstr::AddInstr(BasicBlock *bb, Type t, const std::string &dest, const std::string &x, const std::string &y) :
  IRInstr(bb, Operation::add, t), dest(dest), x(x), y(y)
{

  //std::cout << "add : x=" << x << " y=" << y << " dest=" << dest << std::endl;
}


void AddInstr::gen_asm(std::ostream &o) const {

  o << " \tmovl " << bb->cfg->symbol_to_asm(x) << ", %edx\n";
  o << " \tmovl " << bb->cfg->symbol_to_asm(y) << ", %eax\n";
  o << " \taddl %edx, %eax\n";
  o << " \tmovl %eax, " << bb->cfg->symbol_to_asm(dest) << "\n";

}


SubInstr::SubInstr(BasicBlock *bb, Type t, const std::string &dest, const std::string &x, const std::string &y) :
  IRInstr(bb, Operation::sub, t), dest(dest), x(x), y(y)
{
  //std::cout << "sub : x=" << x << " y=" << y << " dest=" << dest << std::endl;
}


void SubInstr::gen_asm(std::ostream &o) const {

  o << " \tmovl " << bb->cfg->symbol_to_asm(x) << ", %eax\n";
  o << " \tsubl " << bb->cfg->symbol_to_asm(y) << ", %eax\n";
  o << " \tmovl %eax, " << bb->cfg->symbol_to_asm(dest) << "\n";

}

MulInstr::MulInstr(BasicBlock *bb, Type t, const std::string &dest, const std::string &x, const std::string &y) :
  IRInstr(bb, Operation::mul, t), dest(dest), x(x), y(y)
{
  //std::cout << "mul : x=" << x << " y=" << y << " dest=" << dest << std::endl;
}


void MulInstr::gen_asm(std::ostream &o) const {

  o << " \tmovl " << bb->cfg->symbol_to_asm(x) << ", %eax\n";
  o << " \timull " << bb->cfg->symbol_to_asm(y) << ", %eax\n";
  o << " \tmovl %eax, " << bb->cfg->symbol_to_asm(dest) << "\n";

}


DivInstr::DivInstr(BasicBlock *bb, Type t, const std::string &dest, const std::string &x, const std::string &y) :
  IRInstr(bb, Operation::div, t), dest(dest), x(x), y(y)
{
  //std::cout << "div : x=" << x << " y=" << y << " dest=" << dest << std::endl;
}


void DivInstr::gen_asm(std::ostream &o) const {

  std::string t = y;
  if (bb->cfg->symbolIsConst(y)) {
    t = bb->cfg->createTempVar();
    o << " \tmovl " << bb->cfg->symbol_to_asm(y) << ", " << bb->cfg->symbol_to_asm(t) << "\n";
  }
    
  o << " \tmovl " << bb->cfg->symbol_to_asm(x) << ", %eax\n";
  o << " \tcltd\n";
  o << " \tidivl " << bb->cfg->symbol_to_asm(t) << "\n";

  o << " \tmovl %eax, " << bb->cfg->symbol_to_asm(dest) << "\n";

}


ModInstr::ModInstr(BasicBlock *bb, Type t, const std::string &dest, const std::string &x, const std::string &y) :
  IRInstr(bb, Operation::mod, t), dest(dest), x(x), y(y)
{
  //std::cout << "mod : x=" << x << " y=" << y << " dest=" << dest << std::endl;
}


void ModInstr::gen_asm(std::ostream &o) const {

  std::string t = y;
  if (bb->cfg->symbolIsConst(y)) {
    t = bb->cfg->createTempVar();
    o << " \tmovl " << bb->cfg->symbol_to_asm(y) << ", " << bb->cfg->symbol_to_asm(t) << "\n";
  }
    
  o << " \tmovl " << bb->cfg->symbol_to_asm(x) << ", %eax\n";
  o << " \tcltd\n";
  o << " \tidivl " << bb->cfg->symbol_to_asm(t) << "\n";

  o << " \tmovl %edx, " << bb->cfg->symbol_to_asm(dest) << "\n";

}





LdconstInstr::LdconstInstr(BasicBlock *bb, Type t, const std::string &dest, const std::string &val) :
  IRInstr(bb, Operation::ldconst, t), dest(dest), val(val)
{

  //std::cout << "ldconst : val=" << val << " dest=" << dest << std::endl;
  
}


void LdconstInstr::gen_asm(std::ostream &o) const {

  o << " \tmovl " << bb->cfg->symbol_to_asm(val) << ", " << bb->cfg->symbol_to_asm(dest) << "\n";
  
}


CopyInstr::CopyInstr(BasicBlock *bb, Type t, const std::string &dest, const std::string &var) :
  IRInstr(bb, Operation::copy, t), dest(dest), var(var)
{

  //std::cout << "copy : var=" << var << " dest=" << dest << std::endl;

}


void CopyInstr::gen_asm(std::ostream &o) const {

  o << " \tmovl " << bb->cfg->symbol_to_asm(var) << ", %eax\n";

  if (bb->cfg->symbol_to_asm(dest) != "%eax") {
    o << " \tmovl %eax, " << bb->cfg->symbol_to_asm(dest) << "\n";
  }

}


NegInstr::NegInstr(BasicBlock *bb, Type t, const std::string &dest, const std::string &var) :
  IRInstr(bb, Operation::neg, t), dest(dest), var(var)
{

  //std::cout << "copy : var=" << var << " dest=" << dest << std::endl;

}


void NegInstr::gen_asm(std::ostream &o) const {

  o << " \tmovl " << bb->cfg->symbol_to_asm(var) << ", %eax\n";
  o << " \tnegl %eax\n";
  o << " \tmovl %eax, " << bb->cfg->symbol_to_asm(dest) << "\n";
  
}



CallInstr::CallInstr(BasicBlock *bb, Type t, const std::string *params, int nb) :
  IRInstr(bb, Operation::call, t)
{


  this->params = new std::string[nb];
  nbParams = nb;

  for (int i = 0; i < nb; i++) {
    this->params[i] = params[i];
  }
  
  //std::cout << "copy : var=" << var << " dest=" << dest << std::endl;

}


void CallInstr::gen_asm(std::ostream &o) const {

  const std::string regParams[] = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};

  for (int i = nbParams-1; i >= 1; i--) {
    
    o << " \tmov " << bb->cfg->symbol_to_asm(params[i]) << ", " << regParams[i-1] << "\n";

  }

  o << " \tcall " << params[0] << "\n";
  
}
