#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

#include "antlr4-runtime.h"
#include "generated/ifccLexer.h"
#include "generated/ifccParser.h"
#include "generated/ifccBaseVisitor.h"


#include "VarVisitor.h"
#include "IRVisitor.h"
#include "IR.h"

using namespace antlr4;
using namespace std;




int main(int argn, const char **argv)
{
  stringstream in;
  if (argn==2)
  {
     ifstream lecture(argv[1]);
     if( !lecture.good() )
     {
         cerr<<"error: cannot read file: " << argv[1] << endl ;
         exit(1);
     }
     in << lecture.rdbuf();
  }
  else
  {
      cerr << "usage: ifcc path/to/file.c" << endl ;
      exit(1);
  }
  
  ANTLRInputStream input(in.str());

  ifccLexer lexer(&input);
  CommonTokenStream tokens(&lexer);

  tokens.fill();

  ifccParser parser(&tokens);
  tree::ParseTree* tree = parser.axiom();

  if(parser.getNumberOfSyntaxErrors() != 0)
  {
      cerr << "error: syntax error during parsing" << endl;
      exit(1);
  }

  /*
  SymbolTable table;

  VarVisitor varVisitor(&table);
  varVisitor.visit(tree);
  varVisitor.checkUnusedDecla();
  
  if (varVisitor.hasError()) {
    return 1;
  }
  */

  try {
    
    CFG cfg(tree);
    cfg.gen_asm(cout);

  } catch (int e) {
    return 1;
  }

  return 0;
}
