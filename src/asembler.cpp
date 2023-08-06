#include "../inc/asembler.hpp"

Asembler::Asembler(string inputFile,string outputFile){
  this->ulazniFajl=inputFile;
  this->izlazniFajl=outputFile;
  int kk=0;
  ifstream in(ulazniFajl);
  ofstream out(izlazniFajl);
  Simbol sim= Simbol();
  sim.inic(0,"0",0,"","","","",false,nullptr);
  brSimbola++;
  Simboli.push_back(sim);
  while(true){
    vector<string> token;
    if(in.eof()) break;
    if(izaLabele.empty()){
      getline(in,linija);
      brisanjeKom(linija);
      token=tokeni(linija); 
    }
    else{
      token=izaLabele;
      izaLabele.clear();
      linija="";
      for(int i=0;i<token.size()-1;i++)
      {
        linija+=token[i];
        linija+=" ";
      }
      linija+=token[token.size()-1];
    }
    Simbol s= Simbol();
    //.global
    if(global(linija)){
      for(int i=1;i<token.size();i++){
        int post=postoji(token[i]);
        if(post==(int)(-1))
        {
           int br=brSimbola++;
           s.inic(br,"0",0,"NOTYP","GLOB","UND",token[i],false,nullptr);
           Simboli.push_back(s);
        }
        else{
          Simboli[post].bind="GLOB";
          for(int j=0;j<relokacioni.size();j++){  
            if(relokacioni[j].nameSimbol==token[i]){
              relokacioni[j].addend="0";
              relokacioni[j].symbol=Simboli[post].num;
            }
          }
        }
      }
    }
    //.extern
    else if(externi(linija)){
      for(int i=1;i<token.size();i++){
        int br=brSimbola++;
        if(trenutnaSekcija=="" && postoji(token[i])==(int)(-1)){
          s.inic(br,"0",0,"NOTYP","GLOB","UND",token[i],false,nullptr);
          Simboli.push_back(s);
        }
      }
    }
    //.section
    else if(section(linija)){
      if(int k=postoji(token[1])!=(int)(-1) && Simboli[k].type=="SCTN"){
        locationCounter=Simboli[k].size;     
      }
      else{
        Simbol s=Simbol();
        if(trenutnaSekcija!=""){
          for(int i=0;i<Simboli.size();i++){
            if(Simboli[i].name==trenutnaSekcija){
              Simboli[i].size=locationCounter;
              locationCounter=0;
              break;
            }
          }
        }
        trenutnaSekcija=token[1];
        s.inic(brSimbola,"0",locationCounter,"SCTN","LOC",trenutnaSekcija,trenutnaSekcija,true,nullptr);
        zapisi.push_back(Zapis(s.name));
        Simboli.push_back(s);
        brSimbola++;
      }
      trenutnaSekcija=token[1];
    }
    //word
    else if(word(linija)){
      for(int i=1;i<token.size();i++){
        if(literal(token[i])){
          if(regex_match(token[i], regex("((0x([0-9]|[a-f]|[A-F])+))$"))){
            for(int j=0;j<zapisi.size();j++){
              if(zapisi[j].nazivSekcije==trenutnaSekcija){
                token[i]=token[i].substr(2,token[i].size()-2);
                while(token[i].size()%4!=0){string dodaj="0"; dodaj+=token[i]; token[i]=dodaj;}
                if(token[i].length()/2>2)
                locationCounter+=(token[i].length()/2);
                else locationCounter+=2;
                  while(token[i].size()>0){
                    string unos="";
                    unos=unos+token[i][token[i].size()-2];
                    unos=unos+token[i][token[i].size()-1];
                    token[i]=token[i].substr(0,token[i].size()-2);
                    zapisi[j].elementi.push_back(unos);
                  }  
              }
            }
            }
            else{
              for(int j=0;j<zapisi.size();j++){
                if(zapisi[j].nazivSekcije==trenutnaSekcija){
                    long dec=stoi(token[i]);
                    string loc=decuHex(dec);
                    while(loc.size()%4!=0){string dodaj="0"; dodaj+=loc; loc=dodaj;}
                    if(loc.length()/2>2)
                    locationCounter+=(loc.length()/2);
                    else locationCounter+=2;
                    while(loc.size()>0){
                      string unos="";
                      unos=unos+loc[loc.size()-2];
                      unos=unos+loc[loc.size()-1];
                      loc=loc.substr(0,loc.size()-2);
                      zapisi[j].elementi.push_back(unos);
                   }  
                }
              }
            }
      }
      else{
        int k=-1;
        if(k=postoji(token[i])!=(int)(-1)){
          k=postoji(token[i]);
          Rela r=Rela();int j=0;
          string loc=decuHex(locationCounter);
          if(Simboli[k].bind=="GLOB"){             
            r=Rela(ind++,trenutnaSekcija,token[i],loc,"R_X86_PLT16",Simboli[k].num,"0");
          }  
          else if(Simboli[k].bind=="LOC"){
            if(Simboli[k].defined==true){
              r=Rela(ind++,trenutnaSekcija,token[i],loc,"R_X86_PLT16",postoji(trenutnaSekcija),Simboli[k].value);
            }
            else{
              r=Rela(ind,trenutnaSekcija,token[i],loc,"R_X86_PLT16",postoji(trenutnaSekcija),Simboli[k].value);
              int j=0;
              for(j=0;j<unapredtab.size();j++){
                if(unapredtab[j].imeSimbola==token[i]){
                  unapredtab[j].ulazi.push_back(Unapred(ind++));break;
                }
              }
              if(j==unapredtab.size()){
                unapredtab.push_back(TabelaUnapred(token[i]));
                unapredtab[unapredtab.size()-1].ulazi.push_back(Unapred(ind++));
              }
            }
          }
          relokacioni.push_back(r);
        }
        else{
          Simbol s=Simbol();
          s.inic(brSimbola++,decuHex(locationCounter),0,"NOTYP","LOC",to_string(postoji(trenutnaSekcija)),token[i],false,nullptr);
          Simboli.push_back(s);
          Rela r=Rela();
          string loc=decuHex(locationCounter);
          r=Rela(ind,trenutnaSekcija,token[i],loc,"R_X86_PLT16",postoji(trenutnaSekcija),Simboli[k].value);
          int j=0;
          for(j=0;j<unapredtab.size();j++){
            if(unapredtab[j].imeSimbola==token[i]){
              unapredtab[j].ulazi.push_back(Unapred(ind++));break;
            }
          }
          if(j==unapredtab.size()){
            unapredtab.push_back(TabelaUnapred(token[i]));
            unapredtab[unapredtab.size()-1].ulazi.push_back(Unapred(ind++));
          }
          relokacioni.push_back(r);
        }
      locationCounter+=2;
      for(int i=0;i<zapisi.size();i++){
        if(zapisi[i].nazivSekcije==trenutnaSekcija){
          zapisi[i].elementi.push_back("00");
          zapisi[i].elementi.push_back("00");
        }
      }
      }
    }
  }
  //skip
  else if(skip(linija)){
    int povecaj=0;
    if(regex_match(linija, regex("^\\.skip +((0x([0-9]|[a-f]|[A-F])+))$"))){
      string hexbr=token[1].substr(2,token[1].size());
      povecaj=hexudec(hexbr);
    }
    else if(regex_match(linija,regex("^\\.skip +(([0-9])+)$"))){
      povecaj=stoi(token[1]);
    }
    locationCounter+=povecaj;
    for(int i=0;i<zapisi.size();i++){
      if(zapisi[i].nazivSekcije==trenutnaSekcija){
        while(povecaj){
          zapisi[i].elementi.push_back("00");
          povecaj--;
        }
      }
    }
    
  }
  //labela
  //backpatch
  else if(labela(linija)){
    int i=0;
    token[0]=token[0].substr(0,token[0].size()-1);
    brisanjeKom(token[0]);
    string loc=decuHex(locationCounter);  
    for(i=0;i<Simboli.size();i++){
     if(token[0]==Simboli[i].name){
      if(loc=="") loc="0";
        Simboli[i].value=loc;
        Simboli[i].defined=true;  
        Simboli[i].ndx=trenutnaSekcija; 
        for(int j=0;j<unapredtab.size();j++){
          if(unapredtab[j].imeSimbola==token[0]){
            for(int t=0;t<unapredtab[j].ulazi.size();t++){
              int indmoj;
              for(int o=0;o<relokacioni.size();o++) 
                if(relokacioni[o].index==unapredtab[j].ulazi[t].ulazRela) indmoj=o;
              if(Simboli[i].bind=="GLOB"){
                if(unapredtab[j].ulazi[t].brSekcije!=-1){
                  if(unapredtab[j].ulazi[t].brSekcije==postoji(trenutnaSekcija)){
                    if(relokacioni.size()>0)
                    {
                      if(unapredtab[j].ulazi[t].loccnt!=-1){
                        for(int p=0;p<zapisi.size();p++){
                          if(zapisi[p].nazivSekcije==trenutnaSekcija){
                            string pmc=to_string(locationCounter-unapredtab[j].ulazi[t].loccnt);
                            int cou=0,crr=0;
                            while(pmc.size()<4) {string pmc1="0"; pmc=pmc1+pmc;}
                            while(cou<3){
                              zapisi[p].elementi[unapredtab[j].ulazi[t].loccnt+crr]=pmc.substr(cou,2);
                              cou+=2;
                              crr++;
                            }
                          }
                        }
                      }
                      vector<Rela>::iterator itb=relokacioni.begin();
                      relokacioni.erase(itb+indmoj);
                      if(relokacioni.size()==0) relokacioni.clear();
                    }
                  }
                  else{
                    relokacioni[indmoj].addend="FFFE";
                    relokacioni[indmoj].symbol=Simboli[i].num;
                  }
                }
                else{
                  relokacioni[indmoj].addend="0";
                  relokacioni[indmoj].symbol=Simboli[i].num;
                  }
              }
              else{
                if(unapredtab[j].ulazi[t].brSekcije!=-1){
                  if(unapredtab[j].ulazi[t].brSekcije==postoji(trenutnaSekcija)){
                    if(relokacioni.size()>0)
                    {
                      if(unapredtab[j].ulazi[t].loccnt!=-1){
                        for(int p=0;p<zapisi.size();p++){
                          if(zapisi[p].nazivSekcije==trenutnaSekcija){
                            string pmc=to_string(locationCounter-unapredtab[j].ulazi[t].loccnt);
                            int cou=0,crr=0;
                            while(pmc.size()<4) {string pmc1="0"; pmc=pmc1+pmc;}
                            while(cou<3){
                              zapisi[p].elementi[unapredtab[j].ulazi[t].loccnt+crr]=pmc.substr(cou,2);
                              cou+=2;
                              crr++;
                            }
                          }
                        }
                      }
                      vector<Rela>::iterator itb=relokacioni.begin();
                      relokacioni.erase(itb+indmoj);
                      if(relokacioni.size()==0) relokacioni.clear();
                    }
                  }
                  else{
                    int pom=stoi(Simboli[i].value,nullptr,16)-2;
                    if(pom<0) pom=256*256+pom;
                    string pp=decuHex(pom);
                    cout<<pp;
                    relokacioni[indmoj].addend=pp;
                    relokacioni[indmoj].symbol=Simboli[i].num;
                  }
                }
                else{
                  relokacioni[indmoj].addend=Simboli[i].value;
                  relokacioni[indmoj].symbol=postoji(trenutnaSekcija);
                }
                }
            }
            unapredtab[j].imeSimbola="";
            unapredtab[j].ulazi.clear();
            vector<TabelaUnapred>::iterator it=unapredtab.begin();
            unapredtab.erase(it+j);
          }
        }
      break;
      }
    }
    if(i==Simboli.size()) {
      Simbol s=Simbol();
      if(loc=="") loc="0";
      s.inic(brSimbola,loc,0,"SCTN","LOC",trenutnaSekcija,token[0],true,nullptr);
      Simboli.push_back(s);  
      for(int t=0;t<relokacioni.size();t++){
        if(relokacioni[t].nameSimbol==token[0]){
          relokacioni[t].addend=Simboli[i].value;
          relokacioni[t].symbol=postoji(trenutnaSekcija);
        }
      }  
      brSimbola++;
    }
    token.erase(token.begin());
    izaLabele=token;
  }
  //end
  else if(end(linija)){
    for(int i=0;i<Simboli.size();i++){
      if(Simboli[i].name==trenutnaSekcija){
        Simboli[i].size=locationCounter;
        break;
      }
    }
  }
  //jednobajtne
  else if(jednobajtne(linija)){
    if(token[0]=="halt"){
      for(int i=0;i<zapisi.size();i++){
        if(zapisi[i].nazivSekcije==trenutnaSekcija){
            zapisi[i].elementi.push_back("00");
        }
      }
    }
    else if(token[0]=="iret"){
      for(int i=0;i<zapisi.size();i++){
        if(zapisi[i].nazivSekcije==trenutnaSekcija){
            zapisi[i].elementi.push_back("20");
        }
      }
    }
    else if(token[0]=="ret"){
      for(int i=0;i<zapisi.size();i++){
        if(zapisi[i].nazivSekcije==trenutnaSekcija){
            zapisi[i].elementi.push_back("40");
        }
      }
    }
    locationCounter+=1;
  }
  //dvobajtne
  else if(dvobajtne(linija)){
    if(token[0]=="xchg"){
      for(int i=0;i<zapisi.size();i++){
        if(zapisi[i].nazivSekcije==trenutnaSekcija){
          zapisi[i].elementi.push_back("60");
          string regd=token[1].substr(1,token[1].size()-1);
          string regs=token[2].substr(1,token[2].size()-1);
          regd+=regs;
          zapisi[i].elementi.push_back(regd);
        }
      }
    }
    else if(token[0]=="add"){
      for(int i=0;i<zapisi.size();i++){
        if(zapisi[i].nazivSekcije==trenutnaSekcija){
          zapisi[i].elementi.push_back("70");
          string regd=token[1].substr(1,token[1].size()-1);
          string regs=token[2].substr(1,token[2].size()-1);
          regd+=regs;
          zapisi[i].elementi.push_back(regd);
        }
      }
    }
    else if(token[0]=="sub"){
      for(int i=0;i<zapisi.size();i++){
        if(zapisi[i].nazivSekcije==trenutnaSekcija){
          zapisi[i].elementi.push_back("71");
          string regd=token[1].substr(1,token[1].size()-1);
          string regs=token[2].substr(1,token[2].size()-1);
          regd+=regs;
          zapisi[i].elementi.push_back(regd);
        }
      }
    }
     else if(token[0]=="mul"){
      for(int i=0;i<zapisi.size();i++){
        if(zapisi[i].nazivSekcije==trenutnaSekcija){
          zapisi[i].elementi.push_back("72");
          string regd=token[1].substr(1,token[1].size()-1);
          string regs=token[2].substr(1,token[2].size()-1);
          regd+=regs;
          zapisi[i].elementi.push_back(regd);
        }
      }
    }
    else if(token[0]=="div"){
      for(int i=0;i<zapisi.size();i++){
        if(zapisi[i].nazivSekcije==trenutnaSekcija){
          zapisi[i].elementi.push_back("73");
          string regd=token[1].substr(1,token[1].size()-1);
          string regs=token[2].substr(1,token[2].size()-1);
          regd+=regs;
          zapisi[i].elementi.push_back(regd);
        }
      }
    }
    else if(token[0]=="cmp"){
      for(int i=0;i<zapisi.size();i++){
        if(zapisi[i].nazivSekcije==trenutnaSekcija){
          zapisi[i].elementi.push_back("74");
          string regd=token[1].substr(1,token[1].size()-1);
          string regs=token[2].substr(1,token[2].size()-1);
          regd+=regs;
          zapisi[i].elementi.push_back(regd);
        }
      }
    }
    else if(token[0]=="and"){
      for(int i=0;i<zapisi.size();i++){
        if(zapisi[i].nazivSekcije==trenutnaSekcija){
          zapisi[i].elementi.push_back("81");
          string regd=token[1].substr(1,token[1].size()-1);
          string regs=token[2].substr(1,token[2].size()-1);
          regd+=regs;
          zapisi[i].elementi.push_back(regd);
        }
      }
    }
    else if(token[0]=="or"){
      for(int i=0;i<zapisi.size();i++){
        if(zapisi[i].nazivSekcije==trenutnaSekcija){
          zapisi[i].elementi.push_back("82");
          string regd=token[1].substr(1,token[1].size()-1);
          string regs=token[2].substr(1,token[2].size()-1);
          regd+=regs;
          zapisi[i].elementi.push_back(regd);
        }
      }
    }
    else if(token[0]=="xor"){
      for(int i=0;i<zapisi.size();i++){
        if(zapisi[i].nazivSekcije==trenutnaSekcija){
          zapisi[i].elementi.push_back("83");
          string regd=token[1].substr(1,token[1].size()-1);
          string regs=token[2].substr(1,token[2].size()-1);
          regd+=regs;
          zapisi[i].elementi.push_back(regd);
        }
      }
    }
    else if(token[0]=="test"){
      for(int i=0;i<zapisi.size();i++){
        if(zapisi[i].nazivSekcije==trenutnaSekcija){
          zapisi[i].elementi.push_back("84");
          string regd=token[1].substr(1,token[1].size()-1);
          string regs=token[2].substr(1,token[2].size()-1);
          regd+=regs;
          zapisi[i].elementi.push_back(regd);
        }
      }
    }
    else if(token[0]=="shl"){
      for(int i=0;i<zapisi.size();i++){
        if(zapisi[i].nazivSekcije==trenutnaSekcija){
          zapisi[i].elementi.push_back("90");
          string regd=token[1].substr(1,token[1].size()-1);
          string regs=token[2].substr(1,token[2].size()-1);
          regd+=regs;
          zapisi[i].elementi.push_back(regd);
        }
      }
    }
    else if(token[0]=="shr"){
      for(int i=0;i<zapisi.size();i++){
        if(zapisi[i].nazivSekcije==trenutnaSekcija){
          zapisi[i].elementi.push_back("91");
          string regd=token[1].substr(1,token[1].size()-1);
          string regs=token[2].substr(1,token[2].size()-1);
          regd+=regs;
          zapisi[i].elementi.push_back(regd);
        }
      }
    }
    locationCounter+=2;
  }
  //skokovi
  else if(skok(linija) || call(linija)){
    string opkod;
    if(skok(linija)){
      opkod=linija.substr(0,3);
      linija=linija.substr(4,linija.size()-4);
      brisanjeKom(linija);
      if(opkod=="jmp"){
        for(int i=0;i<zapisi.size();i++){
          if(zapisi[i].nazivSekcije==trenutnaSekcija)
            zapisi[i].elementi.push_back("50");
          }
      }
      if(opkod=="jeq") {
        for(int i=0;i<zapisi.size();i++){
          if(zapisi[i].nazivSekcije==trenutnaSekcija)
            zapisi[i].elementi.push_back("51");
          }
      }
      if(opkod=="jne") {
        for(int i=0;i<zapisi.size();i++){
          if(zapisi[i].nazivSekcije==trenutnaSekcija)
            zapisi[i].elementi.push_back("52");
          }
      }
      if(opkod=="jgt") {
        for(int i=0;i<zapisi.size();i++){
          if(zapisi[i].nazivSekcije==trenutnaSekcija)
            zapisi[i].elementi.push_back("53");
          }
      }
    }
    else{
      opkod=linija.substr(0,4);
      linija=linija.substr(5,linija.size()-5);
      brisanjeKom(linija);
      for(int i=0;i<zapisi.size();i++){
        if(zapisi[i].nazivSekcije==trenutnaSekcija)
          zapisi[i].elementi.push_back("30");
        }
    }
      locationCounter+=1;   
      
      string adresiranje=operandizaskokove(linija);
      if(adresiranje=="neposredno"){
          for(int j=0;j<zapisi.size();j++){
            if(zapisi[j].nazivSekcije==trenutnaSekcija){
              zapisi[j].elementi.push_back("FF");
              zapisi[j].elementi.push_back("00");   
              locationCounter+=2;     
            if(literal(token[1])){
              if(regex_match(token[1], regex("((0x([0-9]|[a-f]|[A-F])+))$"))){
                token[1]=token[1].substr(2,token[1].size()-2);                  
                while(token[1].size()%4!=0){string dodaj="0"; dodaj+=token[1]; token[1]=dodaj;}
                int k=0;
                  while(token[1].size()>k){
                    string unos="";
                    unos=unos+token[1][k];
                    unos=unos+token[1][k+1];
                    token[1]=token[1].substr(k+1,token[1].size()-1);      
                    zapisi[j].elementi.push_back(unos);
                    k++;
                    locationCounter+=1;
                  }  
              }
            else{
              long dec=stoi(token[1]);
              string loc=decuHex(dec);
              while(loc.size()%4!=0){string dodaj="0"; dodaj+=loc; loc=dodaj;}
              int k=0;
              while(loc.size()>k){
                string unos="";
                unos=unos+loc[k];
                unos=unos+loc[k+1];
                loc=loc.substr(k+1,loc.size()-1);
                zapisi[j].elementi.push_back(unos);
                locationCounter+=1;
                k++;
              }  
            }
    }
    else{
      int k=-1;
      if(k=postoji(token[1])!=(int)(-1)){
        k=postoji(token[1]);
        Rela r=Rela();int j=0;
        string loc=decuHex(locationCounter);
        if(Simboli[k].bind=="GLOB"){             
          r=Rela(ind++,trenutnaSekcija,token[1],loc,"R_X86_32",Simboli[k].num,"0");
        }  
        else if(Simboli[k].bind=="LOC"){
          if(Simboli[k].defined==true){
            r=Rela(ind++,trenutnaSekcija,token[1],loc,"R_X86_32",postoji(trenutnaSekcija),Simboli[k].value);
          }
          else{
            r=Rela(ind,trenutnaSekcija,token[1],loc,"R_X86_32",postoji(trenutnaSekcija),Simboli[k].value);
            int j=0;
            for(j=0;j<unapredtab.size();j++){
              if(unapredtab[j].imeSimbola==token[1]){
                unapredtab[j].ulazi.push_back(Unapred(ind++));break;
              }
            }
            if(j==unapredtab.size()){
              unapredtab.push_back(TabelaUnapred(token[1]));
              unapredtab[unapredtab.size()-1].ulazi.push_back(Unapred(ind++));
            }

          }
        }
        relokacioni.push_back(r);
        }
        else{
          Simbol s=Simbol();
          s.inic(brSimbola++,decuHex(locationCounter),0,"NOTYP","LOC",to_string(postoji(trenutnaSekcija)),token[1],false,nullptr);
          Simboli.push_back(s);
          Rela r=Rela();
          string loc=decuHex(locationCounter);
          r=Rela(ind,trenutnaSekcija,token[1],loc,"R_X86_32",postoji(trenutnaSekcija),Simboli[k].value);
          int j=0;
          for(j=0;j<unapredtab.size();j++){
            if(unapredtab[j].imeSimbola==token[1]){
              unapredtab[j].ulazi.push_back(Unapred(ind++));break;
            }
          }
          if(j==unapredtab.size()){
            unapredtab.push_back(TabelaUnapred(token[1]));
            unapredtab[unapredtab.size()-1].ulazi.push_back(Unapred(ind++));
          }
          relokacioni.push_back(r);
        }
        for(int i=0;i<zapisi.size();i++){
          if(zapisi[i].nazivSekcije==trenutnaSekcija){
            zapisi[i].elementi.push_back("00");
            zapisi[i].elementi.push_back("00");
            locationCounter+=2;
          }
        } 
    }
    }
    }
  }
  else if(adresiranje=="pcrelativno"){
    token[1]=token[1].substr(1,token[1].size()-1);
    int k=-1;
    for(int i=0;i<zapisi.size();i++){
      if(zapisi[i].nazivSekcije==trenutnaSekcija){
        zapisi[i].elementi.push_back("F7");
        zapisi[i].elementi.push_back("05");
        locationCounter+=2;
      }
    }
    if(k=postoji(token[1])!=(int)(-1)){
      k=postoji(token[1]);
      if(trenutnaSekcija!=Simboli[k].ndx){
      Rela r=Rela();
      int j=0;
      string loc=decuHex(locationCounter);
      if(Simboli[k].bind=="GLOB"){  
        if(Simboli[k].defined==true){
          r=Rela(ind++,trenutnaSekcija,token[1],loc,"R_X86_PC16",postoji(trenutnaSekcija),"FFFE");
        }
        else{           
        r=Rela(ind,trenutnaSekcija,token[1],loc,"R_X86_PC16",Simboli[k].num,"FFFE");
        for(j=0;j<unapredtab.size();j++){
          if(unapredtab[j].imeSimbola==token[1]){
            Unapred u=Unapred(ind++);
            if(Simboli[k].ndx=="UND") u.loccnt=locationCounter;
            u.brSekcije=postoji(trenutnaSekcija);
            unapredtab[j].ulazi.push_back(u);break;
            }
          }
          if(j==unapredtab.size()){
            unapredtab.push_back(TabelaUnapred(token[1]));
            Unapred u=Unapred(ind++);
            if(Simboli[k].ndx=="UND") u.loccnt=locationCounter;
            u.brSekcije=postoji(trenutnaSekcija);
            unapredtab[unapredtab.size()-1].ulazi.push_back(u);
          }
      }
          relokacioni.push_back(r);
      }  
      else if(Simboli[k].bind=="LOC"){
        if(Simboli[k].defined==true){
          int pom=stoi(Simboli[k].value,nullptr,16)-2;
          if(pom<0) pom=256*256+pom;
          string pp=decuHex(pom);
          cout<<pp<<'\n';
          r=Rela(ind++,trenutnaSekcija,token[1],loc,"R_X86_PC16",postoji(trenutnaSekcija),pp);
        }
        else{
          r=Rela(ind,trenutnaSekcija,token[1],loc,"R_X86_PC16",postoji(trenutnaSekcija),Simboli[k].value);
          int j=0;
          for(j=0;j<unapredtab.size();j++){
            if(unapredtab[j].imeSimbola==token[1]){
              Unapred u=Unapred(ind++);
              if(Simboli[k].ndx=="UND") u.loccnt=locationCounter;
              u.brSekcije=postoji(trenutnaSekcija);
              unapredtab[j].ulazi.push_back(u);break;
            }
          }
          if(j==unapredtab.size()){
            unapredtab.push_back(TabelaUnapred(token[1]));
            Unapred u=Unapred(ind++);
            if(Simboli[k].ndx=="UND") u.loccnt=locationCounter;
            u.brSekcije=postoji(trenutnaSekcija);
            unapredtab[unapredtab.size()-1].ulazi.push_back(u);
          }
        }
        relokacioni.push_back(r);
        }
      }
        for(int i=0;i<zapisi.size();i++){
          if(zapisi[i].nazivSekcije==trenutnaSekcija){
          zapisi[i].elementi.push_back("00");
          zapisi[i].elementi.push_back("00");
          locationCounter+=2;
        }
        }
      
    }
      else{
        Simbol s=Simbol();        
        s.inic(brSimbola++,decuHex(locationCounter),0,"NOTYP","LOC",to_string(postoji(trenutnaSekcija)),token[1],false,nullptr);
        Simboli.push_back(s);
        Rela r=Rela();
        string loc=decuHex(locationCounter);
        r=Rela(ind,trenutnaSekcija,token[1],loc,"R_X86_PC16",postoji(trenutnaSekcija),Simboli[k].value);
        int j=0;
        for(j=0;j<unapredtab.size();j++){
          if(unapredtab[j].imeSimbola==token[1]){
            Unapred u=Unapred(ind++);
            if(Simboli[k].ndx=="UND") u.loccnt=locationCounter;
            u.brSekcije=postoji(trenutnaSekcija);
            unapredtab[j].ulazi.push_back(u);break;
          }
        }
        if(j==unapredtab.size()){
          unapredtab.push_back(TabelaUnapred(token[1]));
          Unapred u=Unapred(ind++);
          u.brSekcije=postoji(trenutnaSekcija);
          if(Simboli[k].ndx=="UND") u.loccnt=locationCounter;
          unapredtab[unapredtab.size()-1].ulazi.push_back(u);
        }
        relokacioni.push_back(r);
         for(int i=0;i<zapisi.size();i++){
          if(zapisi[i].nazivSekcije==trenutnaSekcija){
            zapisi[i].elementi.push_back("00");
            zapisi[i].elementi.push_back("00");
            locationCounter+=2;
          }
         }
      } 
  }
  else if(adresiranje=="memorijsko"){
    int k=0;
    token[1]=token[1].substr(1,token[1].size());
    for(int j=0;j<zapisi.size();j++){
      if(zapisi[j].nazivSekcije==trenutnaSekcija){
        zapisi[j].elementi.push_back("FF");
        zapisi[j].elementi.push_back("04"); 
        locationCounter+=2;
      if(literal(token[1])){
      if(regex_match(token[1], regex("((0x([0-9]|[a-f]|[A-F])+))$"))){        
      token[1]=token[1].substr(2,token[1].size()-2);        
      while(token[1].size()%4!=0){string dodaj="0"; dodaj+=token[1]; token[1]=dodaj;}
      if(token[1].length()/2>2)
      k=0;
      while(token[1].size()>k){
        string unos="";
        unos=unos+token[1][k];
        unos=unos+token[1][k+1];
        token[1]=token[1].substr(k+1,token[1].size()-1);      
        zapisi[j].elementi.push_back(unos);
        locationCounter+=1;
        k++;
      }  
      }
      else{
        long dec=stoi(token[1]);
        string loc=decuHex(dec);
        while(loc.size()%4!=0){string dodaj="0"; dodaj+=loc; loc=dodaj;}
        int k=0;
        while(loc.size()>k){
          string unos="";
          unos=unos+loc[k];
          unos=unos+loc[k+1];
          loc=loc.substr(k+1,loc.size()-1);
          zapisi[j].elementi.push_back(unos);
          locationCounter+=1;
          k++;
        }  
      } 
      }
    else{
      int k=-1;
      if(k=postoji(token[1])!=(int)(-1)){
        k=postoji(token[1]);
        Rela r=Rela();
        int j=0;
        string loc=decuHex(locationCounter);
        if(Simboli[k].bind=="GLOB"){             
          r=Rela(ind++,trenutnaSekcija,token[1],loc,"R_X86_32",Simboli[k].num,"0");
        }  
        else if(Simboli[k].bind=="LOC"){
          if(Simboli[k].defined==true){
            r=Rela(ind++,trenutnaSekcija,token[1],loc,"R_X86_32",postoji(trenutnaSekcija),Simboli[k].value);
          }
          else{
            r=Rela(ind,trenutnaSekcija,token[1],loc,"R_X86_32",postoji(trenutnaSekcija),Simboli[k].value);
            int j=0;
            for(j=0;j<unapredtab.size();j++){
              if(unapredtab[j].imeSimbola==token[1]){
                unapredtab[j].ulazi.push_back(Unapred(ind++));break;
              }
            }
            if(j==unapredtab.size()){
              unapredtab.push_back(TabelaUnapred(token[1]));
              unapredtab[unapredtab.size()-1].ulazi.push_back(Unapred(ind++));
            }
          }
        }
        relokacioni.push_back(r);
      }
      else{
        Simbol s=Simbol();        
        s.inic(brSimbola++,decuHex(locationCounter),0,"NOTYP","LOC",to_string(postoji(trenutnaSekcija)),token[1],false,nullptr);
        Simboli.push_back(s);
        Rela r=Rela();
        string loc=decuHex(locationCounter);
        r=Rela(ind,trenutnaSekcija,token[1],loc,"R_X86_32",postoji(trenutnaSekcija),Simboli[k].value);
        int j=0;
        for(j=0;j<unapredtab.size();j++){
          if(unapredtab[j].imeSimbola==token[1]){
            unapredtab[j].ulazi.push_back(Unapred(ind++));break;
          }
        }
        if(j==unapredtab.size()){
          unapredtab.push_back(TabelaUnapred(token[1]));
          unapredtab[unapredtab.size()-1].ulazi.push_back(Unapred(ind++));
        }
        relokacioni.push_back(r);
      }
    for(int i=0;i<zapisi.size();i++){
      if(zapisi[i].nazivSekcije==trenutnaSekcija){
        zapisi[i].elementi.push_back("00");
        zapisi[i].elementi.push_back("00");
        locationCounter+=2;
      }
    }
  }
      }      
        }
  }
  else if(adresiranje=="regdir"){
    string pomsad="";
    pomsad=token[1].substr(1,1);
    for(int j=0;j<zapisi.size();j++){
      if(zapisi[j].nazivSekcije==trenutnaSekcija){
        string reg="F";
        reg=reg+token[1].substr(2,token[1].size()-2);
        zapisi[j].elementi.push_back(reg);
        zapisi[j].elementi.push_back("01");
        locationCounter+=2;
      }
    }
  }
  else if(adresiranje=="regindbez"){
    string pomsad="";
    pomsad=token[1].substr(2,1);
    for(int j=0;j<zapisi.size();j++){
      if(zapisi[j].nazivSekcije==trenutnaSekcija){
        string reg="F";
        reg=reg+token[1].substr(3,1);
        zapisi[j].elementi.push_back(reg);
        zapisi[j].elementi.push_back("02");
        locationCounter+=2;
      }
    }
  }
  else if(adresiranje=="regindsa"){    
    string pomsad="";
    string deci=token[2].substr(1,token[2].size()-2);
    pomsad=token[1].substr(2,1);
    for(int j=0;j<zapisi.size();j++){
      if(zapisi[j].nazivSekcije==trenutnaSekcija){
        string reg="F";
        reg=reg+token[1].substr(3,1);
        zapisi[j].elementi.push_back(reg);
        zapisi[j].elementi.push_back("03");
        locationCounter+=2;
      }
      if(literal(deci)){
        if(!(regex_match(deci, regex("^((0x([0-9]|[a-f]|[A-F])+))$")))){
          long dec=stoi(deci);
          string loc=decuHex(dec);
          while(loc.size()%4!=0){string dodaj="0"; dodaj+=loc; loc=dodaj;}
          kk=0;
          while(loc.size()>kk){
            string unos="";
            unos=unos+loc[kk];
            unos=unos+loc[kk+1];
            loc=loc.substr(kk+1,loc.size()-1);
            zapisi[j].elementi.push_back(unos);
            locationCounter+=1;
            kk++;
          }  
        }
        else{    
          deci=deci.substr(2,token[1].size()-2);                  
          while(deci.size()%4!=0){string dodaj="0"; dodaj+=deci; deci=dodaj;}
          if(deci.length()/2>2)
          kk=0;
          while(deci.size()>kk){
            string unos="";
            unos=unos+deci[kk];
            unos=unos+deci[kk+1];
            deci=deci.substr(kk+1,deci.size()-1);      
            zapisi[j].elementi.push_back(unos);
            locationCounter+=1;
            kk++;
          }  
        }
      }
      else{
        int k=-1;
        if(k=postoji(deci)!=(int)(-1)){
          k=postoji(deci);
          Rela r=Rela();int j=0;
          string loc=decuHex(locationCounter);
          if(Simboli[k].bind=="GLOB"){             
            r=Rela(ind++,trenutnaSekcija,deci,loc,"R_X86_32",Simboli[k].num,"0");
          }  
          else if(Simboli[k].bind=="LOC"){
            if(Simboli[k].defined==true){
              r=Rela(ind++,trenutnaSekcija,deci,loc,"R_X86_32",postoji(trenutnaSekcija),Simboli[k].value);
            }
            else{
              r=Rela(ind,trenutnaSekcija,deci,loc,"R_X86_32",postoji(trenutnaSekcija),Simboli[k].value);
              int j=0;
              for(j=0;j<unapredtab.size();j++){
                if(unapredtab[j].imeSimbola==deci){
                  unapredtab[j].ulazi.push_back(Unapred(ind++));break;
                }
              }
              if(j==unapredtab.size()){
                unapredtab.push_back(TabelaUnapred(deci));
                unapredtab[unapredtab.size()-1].ulazi.push_back(Unapred(ind++));
              }
            }
          }
          relokacioni.push_back(r);
        }
        else{
          Simbol s=Simbol();
          s.inic(brSimbola++,decuHex(locationCounter),0,"NOTYP","LOC",to_string(postoji(trenutnaSekcija)),deci,false,nullptr);
          Simboli.push_back(s);
          Rela r=Rela();
          string loc=decuHex(locationCounter);
          r=Rela(ind,trenutnaSekcija,deci,loc,"R_X86_32",postoji(trenutnaSekcija),Simboli[k].value);
          int j=0;
          for(j=0;j<unapredtab.size();j++){
            if(unapredtab[j].imeSimbola==deci){
              unapredtab[j].ulazi.push_back(Unapred(ind++));break;
            }
          }
          if(j==unapredtab.size()){
            unapredtab.push_back(TabelaUnapred(deci));
            unapredtab[unapredtab.size()-1].ulazi.push_back(Unapred(ind++));
          }
          relokacioni.push_back(r);
        }
        for(int i=0;i<zapisi.size();i++){
          if(zapisi[i].nazivSekcije==trenutnaSekcija){
            zapisi[i].elementi.push_back("00");
            zapisi[i].elementi.push_back("00");
            locationCounter+=2;
          }
        }
    }
    }
  }
  }
  else if(intm(linija)){
    for(int i=0;i<zapisi.size();i++){
      if(zapisi[i].nazivSekcije==trenutnaSekcija){
        zapisi[i].elementi.push_back("10");
        string r="F";
        string regd=token[1].substr(1,token[1].size()-1);  
        regd+=r;
        zapisi[i].elementi.push_back(regd);
      }
    }
    locationCounter+=2;
  }
  else if(notm(linija)){
    for(int i=0;i<zapisi.size();i++){
      if(zapisi[i].nazivSekcije==trenutnaSekcija){
        zapisi[i].elementi.push_back("80");
        string r="F";
        string regd=token[1].substr(1,token[1].size()-1);  
        regd+=r;
        zapisi[i].elementi.push_back(regd);
      }
    }
    locationCounter+=2;
  }
  
  else if(ldst(linija)){
    if(token[0][0]=='l'){
      for(int i=0;i<zapisi.size();i++){
      if(zapisi[i].nazivSekcije==trenutnaSekcija){
        zapisi[i].elementi.push_back("A0");
      }
    }
    }
    else{
      for(int i=0;i<zapisi.size();i++){
      if(zapisi[i].nazivSekcije==trenutnaSekcija){
        zapisi[i].elementi.push_back("B0");
      }
    }
    }
    locationCounter+=1;
    brisanjeKom(linija);
    string lin=linija.substr(7,linija.size()-7);
    brisanjeKom(lin);
    string adresiranje=operandizaucitavanje(lin);
    for(int i=0;i<zapisi.size();i++){
      if(zapisi[i].nazivSekcije==trenutnaSekcija){
        if(adresiranje!="regdir" && adresiranje!="regindbez" && adresiranje!="regindsa" && adresiranje!="pcrelativno"){
          string r="F";
          string regd=token[1].substr(1,token[1].size()-1);  
          regd=regd+r;
          zapisi[i].elementi.push_back(regd);
        }
        else if(adresiranje=="regdir"){
          string r;
          string regd=token[1].substr(1,token[1].size()-1);
          r+=regd;
          string regs=token[2].substr(1,token[2].size()-1);
          r+=regs;
          zapisi[i].elementi.push_back(r);
        }
        else if(adresiranje=="regindsa"){
          string r;
          string regd=token[1].substr(1,token[1].size()-1);
          r+=regd;
          string regs=token[2].substr(2,token[2].size()-2);
          r+=regs;
          zapisi[i].elementi.push_back(r);
        }
         else if(adresiranje=="pcrelativno"){
          string r;
          string regd=token[1].substr(1,token[1].size()-1);
          r+=regd;
          string regs="7";
          r+=regs;
          zapisi[i].elementi.push_back(r);
        }
        else if(adresiranje=="regindbez"){
          string r;
          string regd=token[1].substr(1,token[1].size()-1);
          r+=regd;
          string regs=token[2].substr(2,token[2].size()-3);
          r+=regs;
          zapisi[i].elementi.push_back(r);
        }
      }
    }
    locationCounter+=1;
    if(adresiranje=="neposredno"){
      brisanjeKom(token[2]);
      token[2]=token[2].substr(1,token[2].size()-1);
      for(int j=0;j<zapisi.size();j++){
        if(zapisi[j].nazivSekcije==trenutnaSekcija){
          zapisi[j].elementi.push_back("00");   
          locationCounter+=1;     
          if(literal(token[2])){
            if(regex_match(token[2], regex("((0x([0-9]|[a-f]|[A-F])+))$"))){
              token[2]=token[2].substr(2,token[2].size()-2);                  
              while(token[2].size()%4!=0){string dodaj="0"; dodaj+=token[2]; token[2]=dodaj;}
              int k=0;
                while(token[2].size()>k){
                  string unos="";
                  unos=unos+token[2][k];
                  unos=unos+token[2][k+1];
                  token[2]=token[2].substr(k+1,token[2].size()-1);      
                  zapisi[j].elementi.push_back(unos);
                  k++;
                }  
        }
        else{
              long dec=stoi(token[2]);
              string loc=decuHex(dec);
              while(loc.size()%4!=0){string dodaj="0"; dodaj+=loc; loc=dodaj;}
              int k=0;
              while(loc.size()>k){
                string unos="";
                unos=unos+loc[k];
                unos=unos+loc[k+1];
                loc=loc.substr(k+1,loc.size()-1);
                zapisi[j].elementi.push_back(unos);
                k++;
              }  
            }
        locationCounter+=2;
    }
    else{
      int k=-1;
      if(k=postoji(token[2])!=(int)(-1)){
        k=postoji(token[2]);
        Rela r=Rela();int j=0;
        string loc=decuHex(locationCounter);
        if(Simboli[k].bind=="GLOB"){             
          r=Rela(ind++,trenutnaSekcija,token[1],loc,"R_X86_32",Simboli[k].num,"0");
        }  
        else if(Simboli[k].bind=="LOC"){
          if(Simboli[k].defined==true){
            r=Rela(ind++,trenutnaSekcija,token[2],loc,"R_X86_32",postoji(trenutnaSekcija),Simboli[k].value);
          }
          else{
            r=Rela(ind,trenutnaSekcija,token[2],loc,"R_X86_32",postoji(trenutnaSekcija),Simboli[k].value);
            int j=0;
            for(j=0;j<unapredtab.size();j++){
              if(unapredtab[j].imeSimbola==token[2]){
                unapredtab[j].ulazi.push_back(Unapred(ind++));break;
              }
            }
            if(j==unapredtab.size()){
              unapredtab.push_back(TabelaUnapred(token[2]));
              unapredtab[unapredtab.size()-1].ulazi.push_back(Unapred(ind++));
            }

          }
        }
        relokacioni.push_back(r);
        }
        else{
          Simbol s=Simbol();
          s.inic(brSimbola++,decuHex(locationCounter),0,"NOTYP","LOC",to_string(postoji(trenutnaSekcija)),token[2],false,nullptr);
          Simboli.push_back(s);
          Rela r=Rela();
          string loc=decuHex(locationCounter);
          r=Rela(ind,trenutnaSekcija,token[2],loc,"R_X86_32",postoji(trenutnaSekcija),Simboli[k].value);
          int j=0;
          for(j=0;j<unapredtab.size();j++){
            if(unapredtab[j].imeSimbola==token[2]){
              unapredtab[j].ulazi.push_back(Unapred(ind++));break;
            }
          }
          if(j==unapredtab.size()){
            unapredtab.push_back(TabelaUnapred(token[2]));
            unapredtab[unapredtab.size()-1].ulazi.push_back(Unapred(ind++));
          }
          relokacioni.push_back(r);
        }
        locationCounter+=2;
        for(int i=0;i<zapisi.size();i++){
          if(zapisi[i].nazivSekcije==trenutnaSekcija){
            zapisi[i].elementi.push_back("00");
            zapisi[i].elementi.push_back("00");
          }
        } 
    }
    }
    }
    }
    else if(adresiranje=="pcrelativno"){
    token[2]=token[2].substr(1,token[2].size()-1);
    int k=-1;
    for(int i=0;i<zapisi.size();i++){
      if(zapisi[i].nazivSekcije==trenutnaSekcija){
        zapisi[i].elementi.push_back("05");
        locationCounter+=1;
      }
    }
    if(k=postoji(token[2])!=(int)(-1)){
      k=postoji(token[2]);
      if(trenutnaSekcija!=Simboli[k].ndx){
      Rela r=Rela();
      int j=0;
      string loc=decuHex(locationCounter);
      if(Simboli[k].bind=="GLOB"){  
        if(Simboli[k].defined==true){
          r=Rela(ind++,trenutnaSekcija,token[2],loc,"R_X86_PC16",postoji(trenutnaSekcija),"FFFE");
        }
        else{           
        r=Rela(ind,trenutnaSekcija,token[2],loc,"R_X86_PC16",Simboli[k].num,"FFFE");
        for(j=0;j<unapredtab.size();j++){
          if(unapredtab[j].imeSimbola==token[2]){
            Unapred u=Unapred(ind++);
            if(Simboli[k].ndx=="UND") u.loccnt=locationCounter;
            u.brSekcije=postoji(trenutnaSekcija);
            unapredtab[j].ulazi.push_back(u);break;
            }
          }
          if(j==unapredtab.size()){
            unapredtab.push_back(TabelaUnapred(token[2]));
            Unapred u=Unapred(ind++);
            if(Simboli[k].ndx=="UND") u.loccnt=locationCounter;
            u.brSekcije=postoji(trenutnaSekcija);
            unapredtab[unapredtab.size()-1].ulazi.push_back(u);
          }
      }
          relokacioni.push_back(r);
      }  
      else if(Simboli[k].bind=="LOC"){
        if(Simboli[k].defined==true){
          int pom=stoi(Simboli[k].value,nullptr,16)-2;
          if(pom<0) pom=256*256+pom;
          string pp=decuHex(pom);
          cout<<pp<<'\n';
          r=Rela(ind++,trenutnaSekcija,token[2],loc,"R_X86_PC16",postoji(trenutnaSekcija),pp);
        }
        else{
          r=Rela(ind,trenutnaSekcija,token[2],loc,"R_X86_PC16",postoji(trenutnaSekcija),Simboli[k].value);
          int j=0;
          for(j=0;j<unapredtab.size();j++){
            if(unapredtab[j].imeSimbola==token[2]){
              Unapred u=Unapred(ind++);
              if(Simboli[k].ndx=="UND") u.loccnt=locationCounter;
              u.brSekcije=postoji(trenutnaSekcija);
              unapredtab[j].ulazi.push_back(u);break;
            }
          }
          if(j==unapredtab.size()){
            unapredtab.push_back(TabelaUnapred(token[2]));
            Unapred u=Unapred(ind++);
            if(Simboli[k].ndx=="UND") u.loccnt=locationCounter;
            u.brSekcije=postoji(trenutnaSekcija);
            unapredtab[unapredtab.size()-1].ulazi.push_back(u);
          }
        }
        relokacioni.push_back(r);
        }
      }
        for(int i=0;i<zapisi.size();i++){
          if(zapisi[i].nazivSekcije==trenutnaSekcija){
          zapisi[i].elementi.push_back("00");
          zapisi[i].elementi.push_back("00");
          locationCounter+=2;
        }
        }
      
    }
      else{
        Simbol s=Simbol();        
        s.inic(brSimbola++,decuHex(locationCounter),0,"NOTYP","LOC",to_string(postoji(trenutnaSekcija)),token[2],false,nullptr);
        Simboli.push_back(s);
        Rela r=Rela();
        string loc=decuHex(locationCounter);
        r=Rela(ind,trenutnaSekcija,token[2],loc,"R_X86_PC16",postoji(trenutnaSekcija),Simboli[k].value);
        int j=0;
        for(j=0;j<unapredtab.size();j++){
          if(unapredtab[j].imeSimbola==token[2]){
            Unapred u=Unapred(ind++);
            if(Simboli[k].ndx=="UND") u.loccnt=locationCounter;
            u.brSekcije=postoji(trenutnaSekcija);
            unapredtab[j].ulazi.push_back(u);break;
          }
        }
        if(j==unapredtab.size()){
          unapredtab.push_back(TabelaUnapred(token[2]));
          Unapred u=Unapred(ind++);
          u.brSekcije=postoji(trenutnaSekcija);
          if(Simboli[k].ndx=="UND") u.loccnt=locationCounter;
          unapredtab[unapredtab.size()-1].ulazi.push_back(u);
        }
        relokacioni.push_back(r);
         for(int i=0;i<zapisi.size();i++){
          if(zapisi[i].nazivSekcije==trenutnaSekcija){
            zapisi[i].elementi.push_back("00");
            zapisi[i].elementi.push_back("00");
            locationCounter+=2;
          }
         }
      } 
  }
    else if(adresiranje=="memorijsko"){
      int k=0;
      for(int j=0;j<zapisi.size();j++){
        if(zapisi[j].nazivSekcije==trenutnaSekcija){
          zapisi[j].elementi.push_back("04"); 
          locationCounter+=1;
        if(literal(token[2])){
        if(regex_match(token[2], regex("((0x([0-9]|[a-f]|[A-F])+))$"))){        
        token[2]=token[2].substr(2,token[2].size()-2);        
        while(token[2].size()%4!=0){string dodaj="0"; dodaj+=token[2]; token[2]=dodaj;}
        if(token[2].length()/2>2)
        k=0;
        while(token[2].size()>k){
          string unos="";
          unos=unos+token[2][k];
          unos=unos+token[2][k+1];
          token[2]=token[2].substr(k+1,token[2].size()-1);      
          zapisi[j].elementi.push_back(unos);
          k++;
        }  
      }
      else{
        long dec=stoi(token[2]);
        string loc=decuHex(dec);
        while(loc.size()%4!=0){string dodaj="0"; dodaj+=loc; loc=dodaj;}
        int k=0;
        while(loc.size()>k){
          string unos="";
          unos=unos+loc[k];
          unos=unos+loc[k+1];
          loc=loc.substr(k+1,loc.size()-1);
          zapisi[j].elementi.push_back(unos);
          k++;
        }  
      } 
      }
    else{
      int k=-1;
      if(k=postoji(token[2])!=(int)(-1)){
        k=postoji(token[2]);
        Rela r=Rela();
        int j=0;
        string loc=decuHex(locationCounter);
        if(Simboli[k].bind=="GLOB"){             
          r=Rela(ind++,trenutnaSekcija,token[2],loc,"R_X86_32",Simboli[k].num,"0");
        }  
        else if(Simboli[k].bind=="LOC"){
          if(Simboli[k].defined==true){
            r=Rela(ind++,trenutnaSekcija,token[2],loc,"R_X86_32",postoji(trenutnaSekcija),Simboli[k].value);
          }
          else{
            r=Rela(ind,trenutnaSekcija,token[2],loc,"R_X86_32",postoji(trenutnaSekcija),Simboli[k].value);
            int j=0;
            for(j=0;j<unapredtab.size();j++){
              if(unapredtab[j].imeSimbola==token[2]){
                unapredtab[j].ulazi.push_back(Unapred(ind++));break;
              }
            }
            if(j==unapredtab.size()){
              unapredtab.push_back(TabelaUnapred(token[2]));
              unapredtab[unapredtab.size()-1].ulazi.push_back(Unapred(ind++));
            }
          }
        }
        relokacioni.push_back(r);
      }
      else{
        Simbol s=Simbol();
        
        s.inic(brSimbola++,decuHex(locationCounter),0,"NOTYP","LOC",to_string(postoji(trenutnaSekcija)),token[2],false,nullptr);
        Simboli.push_back(s);
        Rela r=Rela();
        string loc=decuHex(locationCounter);
        r=Rela(ind,trenutnaSekcija,token[2],loc,"R_X86_32",postoji(trenutnaSekcija),Simboli[k].value);
        int j=0;
        for(j=0;j<unapredtab.size();j++){
          if(unapredtab[j].imeSimbola==token[2]){
            unapredtab[j].ulazi.push_back(Unapred(ind++));break;
          }
        }
        if(j==unapredtab.size()){
          unapredtab.push_back(TabelaUnapred(token[2]));
          unapredtab[unapredtab.size()-1].ulazi.push_back(Unapred(ind++));
        }
        relokacioni.push_back(r);
      }
    for(int i=0;i<zapisi.size();i++){
      if(zapisi[i].nazivSekcije==trenutnaSekcija){
        zapisi[i].elementi.push_back("00");
        zapisi[i].elementi.push_back("00");
      }
    }
  }
      }      
    locationCounter+=2;
        }
    } 
    else if(adresiranje=="regdir"){
      for(int j=0;j<zapisi.size();j++){
        if(zapisi[j].nazivSekcije==trenutnaSekcija){
          zapisi[j].elementi.push_back("01");
        }
      }
      locationCounter+=1;
  } 
    else if(adresiranje=="regindbez"){
    for(int j=0;j<zapisi.size();j++){
      if(zapisi[j].nazivSekcije==trenutnaSekcija){
        zapisi[j].elementi.push_back("02");
        locationCounter+=1;
      }
    }
    
    }
    else if(adresiranje=="regindsa"){
    for(int j=0;j<zapisi.size();j++){
      if(zapisi[j].nazivSekcije==trenutnaSekcija){
        zapisi[j].elementi.push_back("03");
        locationCounter+=1;
      }
      string deci=token[3].substr(1,token[3].size()-2);
      if(literal(deci)){
        if(!(regex_match(deci, regex("^((0x([0-9]|[a-f]|[A-F])+))$")))){
          long dec=stoi(deci);
          string loc=decuHex(dec);
          while(loc.size()%4!=0){string dodaj="0"; dodaj+=loc; loc=dodaj;}
          kk=0;
          while(loc.size()>kk){
            string unos="";
            unos=unos+loc[kk];
            unos=unos+loc[kk+1];
            loc=loc.substr(kk+1,loc.size()-1);
            zapisi[j].elementi.push_back(unos);
            kk++;
            locationCounter+=1;
          }  
        }
        else{    
          deci=deci.substr(2,token[1].size()-2);                  
          while(deci.size()%4!=0){string dodaj="0"; dodaj+=deci; deci=dodaj;}
          if(deci.length()/2>2)
          kk=0;
          while(deci.size()>kk){
            string unos="";
            unos=unos+deci[kk];
            unos=unos+deci[kk+1];
            deci=deci.substr(kk+1,deci.size()-1);      
            zapisi[j].elementi.push_back(unos);
            kk++;
            locationCounter+=1;
          }  
        }
      }
      else{
        int k=-1;
        if(k=postoji(deci)!=(int)(-1)){
          k=postoji(deci);
          Rela r=Rela();int j=0;
          string loc=decuHex(locationCounter);
          if(Simboli[k].bind=="GLOB"){             
            r=Rela(ind++,trenutnaSekcija,deci,loc,"R_X86_32",Simboli[k].num,"0");
          }  
          else if(Simboli[k].bind=="LOC"){
            if(Simboli[k].defined==true){
              r=Rela(ind++,trenutnaSekcija,deci,loc,"R_X86_32",postoji(trenutnaSekcija),Simboli[k].value);
            }
            else{
              r=Rela(ind,trenutnaSekcija,deci,loc,"R_X86_32",postoji(trenutnaSekcija),Simboli[k].value);
              int j=0;
              for(j=0;j<unapredtab.size();j++){
                if(unapredtab[j].imeSimbola==deci){
                  unapredtab[j].ulazi.push_back(Unapred(ind++));break;
                }
              }
              if(j==unapredtab.size()){
                unapredtab.push_back(TabelaUnapred(deci));
                unapredtab[unapredtab.size()-1].ulazi.push_back(Unapred(ind++));
              }
            }
          }
          relokacioni.push_back(r);
        }
        else{
          Simbol s=Simbol();
          s.inic(brSimbola++,decuHex(locationCounter),0,"NOTYP","LOC",to_string(postoji(trenutnaSekcija)),deci,false,nullptr);
          Simboli.push_back(s);
          Rela r=Rela();
          string loc=decuHex(locationCounter);
          r=Rela(ind,trenutnaSekcija,deci,loc,"R_X86_32",postoji(trenutnaSekcija),Simboli[k].value);
          int j=0;
          for(j=0;j<unapredtab.size();j++){
            if(unapredtab[j].imeSimbola==deci){
              unapredtab[j].ulazi.push_back(Unapred(ind++));break;
            }
          }
          if(j==unapredtab.size()){
            unapredtab.push_back(TabelaUnapred(deci));
            unapredtab[unapredtab.size()-1].ulazi.push_back(Unapred(ind++));
          }
          relokacioni.push_back(r);
        }
        for(int i=0;i<zapisi.size();i++){
          if(zapisi[i].nazivSekcije==trenutnaSekcija){
            zapisi[i].elementi.push_back("00");
            zapisi[i].elementi.push_back("00");
            locationCounter+=2;
          }
        }
    }
    
    
    
  }
  }
  }
  else if(pushpop(linija)){
    if(token[0]=="push"){
      for(int i=0;i<zapisi.size();i++){
        if(zapisi[i].nazivSekcije==trenutnaSekcija){
          zapisi[i].elementi.push_back("B0");
          locationCounter+=1;
        }
        string pomocni=token[1].substr(1,1);
        pomocni+="6";
        zapisi[i].elementi.push_back(pomocni);
        zapisi[i].elementi.push_back("12");
        locationCounter+=2;
      }
    }
    else{
      for(int i=0;i<zapisi.size();i++){
        if(zapisi[i].nazivSekcije==trenutnaSekcija){
          zapisi[i].elementi.push_back("A0");
          locationCounter+=1;
        }
        string pomocni=token[1].substr(1,1);
        pomocni+="6";
        zapisi[i].elementi.push_back(pomocni);
        zapisi[i].elementi.push_back("42");
        locationCounter+=2;
      }
    }
  }
  }
  out<<"SHDR"<<'\n';
  out<< setfill(' ') <<setw(3)<<"NUM"<< setfill(' ') <<setw(15)<<"VALUE"
      << setfill(' ') <<setw(15)<<"SIZE"<< setfill(' ') <<setw(15)<<"NAME"<<'\n';
  for(int i=0;i<Simboli.size();i++){
    if(Simboli[i].type=="SCTN")
      out<< setfill(' ') <<setw(3)<<Simboli[i].num<< setfill(' ') <<setw(15)<<Simboli[i].value
      << setfill(' ') <<setw(15)<<Simboli[i].size<< setfill(' ') <<setw(15)<<Simboli[i].name<<'\n';
  }
  out<<"**********";

  out<<'\n';
 out<<"TABELASIMBOLA"<<'\n';
		out<< setfill(' ') <<setw(3)<< "NUM" <<setfill(' ')<<setw(15)<< "VALUE" <<setfill(' ')<<setw(15)<< "SIZE" <<setfill(' ')<<setw(15)
    << "TYPE"<<setfill(' ')<<setw(15)<< "BIND" <<setfill(' ')<<setw(15)<< "NDX" <<setfill(' ')<<setw(15)
    << "NAME" <<setfill(' ')<<setw(10)
    << "DEFINED" <<'\n';
  for (int i = 0;i < Simboli.size();i++) {
		out <<setfill(' ')<<setw(3)<< Simboli[i].num<<setfill(' ')<<setw(15)<< Simboli[i].value <<setfill(' ')<<setw(15)
    <<Simboli[i].size <<setfill(' ')<<setw(15)<<setfill(' ')<<Simboli[i].type
    <<setfill(' ')<<setw(15)<<Simboli[i].bind<<setfill(' ')<<setw(15)<<Simboli[i].ndx<<setfill(' ')<<setw(15)<<Simboli[i].name
    <<setfill(' ')<<setw(10)<<Simboli[i].defined<<'\n';
	}  
  out<<"**********";
  out<<'\n';
  out<<"TABELAREL"<<'\n';
  out<< setfill(' ') <<setw(10)<<"SEKCIJA"<< setfill(' ') <<setw(15)<<"OFFSET" << setfill(' ') <<setw(15)<<"TYPE"<< setfill(' ') <<setw(17)<<"SIMBOL"
  << setfill(' ') <<setw(15)<<"ADDEND"<<'\n';
  for (int i = 0;i < relokacioni.size();i++) {
    string k="";
    k+=to_string(relokacioni[i].symbol);
    k+="(";
    for(int p=0;p<Simboli.size();p++){
      if(Simboli[p].num==relokacioni[i].symbol)
        k+=Simboli[p].name;
    }
    k+=")";
    out<< setfill(' ') <<setw(10)<<relokacioni[i].sekcija
    << setfill(' ') <<setw(15)<<relokacioni[i].offset
    << setfill(' ') <<setw(15)<<relokacioni[i].type
    << setfill(' ') <<setw(17)<<k
    << setfill(' ') <<setw(15)<<relokacioni[i].addend
    <<'\n';
	}  
  out<<"**********";
  out<<'\n';
  for (int i = 0;i < zapisi.size();i++) {
    out<<"RELZ"<<'\n';
		out  << zapisi[i].nazivSekcije<< ' '<<zapisi[i].elementi.size()<<'\n';
    for(int j=0;j<zapisi[i].elementi.size();j++){
      out<<zapisi[i].elementi[j]<<' ';
    }
    out<<'\n';
    out<<'\n';
	}
  /*for(int i=0;i<unapredtab.size();i++){
    cout<<unapredtab[i].imeSimbola<<'\n';
    for(int j=0;j<unapredtab[i].ulazi.size();j++){
      cout<<unapredtab[i].ulazi[j].ulazRela<<' '<<unapredtab[i].ulazi[j].brSekcije;
    }
    cout<<'\n';
  }*/
}

