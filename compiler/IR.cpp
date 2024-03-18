#include "IR.h"
#include "IRVisitor.h"
#include "VarVisitor.h"




BasicBlock::BasicBlock(CFG *cfg, std::string entry_label) :
  cfg(cfg), label(entry_label)
{

}

BasicBlock::~BasicBlock() {

  for (IRInstr* instr : instrs) {

    delete instr;

  }

}

void BasicBlock::gen_asm(std::ostream &o) const {


  for (auto it = instrs.cbegin(); it != instrs.cend(); it++) {

    (*it)->gen_asm(o);

  }

  if (exit_true == nullptr) {

    cfg->gen_asm_epilogue(o);

  }

  else if (exit_false == nullptr) {
    //A faire
  }
  else {

    //a faire
  }

}

void BasicBlock::addIRInstr(IRInstr::Operation o, Type t, std::string *params ) {

  instrs.push_back( IRInstr::createInstr(this, o, t, params) );

}

      
      


CFG::CFG(DefFunction *ast, bool debug) : ast(ast),  nextBBnumber(0), debug(debug)
{


  //Mise en place de la table des symboles
  VarVisitor varVisitor(&table);
  varVisitor.visit(ast);
  varVisitor.checkUnusedDecla();
  
  if (varVisitor.hasError()) {
    throw 1;
  }
  

  //creation du squelette de block
  current_bb  = addBasicBlock("main");

  current_bb->exit_true = addBasicBlock(newBBName());
  current_bb = current_bb->exit_true;

  current_bb->exit_true = addBasicBlock("output");
  current_bb->exit_true->exit_true = nullptr;

  //buildIR()
  IRVisitor visitor(this);
  visitor.visit(ast);
  
}

CFG::~CFG()
{

  for (BasicBlock *bb : bbs) {
    delete bb;
  }
  
}

BasicBlock* CFG::addBasicBlock(const std::string &name) {

  BasicBlock *b = new BasicBlock(this, name);
  bbs.push_back(b);

  return b;

}
  

void CFG::gen_asm(std::ostream &o) const {

  o<< ".globl main\n" ;
  o<< " main: \n" ;

  gen_asm_prologue(o);
  
  for (auto it = bbs.cbegin(); it != bbs.cend(); it++) {

    (*it)->gen_asm(o);

  }

}

std::string CFG::symbol_to_asm(const std::string &reg) {

  try {

    if (reg.find("#const") != std::string::npos) {
      return "$" + std::to_string(table.getValue(reg));
    }
    else {
      return std::to_string(table.getValue(reg)) + "(%rbp)";
    }

  } catch (std::exception &e) {}

  if (reg == "#reg_ret") {
    return "%eax";
  }

  else {
    return "";
  }

}

  

void CFG::gen_asm_prologue(std::ostream &o) const {

  o << " \tpushq %rbp\n" << " \tmovq %rsp, %rbp\n"; //x86
  
}

void CFG::gen_asm_epilogue(std::ostream& o) const {

  o << " \tpopq %rbp\n" << " \tret\n"; //x86
  
}



std::string CFG::createTempVar() {
  return table.createTemp();
}

std::string CFG::createConstSymbol(int v) {

  return table.createConst(v);

}

bool CFG::symbolIsConst(const std::string &symbol, int *value) const {

  if (symbol.find("#const") != std::string::npos) {
    if (value != nullptr) {
      *value = table.getValue(symbol);
    }
    return true;
  }
  else {
    return false;
  }
  

}

std::string CFG::newBBName() {

  return "bb" + std::to_string(nextBBnumber++);

}
