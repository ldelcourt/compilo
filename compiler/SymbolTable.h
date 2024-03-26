#pragma once

#include <unordered_map>
#include <string>

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

  std::string createTemp();
  std::string createConst(int value);

  int getSizeMemory() const;

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

inline int SymbolTable::getSizeMemory() const {

  int size = -currentIndex;
  if (size % 16 == 0) {
    return size;
  }

  else {
    size = size/16;
    size = 16*(size+1);
    return size;
  }
    
  
}

  
