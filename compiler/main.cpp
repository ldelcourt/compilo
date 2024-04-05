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


static bool debug = false;
static bool symbol = false;

int main(int argn, const char **argv)
{
  stringstream in;
  if (argn >= 2)
  {
     ifstream lecture(argv[1]);
     if( !lecture.good() )
     {
         cerr<<"error: cannot read file: " << argv[1] << endl ;
         exit(1);
     }
     in << lecture.rdbuf();
     if (argv[2] && string(argv[2]) == "--debug") {
       debug = true;
     }
     if (argv[2] && string(argv[2]) == "--symbol") {
       symbol = true;
     }

  }
  else
  {
      cerr << "usage: ifcc path/to/file.c [--debug] [--symbol]" << endl ;
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

 
  int nbFonctions = tree->children.size() - 2;

  CFG* *cfg = new CFG* [nbFonctions+1];

  
  try {

    int i;
    for (i = 0; i < nbFonctions; i++) {
      cfg[i] = new CFG(tree->children[i], debug, symbol);
      cfg[i]->gen_asm(cout);
    }

    cfg[nbFonctions] = new CFG(tree->children[i], debug, symbol);
    cfg[nbFonctions]->gen_asm(cout);

  } catch (int e) {
    return 1;
  }

  for (int i = 0; i < nbFonctions+1; i++) {
    delete cfg[i];
  }
  delete [] cfg;
  
  

  return 0;
}
