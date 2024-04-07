#include "IR.h"
#include "IRVisitor.h"
#include "VarVisitor.h"

BasicBlock::BasicBlock(CFG *cfg, std::string entry_label)
    : cfg(cfg), label(entry_label) {}

BasicBlock::~BasicBlock() {

  for (IRInstr *instr : instrs) {

    delete instr;
  }
}

void BasicBlock::gen_asm(std::ostream &o) const {
  if (cfg->assemblyLangage == ARM) {
    gen_arm(o);
  } else if (cfg->assemblyLangage == x86) {
    gen_x86(o);
  }
}

void BasicBlock::gen_x86(std::ostream &o) const {

  o << label << ":\n";

  for (auto it = instrs.cbegin(); it != instrs.cend(); it++) {

    (*it)->gen_asm(o);
  }

  if (exit_true == nullptr) {

    cfg->gen_x86_epilogue(o);

  }

  else if (exit_false == nullptr) {
    o << " \tjmp " << exit_true->label << "\n";
  }
}

void BasicBlock::gen_arm(std::ostream &o) const {

  o << label << ":\n";

  for (auto it = instrs.cbegin(); it != instrs.cend(); it++) {

    (*it)->gen_asm(o);
  }

  if (exit_true == nullptr) {

    cfg->gen_arm_epilogue(o);

  }

  else if (exit_false == nullptr) {
    o << "\tb " << exit_true->label << "\n";
  }
}

void BasicBlock::addIRInstr(IRInstr::Operation o, Type t,
                            const std::string *params, int nb) {

  instrs.push_back(IRInstr::createInstr(this, o, t, params, nb));
}

CFG::CFG(DefFunction *ast, AssemblyLangage assemblyLangage, bool debug,
         bool symbol)
    : ast(ast), nextBBnumber(0), assemblyLangage(assemblyLangage), debug(debug),
      symbol(symbol) {

  // Mise en place de la table des symboles
  VarVisitor varVisitor(&table);
  varVisitor.visit(ast);
  varVisitor.checkUnusedDecla();
  if (symbol) {
    table.printTable();
  }

  if (varVisitor.hasError()) {
    throw 1;
  }

  // creation du squelette de block
  current_bb = addBasicBlock("principal");

  current_bb->exit_true = addBasicBlock(newBBName());
  current_bb = current_bb->exit_true;

  current_bb->exit_true = addBasicBlock("output");
  current_bb->exit_true->exit_true = nullptr;

  // buildIR()
  IRVisitor visitor(this);
  visitor.visit(ast);
}

CFG::~CFG() {

  for (BasicBlock *bb : bbs) {
    delete bb;
  }
}

BasicBlock *CFG::addBasicBlock(const std::string &name) {

  BasicBlock *b = new BasicBlock(this, name);
  bbs.push_back(b);

  return b;
}

void CFG::gen_asm(std::ostream &o) const {

  if (assemblyLangage == ARM) {
    o << ".globl _main\n";
    o << "_main: \n";
  } else if (assemblyLangage == x86) {
    o << ".globl _main\n";
    o << "_main: \n";
  }
  gen_asm_prologue(o);

  for (auto it = bbs.cbegin(); it != bbs.cend(); it++) {

    (*it)->gen_asm(o);
  }
}

std::string CFG::symbol_to_asm(const std::string &reg) {

  try {
    if (assemblyLangage == ARM) {
      if (reg.find("#const") != std::string::npos) {
        return "#" + std::to_string(table.getValue(reg));
      } else {
        return "[sp, #" + std::to_string(-table.getValue(reg)) + "]";
      }

    } else if (assemblyLangage == x86) {
      if (reg.find("#const") != std::string::npos) {
        return "$" + std::to_string(table.getValue(reg));
      } else {
        return std::to_string(table.getValue(reg)) + "(%rbp)";
      }
    }
  } catch (std::exception &e) {
  }

  if (reg == "#reg_ret") {
    switch (assemblyLangage) {
    case x86:
      return "%eax";
      break;
    case ARM:
      return "w0";
      break;
    }
  }

  else {
    return "";
  }
}

void CFG::gen_asm_prologue(std::ostream &o) const {
  switch (assemblyLangage) {
  case x86:
    gen_x86_prologue(o);
    break;
  case ARM:
    gen_arm_prologue(o);
    break;
  }
}
void CFG::gen_x86_prologue(std::ostream &o) const {

  o << " \tpushq %rbp\n"
    << " \tmovq %rsp, %rbp\n"; // x86

  // Reservation de la zone mémoire pour la fonction
  o << " \tsubq $" << table.getSizeMemory() << ", %rsp\n";
}

void CFG::gen_x86_epilogue(std::ostream &o) const {

  o << " \tmovq %rbp, %rsp\n"
    << " \tpopq %rbp\n"
    << " \tret\n"; // x86
}

void CFG::gen_arm_prologue(std::ostream &o) const {
  o << "\tsub sp, sp, #" << table.getSizeMemory() << "\n";
}

void CFG::gen_arm_epilogue(std::ostream &o) const {
  o << "\tadd sp, sp, #" << table.getSizeMemory() << "\n";
  o << "\tret\n";
}

std::string CFG::createTempVar() { return table.createTemp(); }

std::string CFG::createConstSymbol(int v) { return table.createConst(v); }

bool CFG::symbolIsConst(const std::string &symbol, int *value) const {

  if (symbol.find("#const") != std::string::npos) {
    if (value != nullptr) {
      *value = table.getValue(symbol);
    }
    return true;
  } else {
    return false;
  }
}

bool CFG::symbolIsRegRet(const std::string &symbol) const {
  if (symbol.find("#reg_ret") != std::string::npos) {
    return true;
  }
  return false;
}

std::string CFG::getRealVarname(const std::string &symbol) {

  std::string varname = symbol;
  size_t pos = 0;
  while (pos != std::string::npos) {
    if (table.contains(varname)) {
      return varname;
    }
    pos = varname.find_last_of(":");
    varname = varname.substr(0, pos);
  }

  return "err";
}

std::string CFG::newBBName() { return "bb" + std::to_string(nextBBnumber++); }
