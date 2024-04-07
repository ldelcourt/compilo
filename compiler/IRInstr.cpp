#include "IRInstr.h"
#include "IR.h"

IRInstr::IRInstr(BasicBlock *bb_, Operation op, Type t)
    : bb(bb_), op(op), t(t) {}

IRInstr *IRInstr::createInstr(BasicBlock *bb, Operation op, Type t,
                              const std::string *params, int nb) {

  switch (op) {

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

  case logiNot:
    return new LogiNotInstr(bb, t, params[0], params[1]);

  case call:
    return new CallInstr(bb, t, params, nb);

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

  case getparams:
    return new GetParamsInstr(bb, t, params, nb);

  case ret:
    return new RetInstr(bb, t, params[0]);

  case getret:
    return new GetRetInstr(bb,t, params[0]);

  }

  return nullptr;
}

void IRInstr::gen_asm(std::ostream &o) const {
  if (bb->getCFG()->getAssemblyLangage() == ARM) {
    gen_arm(o);
  } else if (bb->getCFG()->getAssemblyLangage() == x86) {
    gen_x86_asm(o);
  }
}

AddInstr::AddInstr(BasicBlock *bb, Type t, const std::string &dest, const std::string &x, const std::string &y) :
  IRInstr(bb, Operation::add, t), dest(dest), x(x), y(y)
{
  if(bb->getCFG()->debug()){
    std::cout << "add : x=" << x << " y=" << y << " dest=" << dest << std::endl;
  }
}

void AddInstr::gen_x86_asm(std::ostream &o) const {

  o << " \tmovl " << bb->getCFG()->symbol_to_asm(x) << ", %edx\n";
  o << " \tmovl " << bb->getCFG()->symbol_to_asm(y) << ", %eax\n";
  o << " \taddl %edx, %eax\n";
  o << " \tmovl %eax, " << bb->getCFG()->symbol_to_asm(dest) << "\n";

}

void AddInstr::gen_arm(std::ostream &o) const {
  if (bb->getCFG()->symbolIsConst(x)) {
    o << " \tmov w8, " << bb->getCFG()->symbol_to_asm(x) << "\n";
  } else {
    o << " \tldr w8, " << bb->getCFG()->symbol_to_asm(x) << "\n";
  }
  if (bb->getCFG()->symbolIsConst(y)) {
    o << " \tmov w9, " << bb->getCFG()->symbol_to_asm(y) << "\n";
  } else {
    o << " \tldr w9, " << bb->getCFG()->symbol_to_asm(y) << "\n";
  }
  o << " \tadd w8, w8, w9\n";
  o << " \tstr w8, " << bb->getCFG()->symbol_to_asm(dest) << "\n";
}

SubInstr::SubInstr(BasicBlock *bb, Type t, const std::string &dest, const std::string &x, const std::string &y) :
  IRInstr(bb, Operation::sub, t), dest(dest), x(x), y(y)
{
  if(bb->getCFG()->debug()){
    std::cout << "sub : x=" << x << " y=" << y << " dest=" << dest << std::endl;
  }
}

void SubInstr::gen_x86_asm(std::ostream &o) const {

  o << " \tmovl " << bb->getCFG()->symbol_to_asm(x) << ", %eax\n";
  o << " \tsubl " << bb->getCFG()->symbol_to_asm(y) << ", %eax\n";
  o << " \tmovl %eax, " << bb->getCFG()->symbol_to_asm(dest) << "\n";

}

void SubInstr::gen_arm(std::ostream &o) const {

  if (bb->getCFG()->symbolIsConst(x)) {
    o << " \tmov w8, " << bb->getCFG()->symbol_to_asm(x) << "\n";
  } else {
    o << " \tldr w8, " << bb->getCFG()->symbol_to_asm(x) << "\n";
  }
  if (bb->getCFG()->symbolIsConst(y)) {
    o << " \tmov w9, " << bb->getCFG()->symbol_to_asm(y) << "\n";
  } else {
    o << " \tldr w9, " << bb->getCFG()->symbol_to_asm(y) << "\n";
  }
  o << " \tsubs w8, w8, w9\n";
  o << " \tstr w8, " << bb->getCFG()->symbol_to_asm(dest) << "\n";
}