Simbol* Asembler::pronadjiSimbol(string name){
  for(int i=0;i<Simboli.size();i++){
    if(Simboli[i].name==name)
      return &Simboli[i];
  }
  return nullptr;
}

string Asembler::decuHex(int locationCounter){
  long dec=locationCounter;
  int rem;                   
  string loc;
  int pw=1;
  while(pw*16<=dec) pw*=16;
  while (pw>0) {
    rem = dec / pw;
    dec%=pw;
    pw/=16;
    if (rem > 9) {
      switch (rem) {
        case 10: loc+="A"; break;
        case 11: loc+="B"; break;
        case 12: loc+="C"; break;
        case 13: loc+="D"; break;
        case 14: loc+="E"; break;
        case 15: loc+="F"; break;
        }
      }
    else {loc+=to_string(rem);}
  } 
  return loc;
}

string Asembler::operandizaskokove(string linija){
  if(linija[0]=='*' && linija[1]!='r' && linija[1]!='[') return "memorijsko";
  if(regex_match(linija,regex("^((\\w+)|((0x([0-9]|[a-f])+)|(([0-9])+)))*$")))
    return "neposredno";
  if(regex_match(linija,regex("^\\*r[0-7]$")))
    return "regdir";
  if(regex_match(linija,regex("^\\*\\[r[0-7]\\]$")))
    return "regindbez";
  if(regex_match(linija,regex("^\\*\\[r[0-7] *\\+ *(\\w+)\\]")))
    return "regindsa";
  if(regex_match(linija,regex("^\\%\\w+$")))
    return "pcrelativno";
  return "";
}

