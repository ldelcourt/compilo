#pragma once

#include <unordered_map>
#include <string>
#include <iostream>


typedef enum {

  INT,
  FLOAT,
  VOID
} Type; /**<Typage possible**/


/**
 * @brief Table de symboles
 * @details
 *  Un symbole represente un espace sur la pile
 *  Ce peut-être soit une variable
 *  soit une variable temporaire (pour stocker des résultats intermédiaires de calcul par exemple
 *  soit une constante (exception: pas stocker sur la pile)
 **/
class SymbolTable {

public:

  SymbolTable() : currentIndex(-4), numTemp(0) {} /**<Constructeur**/
  ~SymbolTable() {} /**<Destructeur**/


  /**
   * @brief Ajoute un symbole dans la table sans lui affecter une zone mémoire sur la pile
   * @param symbol nom du symbole à ajouter
   **/
  void add (const std::string &symbol);

  /**
   * @brief Ajoute et donne un index (=affecte une zone mémoire) à un symbole si ce n'est pas encore le cas
   * @param symbol le nom du symbol à affecter
   **/
  void giveIndex (const std::string &symbol);
  
  bool contains (const std::string &symbol) const; /**<true si la table contient le symbol**/

  /**
   * @brief Renvoie la valeur associé au symbol
   * @details
   *  Cette valeur peut être un index de pile ou la valeur d'une constante
   *
   * @param symbol le nom du symbol dont on veut la valeur
   *
   * @throw une exception si le symbole ne fait pas partie de la table
   *
   * @return int
   **/
  int getValue (const std::string &symbol) const;

  
  void printTable() const; /**<Affiche tous les éléments de la table**/

  /**
   * @brief Créer une variable temporaire de la forme #tmp + un nombre et lui affecte un index de zone mémoire
   * @return le nom de cette nouvelle variable
   **/
  std::string createTemp();

  /**
   * @brief Créer une constante de la forme #const + un nombre et lui affecte une valeur
   * @param[in] value la valeur de la constante
   * @return le nom de cette nouvelle variable
   **/
  std::string createConst(int value);

  int getSizeMemory() const; /**<Retourne un multiple de 16 correspondant à la taille occupé en mémoire par l'ensemble des symboles**/

private:

  std::unordered_map<std::string, int> table; /**<la table**/
  //std::unordered_map<std::string, Type> type; //a implémenter
  int currentIndex; /**<le dernier index libre**/

  int numTemp; /**<le dernier numéro libre de symbole factice**/

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

inline void SymbolTable::printTable() const {

  std::cout << "Symbol table:" << table.size() << std::endl;
  for (auto const& x : table) {
    std::cout << x.first << " : " << x.second << std::endl;
  }

}

  
