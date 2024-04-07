#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#include "antlr4-runtime.h"
#include "generated/ifccLexer.h"
#include "generated/ifccParser.h"

#include "IR.h"

using namespace antlr4;
using namespace std;

static bool debug = false;
static bool symbol = false;
static AssemblyLangage assemblyLanguage = x86;

int main(int argn, const char **argv) {
  stringstream in;
  if (argn >= 2) {
    ifstream lecture(argv[1]);
    if (!lecture.good()) {
      cerr << "error: cannot read file: " << argv[1] << endl;
      exit(1);
    }
    in << lecture.rdbuf();
    if (argv[2] && string(argv[2]) == "--debug") {
      debug = true;
    }
    if (argv[2] && string(argv[2]) == "--symbol") {
      symbol = true;
    }
    if (argv[2] && string(argv[2]) == "--x86") {
      assemblyLanguage = x86;
    }
    if (argv[2] && string(argv[2]) == "--ARM") {
      assemblyLanguage = ARM;
    }

  } else {
    cerr << "usage: ifcc path/to/file.c" << endl;
    exit(1);
  }

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
  ANTLRInputStream input(in.str());

  ifccLexer lexer(&input);
  CommonTokenStream tokens(&lexer);

  tokens.fill();

  ifccParser parser(&tokens);
  tree::ParseTree *tree = parser.axiom();

  if (parser.getNumberOfSyntaxErrors() != 0) {
    cerr << "error: syntax error during parsing" << endl;
    exit(1);
  }

  try {

    CFG cfg(tree, assemblyLanguage, debug, symbol);
    cfg.gen_asm(cout);

  } catch (int e) {
    return 1;
  }

  return 0;
}
