#include "IRInstr.h"
#include "IR.h"


IRInstr::IRInstr( BasicBlock *bb_, Operation op, Type t) :
  bb (bb_), op(op), t(t)
{

}

IRInstr* IRInstr::createInstr(BasicBlock *bb, Operation op, Type t, std::string *params) {

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

  case binary_and:
    return new BinaryAndInstr(bb, t, params[0], params[1], params[2]);

  case binary_xor:
    return new BinaryXorInstr(bb, t, params[0], params[1], params[2]);

  case binary_or:
    return new BinaryOrInstr(bb, t, params[0], params[1], params[2]);
  
  case cmp_eq:
    return new CmpEqInstr(bb, t, params[0], params[1], params[2]);

  case cmp_neq:
    return new CmpNeqInstr(bb, t, params[0], params[1], params[2]);

  case cmp_lt:
    return new CmpLtInstr(bb, t, params[0], params[1], params[2]);

  case cmp_gt:
    return new CmpGtInstr(bb, t, params[0], params[1], params[2]); 

  case ifelse:
    return new IfElseInstr(bb, t, params[0]);

  case while_:
    return new WhileInstr(bb, t, params[0]);

  case jmp:
    return new JmpInstr(bb, t, params[0]);   

  }

  return nullptr;

}


AddInstr::AddInstr(BasicBlock *bb, Type t, const std::string &dest, const std::string &x, const std::string &y) :
  IRInstr(bb, Operation::add, t), dest(dest), x(x), y(y)
{
  if(bb->cfg->debug){
    std::cout << "add : x=" << x << " y=" << y << " dest=" << dest << std::endl;
  }
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
  if(bb->cfg->debug){
    std::cout << "sub : x=" << x << " y=" << y << " dest=" << dest << std::endl;
  }
}


void SubInstr::gen_asm(std::ostream &o) const {

  o << " \tmovl " << bb->cfg->symbol_to_asm(x) << ", %eax\n";
  o << " \tsubl " << bb->cfg->symbol_to_asm(y) << ", %eax\n";
  o << " \tmovl %eax, " << bb->cfg->symbol_to_asm(dest) << "\n";

}

MulInstr::MulInstr(BasicBlock *bb, Type t, const std::string &dest, const std::string &x, const std::string &y) :
  IRInstr(bb, Operation::mul, t), dest(dest), x(x), y(y)
{
  if(bb->cfg->debug){
    std::cout << "mul : x=" << x << " y=" << y << " dest=" << dest << std::endl;
  }
}


void MulInstr::gen_asm(std::ostream &o) const {

  o << " \tmovl " << bb->cfg->symbol_to_asm(x) << ", %eax\n";
  o << " \timull " << bb->cfg->symbol_to_asm(y) << ", %eax\n";
  o << " \tmovl %eax, " << bb->cfg->symbol_to_asm(dest) << "\n";

}


DivInstr::DivInstr(BasicBlock *bb, Type t, const std::string &dest, const std::string &x, const std::string &y) :
  IRInstr(bb, Operation::div, t), dest(dest), x(x), y(y)
{
  if(bb->cfg->debug){
    std::cout << "div : x=" << x << " y=" << y << " dest=" << dest << std::endl;
  }
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
  if(bb->cfg->debug){
    std::cout << "mod : x=" << x << " y=" << y << " dest=" << dest << std::endl;
  }
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

  if(bb->cfg->debug){
    std::cout << "ldconst : val=" << val << " dest=" << dest << std::endl;
  }
  
}


void LdconstInstr::gen_asm(std::ostream &o) const {

  o << " \tmovl " << bb->cfg->symbol_to_asm(val) << ", " << bb->cfg->symbol_to_asm(dest) << "\n";
  
}


CopyInstr::CopyInstr(BasicBlock *bb, Type t, const std::string &dest, const std::string &var) :
  IRInstr(bb, Operation::copy, t), dest(dest), var(var)
{

  if(bb->cfg->debug){
    std::cout << "copy : var=" << var << " dest=" << dest << std::endl;
  }

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

  if(bb->cfg->debug){
    std::cout << "copy : var=" << var << " dest=" << dest << std::endl;
  }

}


