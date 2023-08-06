#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <regex>
#include <fstream>
#include <stdlib.h>

#include "../inc/tabelasimbola.hpp"
#include "../inc/sekcija.hpp"
#include "../inc/pomocna.hpp"
#include "../inc/hex.hpp"

using namespace std;

class Linker{
  public:
  uint16_t loccnt=0;
  string izlazniFajl;
  string trenutniulf;
  string trenutnaSekcija;
  int brsek=0;
  int brSimbola=0;
  vector<Sadrzaj> izlaz;
  vector<string> sviulaznif;
  vector<Simbol> velikatsimb;
  vector<Sekcija> sekcije;
  vector<Pomocna> pomocne;
    Linker(string izlaznif);
    void dodaj(string ulazni);
    void mapiranje(string ulazni);
    void prepravkaTs();
    void razresavanje();
    int postojiSekcija(string naziv);
    int postojiPomocna(string naziv);
    int postojiSimbol(string naziv);
    void brisanjeK(string &linija);
    vector<string> relazapisi;
    vector<string> tokeni(string linija);
    string decuHex(int localCounter);
    int hexudec(string hexp);
    void ispis();
};