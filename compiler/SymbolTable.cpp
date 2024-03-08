#include "SymbolTable.h"

int SymbolTable::addSymbol(string name) {
  if(symbolTable.find(name) != symbolTable.end()) {
    // Si le symbole existe déjà, on retourne -1
    cout << "Error: var " << name << " is declared twice" << endl;
    return -1;
  }

  symbolTable[name] = 0;
}

string SymbolTable::autoAddSymbol() {
  string name = "#autoVar" + to_string(currentIndex);
  symbolTable[name] = currentIndex;
  currentIndex += -4;
  return name;
}

int SymbolTable::isUsed(string name) {
  if(checkDeclaration(name) == -1) return -1;
  if (symbolTable[name] == 0) {
    symbolTable[name] = currentIndex;
    currentIndex += -4;
  }
  return 0;
}

int SymbolTable::checkDeclaration(string name) {
  if(symbolTable.find(name) == symbolTable.end()) {
    cout << "Error: var " << name << " is used but not declared" << endl;
    return -1;
  }
  return 0;
}

int SymbolTable::checkUsage() {
  int retVal = 0;
  for (auto const& x : symbolTable) {
    if (x.second == 0) {
      cout << "Warning: var " << x.first << " is declared but never used" << endl;
      retVal = -1;
    }
  }
  return retVal;
}

void SymbolTable::printTable() {
  cout << "Symbol table:" << symbolTable.size() << endl;
  for (auto const& x : symbolTable) {
    cout << x.first << " : " << x.second << endl;
  }
}