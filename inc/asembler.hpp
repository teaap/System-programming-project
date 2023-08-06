#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <regex>
#include <fstream>
#include <stdlib.h>
#include "tabelaunapred.hpp"
#include "token.hpp"
#include "tabelasimbola.hpp"
#include "zapis.hpp"
#include "relaTabela.hpp"

using namespace std;

class Asembler{
  protected:
    string ulazniFajl;
    string izlazniFajl;
    string linija;
    string trenutnaSekcija="";
    uint16_t locationCounter=0;
    int brSimbola=0;
    int ind=0;
    vector<string> izaLabele;
    vector<Simbol> Simboli;
    vector<Zapis> zapisi;
    vector<string> simboliuLiniji;
    vector<Rela> relokacioni;
    vector<TabelaUnapred> unapredtab; 
  public:
  
  Asembler(string ulaznif,string izlaznif);
  vector<string> tokeni(string linija);

  void ispis();
  int hexudec(string hexp);
  string decuHex(int localCounter);
  Simbol* pronadjiSimbol(string name);
  bool skok(string linija);
  bool jednobajtne(string linija);
  bool dvobajtne(string linija);
  bool literal(string linija);
  bool section(string linija);
  bool global(string linija);
  bool externi(string linija);
  bool word(string linija);
  bool skip(string linija);
  bool end(string linija);
  bool labela(string linija);
  bool call(string linija);
  bool notm(string linija);
  bool ldst(string linija);
  bool intm(string linija);
  bool pushpop(string linija);
  void brisanjeKom(string &linija);
  void prviProlaz();
  void backpatch();
  string operandizaucitavanje(string linija);
  string operandizaskokove(string linija);
  int postoji(string s);

};