string Asembler::operandizaucitavanje(string linija){
  if(regex_match(linija,regex("^(((0x([0-9]|[a-f])+)|(([0-9])+)|(\\w+)))*$")))
    return "memorijsko";
  if(regex_match(linija,regex(("^\\$((\\w+)|((0x([0-9]|[a-f])+)|(([0-9])+)))$"))))
    return "neposredno";
  if(regex_match(linija,regex("^r[0-7]$")))
    return "regdir";
  if(regex_match(linija,regex("^\\[r[0-7]\\]$")))
    return "regindbez";
  if(regex_match(linija,regex(("^\\[r[0-7] *\\+ *((\\w+)|((0x([0-9]|[a-f])+)|(([0-9])+)))\\]$"))))
    return "regindsa";
  if(regex_match(linija,regex("^\\%\\w+$")))
    return "pcrelativno";
  return "";
}

bool Asembler::pushpop(string linija){
  return (regex_match(linija, regex("^(push|pop) +r[0-7] *$")));
}

bool Asembler::literal(string linija){
  return (regex_match(linija, regex("((0x([0-9]|[a-f]|[A-F])+)|(([0-9])+))$")));
}

bool Asembler::intm(string linija){
  return (regex_match(linija, regex("^int *r[0-7] *$")));
}

