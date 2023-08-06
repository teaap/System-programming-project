#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <regex>
#include <fstream>
#include <stdlib.h>

using namespace std;

class Memorija{
  public:
    string adresa;
    string sadrzaj;
    Memorija(string adr,string sadrzaj){
      this->adresa=adr;
      this->sadrzaj=sadrzaj;
    }
};

class Emulator{
  public:
    string ulazni;
    string kod;
    vector<Memorija> memorija;
    vector<Memorija> ivt;
    int psw[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    string r[8]={"0000","0000","0000","0000","0000","0000","0000","0000"};
    bool tajmer,terminal;
    bool umanjiza2=false,uvecajza2=false;
    int ind;
    int d;
    void citanjeizfajla();
    void izvrsi();
    void ispis();
    void stringUPSW(string s);
    void upisiumem(string adr,string sad);
    void stavinastek(string bajt);
    void azurirajPSW(int levi,int desni,int broj);
    void azurirajReg(int indeks,int upmod);
    string operandpc();
    string operandpcB0();
    string skinisasteka();
    string pswUString();
    string sadrzaj2stek(string adresa);
    string sadrzaj2bajta(string adresa);
    string sadrzajbajt(string adresa);
    Emulator(string izlazni){
      this->ulazni=izlazni;
      citanjeizfajla();
      izvrsi();
      ispis();
    }  
    vector<string> tokeni(string linija);
    string decuHex(int locationCounter);
    string prosirido4(string rec);
};

