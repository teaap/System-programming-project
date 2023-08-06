#include <stdio.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <regex>
#include <fstream>

using namespace std;

class Rela{
  public:
    int index;
    string sekcija;
    string nameSimbol;
    string offset;
    string type;
    int symbol;
    string addend;
  public:
    Rela(){}
    Rela(int ind,string sekcija,string nameSimbol,string of,string t,int s,string a){
      index=ind;
      this->sekcija=sekcija;
      this->nameSimbol=nameSimbol;
      this->offset=of;
      this->type=t;
      this->symbol=s;
      this->addend=a;
    }
};