void NegInstr::gen_asm(std::ostream &o) const {

  std::cout << " \tmovl " << bb->cfg->symbol_to_asm(var) << ", %eax\n";
  std::cout << " \tnegl %eax\n";
  std::cout << " \tmovl %eax, " << bb->cfg->symbol_to_asm(dest) << "\n";
  
}

CmpEqInstr::CmpEqInstr(BasicBlock *bb, Type t, const std::string &dest, const std::string &x, const std::string &y) :
  IRInstr(bb, Operation::cmp_eq, t), dest(dest), x(x), y(y)
{
  if(bb->cfg->debug){
    std::cout << "CmpEq : x=" << x << " y=" << y << " dest=" << dest << std::endl;
  }
}


void CmpEqInstr::gen_asm(std::ostream &o) const {

  o << " \tmovl " << bb->cfg->symbol_to_asm(x) << ", %eax\n";
  o << " \tcmpl " << bb->cfg->symbol_to_asm(y) << ", %eax\n";
  o << " \tsete	  %al\n";
  o << " \tmovzbl	%al, %eax\n";
  o << " \tmovl %eax, " << bb->cfg->symbol_to_asm(dest) << "\n";

}

CmpNeqInstr::CmpNeqInstr(BasicBlock *bb, Type t, const std::string &dest, const std::string &x, const std::string &y) :
  IRInstr(bb, Operation::cmp_neq, t), dest(dest), x(x), y(y)
{
  if(bb->cfg->debug){
    std::cout << "CmpNeq : x=" << x << " y=" << y << " dest=" << dest << std::endl;
  }
}


void CmpNeqInstr::gen_asm(std::ostream &o) const {

  o << " \tmovl " << bb->cfg->symbol_to_asm(x) << ", %eax\n";
  o << " \tcmpl " << bb->cfg->symbol_to_asm(y) << ", %eax\n";
  o << " \tsetne	  %al\n";
  o << " \tmovzbl	%al, %eax\n";
  o << " \tmovl %eax, " << bb->cfg->symbol_to_asm(dest) << "\n";

}

CmpLtInstr::CmpLtInstr(BasicBlock *bb, Type t, const std::string &dest, const std::string &x, const std::string &y) :
  IRInstr(bb, Operation::cmp_lt, t), dest(dest), x(x), y(y)
{
  if(bb->cfg->debug){
    std::cout << "CmpLt : x=" << x << " y=" << y << " dest=" << dest << std::endl;
  }
}


void CmpLtInstr::gen_asm(std::ostream &o) const {

  o << " \tmovl " << bb->cfg->symbol_to_asm(x) << ", %eax\n";
  o << " \tcmpl " << bb->cfg->symbol_to_asm(y) << ", %eax\n";
  o << " \tsetl	  %al\n";
  o << " \tmovzbl	%al, %eax\n";
  o << " \tmovl %eax, " << bb->cfg->symbol_to_asm(dest) << "\n";

}

CmpGtInstr::CmpGtInstr(BasicBlock *bb, Type t, const std::string &dest, const std::string &x, const std::string &y) :
  IRInstr(bb, Operation::cmp_gt, t), dest(dest), x(x), y(y)
{
  if(bb->cfg->debug){
    std::cout << "CmpGt : x=" << x << " y=" << y << " dest=" << dest << std::endl;
  }
}


void CmpGtInstr::gen_asm(std::ostream &o) const {

  o << " \tmovl " << bb->cfg->symbol_to_asm(x) << ", %eax\n";
  o << " \tcmpl " << bb->cfg->symbol_to_asm(y) << ", %eax\n";
  o << " \tsetg	  %al\n";
  o << " \tmovzbl	%al, %eax\n";
  o << " \tmovl %eax, " << bb->cfg->symbol_to_asm(dest) << "\n";

}
BinaryAndInstr::BinaryAndInstr(BasicBlock *bb, Type t, const std::string &dest, const std::string &x, const std::string &y) :
  IRInstr(bb, Operation::binary_and, t), dest(dest), x(x), y(y)
{

  if(bb->cfg->debug){
    std::cout << "BinaryAnd : x=" << x << " y=" << y << " dest=" << dest << std::endl;
  }

}