MulInstr::MulInstr(BasicBlock *bb, Type t, const std::string &dest, const std::string &x, const std::string &y) :
  IRInstr(bb, Operation::mul, t), dest(dest), x(x), y(y)
{
  if(bb->getCFG()->debug()){
    std::cout << "mul : x=" << x << " y=" << y << " dest=" << dest << std::endl;
  }
}

void MulInstr::gen_x86_asm(std::ostream &o) const {

  o << " \tmovl " << bb->getCFG()->symbol_to_asm(x) << ", %eax\n";
  o << " \timull " << bb->getCFG()->symbol_to_asm(y) << ", %eax\n";
  o << " \tmovl %eax, " << bb->getCFG()->symbol_to_asm(dest) << "\n";

}

void MulInstr::gen_arm(std::ostream &o) const {
  if (bb->getCFG()->symbolIsConst(x)) {
    o << " \tmov w8, " << bb->getCFG()->symbol_to_asm(x) << "\n";
  } else {
    o << " \tldr w8, " << bb->getCFG()->symbol_to_asm(x) << "\n";
  }
  if (bb->getCFG()->symbolIsConst(y)) {
    o << " \tmov w9, " << bb->getCFG()->symbol_to_asm(y) << "\n";
  } else {
    o << " \tldr w9, " << bb->getCFG()->symbol_to_asm(y) << "\n";
  }
  o << " \tmul w8, w8, w9\n";
  o << " \tstr w8, " << bb->getCFG()->symbol_to_asm(dest) << "\n";
}

DivInstr::DivInstr(BasicBlock *bb, Type t, const std::string &dest, const std::string &x, const std::string &y) :
  IRInstr(bb, Operation::div, t), dest(dest), x(x), y(y)
{
  if(bb->getCFG()->debug()){
    std::cout << "div : x=" << x << " y=" << y << " dest=" << dest << std::endl;
  }
}

void DivInstr::gen_x86_asm(std::ostream &o) const {

  std::string t = y;
  if (bb->getCFG()->symbolIsConst(y)) {
    t = bb->getCFG()->createTempVar();
    o << " \tmovl " << bb->getCFG()->symbol_to_asm(y) << ", " << bb->getCFG()->symbol_to_asm(t) << "\n";
  }
    
  o << " \tmovl " << bb->getCFG()->symbol_to_asm(x) << ", %eax\n";
  o << " \tcltd\n";
  o << " \tidivl " << bb->getCFG()->symbol_to_asm(t) << "\n";

  o << " \tmovl %eax, " << bb->getCFG()->symbol_to_asm(dest) << "\n";

}

void DivInstr::gen_arm(std::ostream &o) const {
  if (bb->getCFG()->symbolIsConst(x)) {
    o << " \tmov w8, " << bb->getCFG()->symbol_to_asm(x) << "\n";
  } else {
    o << " \tldr w8, " << bb->getCFG()->symbol_to_asm(x) << "\n";
  }
  if (bb->getCFG()->symbolIsConst(y)) {
    o << " \tmov w9, " << bb->getCFG()->symbol_to_asm(y) << "\n";
  } else {
    o << " \tldr w9, " << bb->getCFG()->symbol_to_asm(y) << "\n";
  }
  o << " \tsdiv w8, w8, w9\n";
  o << " \tstr w8, " << bb->getCFG()->symbol_to_asm(dest) << "\n";
}

ModInstr::ModInstr(BasicBlock *bb, Type t, const std::string &dest, const std::string &x, const std::string &y) :
  IRInstr(bb, Operation::mod, t), dest(dest), x(x), y(y)
{
  if(bb->getCFG()->debug()){
    std::cout << "mod : x=" << x << " y=" << y << " dest=" << dest << std::endl;
  }
}

