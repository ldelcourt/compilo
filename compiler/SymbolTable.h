#pragma once

#include <unordered_map>
#include <string>
#include <iostream>

typedef enum {

  INT,
  FLOAT
} Type;


class SymbolTable {

public:

  SymbolTable() : currentIndex(-4), numTemp(0) {}
  ~SymbolTable() {}


  void add (const std::string &symbol);
  void giveIndex (const std::string &symbol);

  bool contains (const std::string &symbol) const;

  int getValue (const std::string &symbol) const;

  void printTable() const;

  std::string createTemp();
  std::string createConst(int value);

private:

  std::unordered_map<std::string, int> table;
  std::unordered_map<std::string, Type> type; //a implémenter
  int currentIndex;

  int numTemp;

};


inline int SymbolTable::getValue(const std::string &symbol) const {

  return table.at(symbol);

}

inline bool SymbolTable::contains(const std::string &symbol) const {

  return table.count(symbol) > 0;

}


inline void SymbolTable::add (const std::string &symbol) {
  
  table[symbol] = 0;

}

inline void SymbolTable::giveIndex(const std::string &symbol) {

  if (table.count(symbol) > 0 && table[symbol] != 0) {
    return;
  }

  table[symbol] = currentIndex;
  currentIndex -= 4;
}
  

inline std::string SymbolTable::createTemp() {

  std::string name = "#tmp" + std::to_string(numTemp);
  numTemp++;

  giveIndex(name);

  return name;
  
}


inline std::string SymbolTable::createConst(int value) {

  std::string name = "#const" + std::to_string(numTemp);
  numTemp++;

  table[name] = value;

  return name;

}

inline void SymbolTable::printTable() const {

  std::cout << "Symbol table:" << table.size() << std::endl;
  for (auto const& x : table) {
    std::cout << x.first << " : " << x.second << std::endl;
  }

}

  
