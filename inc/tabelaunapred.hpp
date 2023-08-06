#include <stdio.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <regex>
#include <fstream>
#pragma once

using namespace std;

class Unapred{
  public:
    int ulazRela;
    int brSekcije=-1;
    int loccnt=-1;

    Unapred(int ulazRela){
      this->ulazRela=ulazRela;
    }
};
class TabelaUnapred{
  public:
    string imeSimbola;
    vector<Unapred> ulazi;
    TabelaUnapred(string imesim){
      imeSimbola=imesim;
    }

};