void ModInstr::gen_x86_asm(std::ostream &o) const {

  std::string t = y;
  if (bb->getCFG()->symbolIsConst(y)) {
    t = bb->getCFG()->createTempVar();
    o << " \tmovl " << bb->getCFG()->symbol_to_asm(y) << ", " << bb->getCFG()->symbol_to_asm(t) << "\n";
  }
    
  o << " \tmovl " << bb->getCFG()->symbol_to_asm(x) << ", %eax\n";
  o << " \tcltd\n";
  o << " \tidivl " << bb->getCFG()->symbol_to_asm(t) << "\n";

  o << " \tmovl %edx, " << bb->getCFG()->symbol_to_asm(dest) << "\n";

}

void ModInstr::gen_arm(std::ostream &o) const {

  if (bb->getCFG()->symbolIsConst(x)) {
    o << " \tmov w8, " << bb->getCFG()->symbol_to_asm(x) << "\n";
  } else {
    o << " \tldr w8, " << bb->getCFG()->symbol_to_asm(x) << "\n";
  }
  if (bb->getCFG()->symbolIsConst(y)) {
    o << " \tmov w10, " << bb->getCFG()->symbol_to_asm(y) << "\n";
  } else {
    o << " \tldr w10, " << bb->getCFG()->symbol_to_asm(y) << "\n";
  }
  o << " \tsdiv w9, w8, w10\n";
  o << " \tmul w9, w9, w10\n";
  o << " \tsubs w8, w8, w9\n";
  o << " \tstr w8, " << bb->getCFG()->symbol_to_asm(dest) << "\n";
}


LdconstInstr::LdconstInstr(BasicBlock *bb, Type t, const std::string &dest, const std::string &val)
  : IRInstr(bb, Operation::ldconst, t), dest(dest), val(val)
{

  if(bb->getCFG()->debug()){
    std::cout << "ldconst : val=" << val << " dest=" << dest << std::endl;
  }
}

void LdconstInstr::gen_x86_asm(std::ostream &o) const {

  o << " \tmovl " << bb->getCFG()->symbol_to_asm(val) << ", " << bb->getCFG()->symbol_to_asm(dest) << "\n";
  
}

void LdconstInstr::gen_arm(std::ostream &o) const {
  o << " \tmov w8, " << bb->getCFG()->symbol_to_asm(val) << "\n";
  o << " \tstr w8, " << bb->getCFG()->symbol_to_asm(dest) << "\n";
  
}

CopyInstr::CopyInstr(BasicBlock *bb, Type t, const std::string &dest, const std::string &var)
  : IRInstr(bb, Operation::copy, t), dest(dest), var(var)
{

  if(bb->getCFG()->debug()){
    std::cout << "copy : var=" << var << " dest=" << dest << std::endl;
  }
}

void CopyInstr::gen_x86_asm(std::ostream &o) const {

  o << " \tmovl " << bb->getCFG()->symbol_to_asm(var) << ", %eax\n";

  if (bb->getCFG()->symbol_to_asm(dest) != "%eax") {
    o << " \tmovl %eax, " << bb->getCFG()->symbol_to_asm(dest) << "\n";
  }
}

void CopyInstr::gen_arm(std::ostream &o) const {

  if (bb->getCFG()->symbolIsConst(var)) {
    o << " \tmov w8, " << bb->getCFG()->symbol_to_asm(var) << "\n";
  } else {
    o << " \tldr w8, " << bb->getCFG()->symbol_to_asm(var) << "\n";
  }
  o << " \tstr w8, " << bb->getCFG()->symbol_to_asm(dest) << "\n";
}

NegInstr::NegInstr(BasicBlock *bb, Type t, const std::string &dest, const std::string &var)
  : IRInstr(bb, Operation::neg, t), dest(dest), var(var)
{

  if(bb->getCFG()->debug()){
    std::cout << "copy : var=" << var << " dest=" << dest << std::endl;
  }
}