bool Asembler::call(string linija){
  return (regex_match(linija, regex("^call *.*$")));
}

bool Asembler::notm(string linija){
  return (regex_match(linija, regex("^not *r[0-7] *$")));
}

bool Asembler::section(string linija){
  regex sect("^\\.section +\\w+$");
  return regex_match(linija,sect);
}

bool Asembler::skok(string linija){
  regex sk("^(jmp|jeq|jne|jgt) +.*$");
  return regex_match(linija,sk);
}

bool Asembler::global(string linija){
  regex glob("^\\.global +(\\w+ *,* *)+$");
  return regex_match(linija,glob);
}

bool Asembler::jednobajtne(string linija){
  regex jednob("^(halt|iret|ret)$");
  return regex_match(linija,jednob);
}

bool Asembler::dvobajtne(string linija){
  regex dvob("^(xchg|add|sub|mul|div|cmp|and|or|xor|test|shl|shr) *r[0-7], *r[0-7]$");
  return regex_match(linija,dvob);
}

bool Asembler::externi(string linija){
  regex ext("^\\.extern +(\\w+ *,* *)+$");
  return regex_match(linija,ext);
}

bool Asembler::word(string linija){
  regex wor("^\\.word +(\\w+ *,* *)+$");
  return regex_match(linija,wor);
}

