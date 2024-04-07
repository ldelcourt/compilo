#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

#include "antlr4-runtime.h"
#include "generated/ifccLexer.h"
#include "generated/ifccParser.h"


#include "FunctionVisitor.h"

using namespace antlr4;
using namespace std;


static bool debug = false;
static bool symbol = false;

int main(int argn, const char **argv)
{

  //Recup des paramètres en ligne de commande
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


  //lexer + parser
  ANTLRInputStream input(in.str());

  ifccLexer lexer(&input);
  CommonTokenStream tokens(&lexer);

  tokens.fill();

  ifccParser parser(&tokens);
  //Arbre AST
  tree::ParseTree* tree = parser.axiom();
  
  if(parser.getNumberOfSyntaxErrors() != 0)
  {
      cerr << "error: syntax error during parsing" << endl;
      exit(1);
  }
  

  //Visiteur des fonctions et création des CFG
  FunctionVisitor fv (debug, symbol);
  fv.visit(tree);

  if (fv.hasError()) {
    return 1;
  }

  //Generation asm x86
  fv.gen_asm(cout);
  
  

  return 0;
}