void NegInstr::gen_x86_asm(std::ostream &o) const {

  o << " \tmovl " << bb->getCFG()->symbol_to_asm(var) << ", %eax\n";
  o << " \tnegl %eax\n";
  o << " \tmovl %eax, " << bb->getCFG()->symbol_to_asm(dest) << "\n";
  
}

void NegInstr::gen_arm(std::ostream &o) const {

  o << " \tmov w8, #0\n";
  if (bb->getCFG()->symbolIsConst(dest)) {
    o << " \tmov w9, " << bb->getCFG()->symbol_to_asm(var) << "\n";
  } else {
    o << " \tldr w9, " << bb->getCFG()->symbol_to_asm(var) << "\n";
  }
  o << " \tsubs w8, w8, w9\n";
  o << " \tstr w8, " << bb->getCFG()->symbol_to_asm(dest) << "\n";
}

LogiNotInstr::LogiNotInstr(BasicBlock *bb, Type t, const std::string &dest, const std::string &var)
  : IRInstr(bb, Operation::logiNot, t), dest(dest), var(var)
{

  if(bb->getCFG()->debug()){
    std::cout << "loginot : var=" << var << " dest=" << dest << std::endl;
  }
}

void LogiNotInstr::gen_x86_asm(std::ostream &o) const {

  o << " \tcmpl $0, " << bb->getCFG()->symbol_to_asm(var) << "\n";
  o << " \tsete %al\n";
  o << " \tmovzbl %al, %eax\n";
  o << " \tmovl %eax, " << bb->getCFG()->symbol_to_asm(dest) << "\n";
}

void LogiNotInstr::gen_arm(std::ostream &o) const {
  if (bb->getCFG()->symbolIsConst(var)) {
    o << " \tmov w8, " << bb->getCFG()->symbol_to_asm(var) << "\n";
  } else {
    o << " \tldr w8, " << bb->getCFG()->symbol_to_asm(var) << "\n";
  }
  o << " \tcset w8, eq\n";
  o << " \tand w8, w8, #0x1\n";
  o << " \tstr w8, " << bb->getCFG()->symbol_to_asm(dest) << "\n";
}

CmpEqInstr::CmpEqInstr(BasicBlock *bb, Type t, const std::string &dest, const std::string &x, const std::string &y)
  : IRInstr(bb, Operation::cmp_eq, t), dest(dest), x(x), y(y)
{
  if(bb->getCFG()->debug()){
    std::cout << "CmpEq : x=" << x << " y=" << y << " dest=" << dest << std::endl;
  }
}

void CmpEqInstr::gen_x86_asm(std::ostream &o) const {

  o << " \tmovl " << bb->getCFG()->symbol_to_asm(x) << ", %eax\n";
  o << " \tcmpl " << bb->getCFG()->symbol_to_asm(y) << ", %eax\n";
  o << " \tsete	  %al\n";
  o << " \tmovzbl	%al, %eax\n";
  o << " \tmovl %eax, " << bb->getCFG()->symbol_to_asm(dest) << "\n";

}

void CmpEqInstr::gen_arm(std::ostream &o) const {
  if (bb->getCFG()->symbolIsConst(x)) {
    o << " \tmov w8, " << bb->getCFG()->symbol_to_asm(x) << "\n";
  } else {
    o << " \tldr w8, " << bb->getCFG()->symbol_to_asm(x) << "\n";
  }
  if (bb->getCFG()->symbolIsConst(y)) {
    o << " \tmov w9, " << bb->getCFG()->symbol_to_asm(y) << "\n";
  } else {
    o << " \tldr w9, " << bb->getCFG()->symbol_to_asm(y) << "\n";
  }
  o << " \tsubs w8, w8, w9\n";
  o << " \tcset w8, ne\n";
  o << " \tstr w8, " << bb->getCFG()->symbol_to_asm(dest) << "\n";
}

CmpNeqInstr::CmpNeqInstr(BasicBlock *bb, Type t, const std::string &dest, const std::string &x, const std::string &y)
  :IRInstr(bb, Operation::cmp_neq, t), dest(dest), x(x), y(y)
{
  if(bb->getCFG()->debug()){
    std::cout << "CmpNeq : x=" << x << " y=" << y << " dest=" << dest << std::endl;
  }
}

