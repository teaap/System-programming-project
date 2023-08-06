#include <stdio.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <regex>
#include <fstream>
#include "tabelaunapred.hpp"

using namespace std;

class Simbol{
  public:
    int num;
    string value;
    int size;
    string type;
    string bind;
    string ndx;
    string name;
    bool defined;
    Unapred* flink;
  public:
    Simbol(){}
    void inic(int num,string value,int size,string type,string bind,string ndx,string name,bool defined,Unapred* flink){
      this->num=num;
      this->value=value;
      this->size=size;
      this->type=type;
      this->bind=bind;
      this->ndx=ndx;
      this->name=name;
      this->defined=defined;  
      this->flink=flink;
    }
};