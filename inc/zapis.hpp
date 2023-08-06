#include <stdio.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <regex>
#include <fstream>
#include <stdlib.h>

using namespace std;

class Zapis{
  public:
    string nazivSekcije;
    vector<string> elementi;
    Zapis(string naziv){
      nazivSekcije=naziv;
    }

};