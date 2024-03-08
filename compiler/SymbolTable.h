#pragma once

#include <map>
#include <string>
#include <iostream>

using namespace std;

// Cette classe sert à stocker les noms de variables déclarées dans le programme et leur adresse mémoire
class SymbolTable {
public:
    map<string, int> symbolTable;
    SymbolTable() { symbolTable = map<string, int>(); }
    int addSymbol(string name);
    string autoAddSymbol();
    int isUsed(string name);
    int checkDeclaration(string name);
    int checkUsage();
    void printTable();
    int getSymbolIndex(string name) { return symbolTable[name]; }

private:
    int currentIndex = -4;
};