void CmpNeqInstr::gen_x86_asm(std::ostream &o) const {

  o << " \tmovl " << bb->getCFG()->symbol_to_asm(x) << ", %eax\n";
  o << " \tcmpl " << bb->getCFG()->symbol_to_asm(y) << ", %eax\n";
  o << " \tsetne	  %al\n";
  o << " \tmovzbl	%al, %eax\n";
  o << " \tmovl %eax, " << bb->getCFG()->symbol_to_asm(dest) << "\n";

}

void CmpNeqInstr::gen_arm(std::ostream &o) const {
  if (bb->getCFG()->symbolIsConst(x)) {
    o << " \tmov w8, " << bb->getCFG()->symbol_to_asm(x) << "\n";
  } else {
    o << " \tldr w8, " << bb->getCFG()->symbol_to_asm(x) << "\n";
  }
  if (bb->getCFG()->symbolIsConst(y)) {
    o << " \tmov w9, " << bb->getCFG()->symbol_to_asm(y) << "\n";
  } else {
    o << " \tldr w9, " << bb->getCFG()->symbol_to_asm(y) << "\n";
  }
  o << " \tsubs w8, w8, w9\n";
  o << " \tcset w8, eq\n";
  o << " \tstr w8, " << bb->getCFG()->symbol_to_asm(dest) << "\n";
}

CmpLtInstr::CmpLtInstr(BasicBlock *bb, Type t, const std::string &dest, const std::string &x, const std::string &y)
  : IRInstr(bb, Operation::cmp_lt, t), dest(dest), x(x), y(y)
{
  if(bb->getCFG()->debug()){
    std::cout << "CmpLt : x=" << x << " y=" << y << " dest=" << dest << std::endl;
  }
}

void CmpLtInstr::gen_x86_asm(std::ostream &o) const {

  o << " \tmovl " << bb->getCFG()->symbol_to_asm(x) << ", %eax\n";
  o << " \tcmpl " << bb->getCFG()->symbol_to_asm(y) << ", %eax\n";
  o << " \tsetl	  %al\n";
  o << " \tmovzbl	%al, %eax\n";
  o << " \tmovl %eax, " << bb->getCFG()->symbol_to_asm(dest) << "\n";

}

void CmpLtInstr::gen_arm(std::ostream &o) const {
  if (bb->getCFG()->symbolIsConst(x)) {
    o << " \tmov w8, " << bb->getCFG()->symbol_to_asm(x) << "\n";
  } else {
    o << " \tldr w8, " << bb->getCFG()->symbol_to_asm(x) << "\n";
  }
  if (bb->getCFG()->symbolIsConst(y)) {
    o << " \tmov w9, " << bb->getCFG()->symbol_to_asm(y) << "\n";
  } else {
    o << " \tldr w9, " << bb->getCFG()->symbol_to_asm(y) << "\n";
  }
  o << " \tsubs w8, w8, w9\n";
  o << " \tcset w8, ge\n";
  o << " \tstr w8, " << bb->getCFG()->symbol_to_asm(dest) << "\n";
}

CmpGtInstr::CmpGtInstr(BasicBlock *bb, Type t, const std::string &dest, const std::string &x, const std::string &y) :
  IRInstr(bb, Operation::cmp_gt, t), dest(dest), x(x), y(y)
{
  if(bb->getCFG()->debug()){
    std::cout << "CmpGt : x=" << x << " y=" << y << " dest=" << dest << std::endl;
  }
}

void CmpGtInstr::gen_x86_asm(std::ostream &o) const {

  o << " \tmovl " << bb->getCFG()->symbol_to_asm(x) << ", %eax\n";
  o << " \tcmpl " << bb->getCFG()->symbol_to_asm(y) << ", %eax\n";
  o << " \tsetg	  %al\n";
  o << " \tmovzbl	%al, %eax\n";
  o << " \tmovl %eax, " << bb->getCFG()->symbol_to_asm(dest) << "\n";

}

