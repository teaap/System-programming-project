#include <stdio.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <regex>
#include <fstream>

using namespace std;

class Sekcija{
  public:
    int num;
    string naziv;
    string duzina;
    string adresa;

  public:
    Sekcija(string n){
      this->naziv=n;
    }
};