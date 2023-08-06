#include <stdio.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <regex>
#include <fstream>

using namespace std;

class Token{
  protected:
    string naziv;
    bool sekcija;
    bool labela;
  public:
    Token(string ime){
      this->naziv=ime;
    }

};