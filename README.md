# C compiler with antlr4/c++
------
## Compiler part
### Instructions

This project is a basic version of a c compiler done using antlr4 and C++
It supports basic features of the C language and is incrementable.
This compiler translates C code to x86 Assembly code and then needs gcc to compile into binary

### Source Files
- `ifcc.g4` contains the grammar in antlr4 format
- `VarVisitor.h` contains the code to generate a symbolTable to check var declaration, initialisation and utilisation
- `IR.Visitor.h` creates the Intermediate Representation (IR) tree to allow for easier translation to assembly code
- `IRInstr.h` generate the actual assembly code from IR
- `IR.h` contains the classes used for the IR (Control Flow Graph, Basic Blocks and Instruction)

### Prerequisites
  Before building your  compiler, you should install  antlr using your
  distribution's  package manager,  or by  running the  provided shell
  script:   `install-antlr.sh`. 
    
### Compilation scripts
- `Makefile` contains  the actual build  logic. Please read  this file
  and ask questions on parts you do not understand.
  The  Makefile includes a .mk file that defines several variables 
  (ANTLR, ANTLRJAR,  ANTLRINC and  ANTLRLIB) indicating the location 
  of various parts of the Antlr suite. 
  You can (should)  change  those values to suit your installation, either
  by editing the  Makefile and/or adding another .mk file.

### Build and use the project
- To build the project, just run the command `make` in your terminal while in the compiler directory
- This creates a ifcc target used as follow `./ifcc path/to/file [--debug | --symbol]`
  - The `--debug` option shows the IR before the assembly code
  - The `--symbol` allows you to view the whole symbol table

---
## Test part
### Test explanation
Tests are run using a Python script with compiles the different testfiles with both our compiler and gcc
It then compares the compiling output of both programm. After that both assembly codes are converted to binary to test the execution output

### Run tests
In order to run the tests, ensure you have `Python 3` installed
Then go to the `tests` directory and run `python3 ifcc-test.py testfiles`
Some tests require user input.