void CmpGtInstr::gen_arm(std::ostream &o) const {
  if (bb->getCFG()->symbolIsConst(x)) {
    o << " \tmov w8, " << bb->getCFG()->symbol_to_asm(x) << "\n";
  } else {
    o << " \tldr w8, " << bb->getCFG()->symbol_to_asm(x) << "\n";
  }
  if (bb->getCFG()->symbolIsConst(y)) {
    o << " \tmov w9, " << bb->getCFG()->symbol_to_asm(y) << "\n";
  } else {
    o << " \tldr w9, " << bb->getCFG()->symbol_to_asm(y) << "\n";
  }
  o << " \tsubs w8, w8, w9\n";
  o << " \tcset w8, le\n";
  o << " \tstr w8, " << bb->getCFG()->symbol_to_asm(dest) << "\n";
}

BinaryAndInstr::BinaryAndInstr(BasicBlock *bb, Type t, const std::string &dest, const std::string &x, const std::string &y)
  : IRInstr(bb, Operation::binary_and, t), dest(dest), x(x), y(y)
{

  if (bb->getCFG()->debug()) {
    std::cout << "BinaryAnd : x=" << x << " y=" << y << " dest=" << dest
              << std::endl;
  }
}

void BinaryAndInstr::gen_x86_asm(std::ostream &o) const {

  o << " \tmovl " << bb->getCFG()->symbol_to_asm(x) << ", %eax\n";
  o << " \tandl " << bb->getCFG()->symbol_to_asm(y) << ", %eax\n";
  o << " \tmovl %eax, " << bb->getCFG()->symbol_to_asm(dest) << "\n";
}

void BinaryAndInstr::gen_arm(std::ostream &o) const {
  if (bb->getCFG()->symbolIsConst(x)) {
    o << " \tmov w8, " << bb->getCFG()->symbol_to_asm(x) << "\n";
  } else {
    o << " \tldr w8, " << bb->getCFG()->symbol_to_asm(x) << "\n";
  }
  if (bb->getCFG()->symbolIsConst(y)) {
    o << " \tmov w9, " << bb->getCFG()->symbol_to_asm(y) << "\n";
  } else {
    o << " \tldr w9, " << bb->getCFG()->symbol_to_asm(y) << "\n";
  }
  o << " \tand w8, w8, w9\n";
  o << " \tstr w8, " << bb->getCFG()->symbol_to_asm(dest) << "\n";
}

BinaryXorInstr::BinaryXorInstr(BasicBlock *bb, Type t, const std::string &dest, const std::string &x, const std::string &y)
  : IRInstr(bb, Operation::binary_xor, t), dest(dest), x(x), y(y)
{
  if (bb->getCFG()->debug()) {
    std::cout << "BinaryXor : x=" << x << " y=" << y << " dest=" << dest
              << std::endl;
  }
}

void BinaryXorInstr::gen_x86_asm(std::ostream &o) const {

  o << " \tmovl " << bb->getCFG()->symbol_to_asm(x) << ", %eax\n";
  o << " \txorl " << bb->getCFG()->symbol_to_asm(y) << ", %eax\n";
  o << " \tmovl %eax, " << bb->getCFG()->symbol_to_asm(dest) << "\n";
}

void BinaryXorInstr::gen_arm(std::ostream &o) const {
  if (bb->getCFG()->symbolIsConst(x)) {
    o << " \tmov w8, " << bb->getCFG()->symbol_to_asm(x) << "\n";
  } else {
    o << " \tldr w8, " << bb->getCFG()->symbol_to_asm(x) << "\n";
  }
  if (bb->getCFG()->symbolIsConst(y)) {
    o << " \tmov w9, " << bb->getCFG()->symbol_to_asm(y) << "\n";
  } else {
    o << " \tldr w9, " << bb->getCFG()->symbol_to_asm(y) << "\n";
  }
  o << " \teor w8, w8, w9\n";
  o << " \tstr w8, " << bb->getCFG()->symbol_to_asm(dest) << "\n";
}

