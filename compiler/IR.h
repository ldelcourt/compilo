#ifndef IR_H
#define IR_H

#include <vector>
#include <stack>
#include <string>
#include <iostream>
#include <initializer_list>

// Declarations from the parser -- replace with your own
#include "antlr4-runtime.h"
#include "SymbolTable.h"

#include "IRInstr.h"


typedef antlr4::tree::ParseTree DefFunction;

class CFG;


/**  The class for a basic block */

/* A few important comments.
   IRInstr has no jump instructions.
   cmp_* instructions behaves as an arithmetic two-operand instruction (add or mult),
   returning a boolean value (as an int)

   Assembly jumps are generated as follows:
   BasicBlock::gen_asm() first calls IRInstr::gen_asm() on all its instructions, and then 
   if  exit_true  is a  nullptr, 
   the epilogue is generated
   else if exit_false is a nullptr, 
   an unconditional jmp to the exit_true branch is generated
   else (we have two successors, hence a branch)
   an instruction comparing the value of test_var_name to true is generated,
   followed by a conditional branch to the exit_false branch,
   followed by an unconditional branch to the exit_true branch
   The attribute test_var_name itself is defined when converting 
   the if, while, etc of the AST  to IR.

   Possible optimization:
   a cmp_* comparison instructions, if it is the last instruction of its block, 
   generates an actual assembly comparison 
   followed by a conditional jump to the exit_false branch
*/

class BasicBlock {
public:
  BasicBlock(CFG* cfg, std::string entry_label);
  ~BasicBlock();
  
  
  void gen_asm(std::ostream &o) const; /**< x86 assembly code generation for this basic block (very simple) */

  void addIRInstr(IRInstr::Operation op, Type t, std::string *params);


  // No encapsulation whatsoever here. Feel free to do better.
  BasicBlock* exit_true;  /**< pointer to the next basic block, true branch. If nullptr, return from procedure */ 
  BasicBlock* exit_false; /**< pointer to the next basic block, false branch. If null_ptr, the basic block ends with an unconditional jump */
  
  
  std::string test_var_name;  /** < when generating IR code for an if(expr) or while(expr) etc,
		     store here the name of the variable that holds the value of expr */


  CFG* cfg; /** < the CFG where this block belongs */
  
private:
  std::string label; /**< label of the BB, also will be the label in the generated code */
  
  std::vector<IRInstr*> instrs; /** < the instructions themselves. */
  

 
};




/** The class for the control flow graph, also includes the symbol table */

/* A few important comments:
   The entry block is the one with the same label as the AST function name.
   (it could be the first of bbs, or it could be defined by an attribute value)
   The exit block is the one with both exit pointers equal to nullptr.
   (again it could be identified in a more explicit way)

*/
class CFG {
public:
  
  CFG(DefFunction *ast);
  ~CFG();
	
  BasicBlock* addBasicBlock(const std::string &name);

  // x86 code generation: could be encapsulated in a processor class in a retargetable compiler
  void gen_asm(std::ostream& o) const;
  
  std::string symbol_to_asm(const std::string &reg); /**< helper method: inputs a IR reg or input variable, returns e.g. "-24(%rbp)" for the proper value of 24 */

  void gen_asm_prologue(std::ostream& o) const;
  void gen_asm_epilogue(std::ostream& o) const;

  

  //symbol table management
  std::string createTempVar();
  std::string createConstSymbol(int v);
  bool symbolIsConst(const std::string &symbol, int *value = nullptr) const;
  


  // basic block management
  std::string newBBName();
  BasicBlock* current_bb;


  
private:
  
  int nextBBnumber; /**< just for naming */
	
  std::vector <BasicBlock*> bbs; /**< all the basic blocks of this CFG*/
  
  DefFunction *ast;

  SymbolTable table;

};


#endif