bool Asembler::ldst(string linija){
  regex ls("^(ldr|str) .*$");
  return regex_match(linija,ls);
}

bool Asembler::skip(string linija){
  regex ski("^\\.skip +((0x([0-9]|[a-f]|[A-F])+)|(([0-9])+))$");
  return regex_match(linija,ski);
}

bool Asembler::end(string linija){
  regex en("^\\.end");
  return regex_match(linija,en);
}

bool Asembler::labela(string linija){
  regex lin("^\\w+\\:.*$");
  return regex_match(linija,lin);
}

void Asembler::brisanjeKom(string &linija){
  	regex beliZnakovi("^\\s+ *.*$");
	  regex komentari("^.*#.*$");
  	while(regex_match(linija,beliZnakovi)){
    	linija=linija.substr(1,linija.length());
  }
    while(regex_match(linija,komentari)){
    	int str=linija.find('#');
    	linija=linija.substr(0,str);
  }
  while(linija[linija.size()-1]=='\t' || linija[linija.size()-1]==' '){
    linija=linija.substr(0,linija.size()-1);
  }
}


int Asembler::postoji(string s){
      for(int i=0;i<Simboli.size();i++){
        if(Simboli[i].name==s ) return i; 
      }
  return -1;
}

int Asembler::hexudec(string hexp) {
    int val = 0;
    const char *hex=hexp.c_str();
    while (*hex) {
        char byte = *hex++; 
        if (byte >= '0' && byte <= '9') byte = byte - '0';
        else if (byte >= 'a' && byte <='f') byte = byte - 'a' + 10;
        else if (byte >= 'A' && byte <='F') byte = byte - 'A' + 10;    
        val = (val << 4) | (byte & 0xF);
    }
    return val;
}

vector<string> Asembler::tokeni(string linija){
   vector<string> ret;  
    string word="";
    for(auto x:linija) {
     if(x!=' ' && x!='\t' && x!='\f' && x!='\n' && x!='\r' && x!=',' && x!=':' && x!='+' && x!='-') {
        word=word+x;     
      }
      else  if(x=='+' || x=='-') {
      if(word!="") {
           ret.push_back(word); 
           word="";       
           }
            word+=x;      
      }
      else if(x==':') {
         if(word!="") {
           word=word+x;
           ret.push_back(word);
         }  
          else {            
          if(ret.size()>0) ret[ret.size()-1]+=":";
        }
        word="";   
      }
      else {
        if(word=="+" || word=="-") continue;
        if(word!="")  ret.push_back(word); 
        word="";   
      }
    }
    if(word!="")
    ret.push_back(word);
   return ret;
}



int main(int argc, char* argv[]) {
	Asembler a=Asembler(argv[3],argv[2]);
	
}