BinaryOrInstr::BinaryOrInstr(BasicBlock *bb, Type t, const std::string &dest, const std::string &x, const std::string &y)
  : IRInstr(bb, Operation::binary_or, t), dest(dest), x(x), y(y)
{

  if (bb->getCFG()->debug()) {
    std::cout << "BinaryOr : x=" << x << " y=" << y << " dest=" << dest
              << std::endl;
  }
}

void BinaryOrInstr::gen_x86_asm(std::ostream &o) const {

  o << " \tmovl " << bb->getCFG()->symbol_to_asm(x) << ", %eax\n";
  o << " \torl " << bb->getCFG()->symbol_to_asm(y) << ", %eax\n";
  o << " \tmovl %eax, " << bb->getCFG()->symbol_to_asm(dest) << "\n";
}

void BinaryOrInstr::gen_arm(std::ostream &o) const {
  if (bb->getCFG()->symbolIsConst(x)) {
    o << " \tmov w8, " << bb->getCFG()->symbol_to_asm(x) << "\n";
  } else {
    o << " \tldr w8, " << bb->getCFG()->symbol_to_asm(x) << "\n";
  }
  if (bb->getCFG()->symbolIsConst(y)) {
    o << " \tmov w9, " << bb->getCFG()->symbol_to_asm(y) << "\n";
  } else {
    o << " \tldr w9, " << bb->getCFG()->symbol_to_asm(y) << "\n";
  }
  o << " \torr w8, w8, w9\n";
  o << " \tstr w8, " << bb->getCFG()->symbol_to_asm(dest) << "\n";
}

IfElseInstr::IfElseInstr(BasicBlock *bb, Type t, const std::string &cond)
  : IRInstr(bb, Operation::ifelse, t), cond(cond)
{
  if (bb->getCFG()->debug()) {
    std::cout << "IfElse : cond=" << cond << " ifTrue=" << bb->exit_true->getLabel()
              << " ifFalse=" << bb->exit_false->getLabel() << std::endl;
  }
}

void IfElseInstr::gen_x86_asm(std::ostream &o) const {

  o << " \tcmpl $0, " << bb->getCFG()->symbol_to_asm(cond) << "\n";
  o << " \tje " << bb->exit_false->getLabel() << "\n";
  o << " \tjmp " << bb->exit_true->getLabel() << "\n";
}

void IfElseInstr::gen_arm(std::ostream &o) const {

  if (bb->getCFG()->symbolIsConst(cond)) {
    o << " \tmov w8, " << bb->getCFG()->symbol_to_asm(cond) << "\n";
  } else {
    o << " \tldr w8, " << bb->getCFG()->symbol_to_asm(cond) << "\n";
  }
  o << " \ttbnz w8, #0, " << bb->exit_false->getLabel() << "\n";
  o << " \tb " << bb->exit_true->getLabel() << "\n";
}

WhileInstr::WhileInstr(BasicBlock *bb, Type t, const std::string &cond)
  : IRInstr(bb, Operation::while_, t), cond(cond)
{
  if (bb->getCFG()->debug()) {
    std::cout << "While : cond=" << cond
              << " whileTrue=" << bb->exit_true->getLabel()
              << " whileFalse=" << bb->exit_false->getLabel() << std::endl;
  }
}

void WhileInstr::gen_x86_asm(std::ostream &o) const {

  o << " \tcmpl $0, " << bb->getCFG()->symbol_to_asm(cond) << "\n";
  o << " \tje " << bb->exit_false->getLabel() << "\n";
  o << " \tjmp " << bb->exit_true->getLabel() << "\n";
  
}

void WhileInstr::gen_arm(std::ostream &o) const {

  if (bb->getCFG()->symbolIsConst(cond)) {
    o << " \tmov w8, " << bb->getCFG()->symbol_to_asm(cond) << "\n";
  } else {
    o << " \tldr w8, " << bb->getCFG()->symbol_to_asm(cond) << "\n";
  }
  o << " \ttbnz w8, #0, " << bb->exit_false->getLabel() << "\n";
  o << " \tb " << bb->exit_true->getLabel() << "\n";
}

