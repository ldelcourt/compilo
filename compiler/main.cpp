#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#include "antlr4-runtime.h"
#include "generated/ifccLexer.h"
#include "generated/ifccParser.h"

#include "FunctionVisitor.h"

using namespace antlr4;
using namespace std;

static bool debug = false;
static bool symbol = false;
static AssemblyLangage assemblyLanguage = x86;

int main(int argn, const char **argv) {

  stringstream in;

  const char *configAssembly = getenv("CONFIG_ASSEMBLY");
  string config = configAssembly != nullptr ? configAssembly : "";
  if (config != "") {
    if (config == "ARM") {
      assemblyLanguage = ARM;
    } else if (config == "x86") {
      assemblyLanguage = x86;
    } else {
      cout << "Invalid configuration: " << configAssembly << endl;
      cout << "Please run the command 'export CONFIG_ASSEMBLY=assembly'"
           << endl;
      cout << "With assembly being ARM or x86 depending on your OS" << endl;
      return 1;
    }
  } else {
    cout << "Invalid configuration" << endl;
    cout << "Please run the command 'export CONFIG_ASSEMBLY=assembly'" << endl;
    cout << "With assembly being ARM or x86 depending on your OS" << endl;
    return 1;
  }

  if (argn >= 2) {
    ifstream lecture(argv[1]);
    if (!lecture.good()) {
      cerr << "error: cannot read file: " << argv[1] << endl;
      exit(1);
    }
    in << lecture.rdbuf();
    for (int i = 2; i < argn; i++) {
      if (string(argv[i]) == "--debug") {
        debug = true;
      }
      if (string(argv[i]) == "--symbol") {
        symbol = true;
      }
      if (string(argv[i]) == "--x86") {
        assemblyLanguage = x86;
      }
      if (string(argv[i]) == "--ARM") {
        assemblyLanguage = ARM;
      }
    }

  } else {
    cerr << "usage: ifcc path/to/file.c [--debug] [--symbol] [--x86|--ARM]"
         << endl;
    exit(1);
  }

  ANTLRInputStream input(in.str());

  // lexer+parser
  ifccLexer lexer(&input);
  CommonTokenStream tokens(&lexer);

  tokens.fill();

  ifccParser parser(&tokens);
  // Arbre AST
  tree::ParseTree *tree = parser.axiom();

  if (parser.getNumberOfSyntaxErrors() != 0) {
    cerr << "error: syntax error during parsing" << endl;
    exit(1);
  }

  // Visiteur des fonctions et création des CFG
  FunctionVisitor fv(assemblyLanguage, debug, symbol);
  fv.visit(tree);

  if (fv.hasError()) {
    return 1;
  }

  // Generation asm x86
  fv.gen_asm(cout);

  return 0;
}