void BinaryAndInstr::gen_asm(std::ostream &o) const {

  o << " \tmovl " << bb->cfg->symbol_to_asm(x) << ", %eax\n";
  o << " \tandl " << bb->cfg->symbol_to_asm(y) << ", %eax\n";
  o << " \tmovl %eax, " << bb->cfg->symbol_to_asm(dest) << "\n";
  
}

BinaryXorInstr::BinaryXorInstr(BasicBlock *bb, Type t, const std::string &dest, const std::string &x, const std::string &y) :
  IRInstr(bb, Operation::binary_xor, t), dest(dest), x(x), y(y)
{

  if(bb->cfg->debug){
    std::cout << "BinaryXor : x=" << x << " y=" << y << " dest=" << dest << std::endl;
  }

}


void BinaryXorInstr::gen_asm(std::ostream &o) const {

  o << " \tmovl " << bb->cfg->symbol_to_asm(x) << ", %eax\n";
  o << " \txorl " << bb->cfg->symbol_to_asm(y) << ", %eax\n";
  o << " \tmovl %eax, " << bb->cfg->symbol_to_asm(dest) << "\n";
  
}

BinaryOrInstr::BinaryOrInstr(BasicBlock *bb, Type t, const std::string &dest, const std::string &x, const std::string &y) :
  IRInstr(bb, Operation::binary_or, t), dest(dest), x(x), y(y)
{

  if(bb->cfg->debug){
    std::cout << "BinaryOr : x=" << x << " y=" << y << " dest=" << dest << std::endl;
  }

}


void BinaryOrInstr::gen_asm(std::ostream &o) const {

  o << " \tmovl " << bb->cfg->symbol_to_asm(x) << ", %eax\n";
  o << " \torl " << bb->cfg->symbol_to_asm(y) << ", %eax\n";
  o << " \tmovl %eax, " << bb->cfg->symbol_to_asm(dest) << "\n";
  
}

IfElseInstr::IfElseInstr(BasicBlock *bb, Type t, const std::string &cond) :
  IRInstr(bb, Operation::ifelse, t), cond(cond)
{
  if(bb->cfg->debug){
    std::cout << "IfElse : cond=" << cond << " ifTrue=" << bb->exit_true->label << " ifFalse=" << bb->exit_false->label << std::endl;
  }

}

void IfElseInstr::gen_asm(std::ostream &o) const {

  o << " \tcmpl $0, " << bb->cfg->symbol_to_asm(cond) << "\n";
  o << " \tje " << bb->exit_false->label << "\n";
  o << " \tjmp " << bb->exit_true->label << "\n";
  
}

WhileInstr::WhileInstr(BasicBlock *bb, Type t, const std::string &cond) :
  IRInstr(bb, Operation::while_, t), cond(cond)
{
  if(bb->cfg->debug){
    std::cout << "While : cond=" << cond << " whileTrue=" << bb->exit_true->label << " whileFalse=" << bb->exit_false->label << std::endl;
  }

}

void WhileInstr::gen_asm(std::ostream &o) const {

  o << " \tcmpl $0, " << bb->cfg->symbol_to_asm(cond) << "\n";
  o << " \tje " << bb->exit_false->label << "\n";
  o << " \tjmp " << bb->exit_true->label << "\n";
  
}

JmpInstr::JmpInstr(BasicBlock *bb, Type t, const std::string &dest) :
  IRInstr(bb, Operation::jmp, t), dest(dest)
{
  if(bb->cfg->debug){
    std::cout << "Jmp : dest=" << dest << std::endl;
  }

}

void JmpInstr::gen_asm(std::ostream &o) const {

  o << " \tjmp " << dest << "\n";
  
}