JmpInstr::JmpInstr(BasicBlock *bb, Type t, const std::string &dest)
  : IRInstr(bb, Operation::jmp, t), dest(dest)
{
  if (bb->getCFG()->debug()) {
    std::cout << "Jmp : dest=" << dest << std::endl;
  }
}

void JmpInstr::gen_x86_asm(std::ostream &o) const {

  o << " \tjmp " << dest << "\n";
}

void JmpInstr::gen_arm(std::ostream &o) const {
  o << " \tb " << dest << "\n";
}

CallInstr::CallInstr(BasicBlock *bb, Type t, const std::string *params, int nb)
  : IRInstr(bb, Operation::call, t)
{

  this->params = new std::string[nb];
  nbParams = nb;

  for (int i = 0; i < nb; i++) {
    this->params[i] = params[i];
  }
  if(bb->getCFG()->debug()){
    std::cout << "call : func=" << params[0] << std::endl;
  }
}

void CallInstr::gen_x86_asm(std::ostream &o) const {

  const std::string regParams[] = {"%edi", "%esi", "%edx", "%ecx", "%r8d", "%r9d"};

  for (int i = nbParams-1; i >= 1; i--) {
    
    o << " \tmovl " << bb->getCFG()->symbol_to_asm(params[i]) << ", " << regParams[i-1] << "\n";

  }

  o << " \tcall " << params[0] << "\n";
}

void CallInstr::gen_arm(std::ostream &o) const {
  // A faire
}


GetParamsInstr::GetParamsInstr(BasicBlock *bb, Type t, const std::string *params, int nb) :
  IRInstr(bb, Operation::getparams, t)
{


  this->params = new std::string[nb];
  nbParams = nb;

  if(bb->getCFG()->debug()){
    std::cout << "getparams : ";
  }
  
  for (int i = 0; i < nb; i++) {
    this->params[i] = params[i];

    if(bb->getCFG()->debug()){
      std::cout << i << "=" << params[i] <<  std::endl;
    }
  }
  
}


void GetParamsInstr::gen_x86_asm(std::ostream &o) const {

  const std::string regParams[] = {"%edi", "%esi", "%edx", "%ecx", "%r8d", "%r9d"};

  for (int i = 0; i < nbParams; i++) {
    
    o << " \tmovl " << regParams[i] << ", " <<  bb->getCFG()->symbol_to_asm(params[i]) << "\n";

  }

}


RetInstr::RetInstr(BasicBlock *bb, Type t, const std::string &var) :
  IRInstr(bb, Operation::ret, t), var(var)
{

  if(bb->getCFG()->debug()){
    std::cout << "ret : var=" << var << std::endl;
  }

}


void RetInstr::gen_x86_asm(std::ostream &o) const {

  o << " \tmovl " << bb->getCFG()->symbol_to_asm(var) << ", %eax\n";

  o << " \tjmp " << bb->getCFG()->getNameFunction() << "_output\n";

}

void RetInstr::gen_arm(std::ostream &o) const {

  std::string instr = bb->getCFG()->symbolIsConst(var) ? "mov" : "ldr";
  o << " \t" << instr << " w0, " << bb->getCFG()->symbol_to_asm(var) << "\n";

  o << " \tb " <<  bb->getCFG()->getNameFunction() << "_output\n";

}


GetRetInstr::GetRetInstr(BasicBlock *bb, Type t, const std::string &dest) :
  IRInstr(bb, Operation::getret, t), dest(dest)
{

  if(bb->getCFG()->debug()){
    std::cout << "getret : dest=" << dest << std::endl;
  }
  
}


void GetRetInstr::gen_x86_asm(std::ostream &o) const {

  o << " \tmovl %eax, " << bb->getCFG()->symbol_to_asm(dest) << "\n";
  
}
