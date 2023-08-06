#include "../inc/linker.hpp"

Linker::Linker(string izlaznif){
  this->izlazniFajl=izlaznif;
  
	Sekcija sek=Sekcija("");
	sek.adresa="0";
	sek.duzina="0";
	sek.num=0;
	brsek++;
	sekcije.push_back(sek);
  Simbol sim= Simbol();
  sim.inic(0,"0",0,"","","","",false,nullptr);
  brSimbola++;
  velikatsimb.push_back(sim);
}

void Linker::dodaj(string ulazni){
  this->sviulaznif.push_back(ulazni);
	mapiranje(ulazni);
}

void Linker::mapiranje(string ulazni){
	ifstream in(ulazni);
	string linija;
	while(true){
    vector<string> token;
    if(in.eof()) break;
    getline(in,linija);
    brisanjeK(linija);
    token=tokeni(linija);
		if(token.size()>0){ 
			if(token[0]=="SHDR"){
				getline(in,linija);
        while(1){
          getline(in,linija);
          brisanjeK(linija);
          token=tokeni(linija);
          if(token[0]=="**********")break;
          int p;
          if((p=postojiSekcija(token[3]))==0){
            Sekcija s=Sekcija(token[3]);
            s.adresa=decuHex(loccnt);
            s.duzina=token[2];
            s.num=brsek++;
            loccnt+=stoi(token[2]);
            Pomocna p=Pomocna();
            p.indFajla=sviulaznif.size()-1;
            p.pocetnaadr=s.adresa;
            p.size=token[2];
            p.nazivSimb=token[3];
            pomocne.push_back(p);
            sekcije.push_back(s);
          }
          else{
            p=postojiSekcija(token[3]);
            for(int k=0;k<sekcije.size();k++){
              if(sekcije[k].num==p){
                int pov=stoi(token[2]);
                int pom=stoi(sekcije[k].duzina);
                sekcije[k].duzina=to_string(pov+pom);
                Pomocna p=Pomocna();
                p.indFajla=sviulaznif.size()-1;
                int ind;
                for(int c=0;c<pomocne.size();c++){
                  if(pomocne[c].nazivSimb==token[3] && pomocne[c].indFajla==(p.indFajla-1)){
                      ind=c+1;
                      int pom2=hexudec(pomocne[c].pocetnaadr);
                      int pov2=stoi(pomocne[c].size);
                      p.pocetnaadr=decuHex(pov2+pom2);
                  }
                }
                p.nazivSimb=token[3];
                p.size=token[2];
                vector<Pomocna>::iterator it=pomocne.begin();
                pomocne.insert(it+ind,p);
                loccnt+=stoi(token[2],nullptr,16);
              }
            }
            for(int k=p+1;k<sekcije.size();k++){
              int pov=stoi(token[2]);
              int pom=hexudec(sekcije[k].adresa);
              int sum=pov+pom;
              sekcije[k].adresa=decuHex(sum);
          }
          int t=0;
          for(int j=0;j<pomocne.size();j++){
                if(pomocne[j].nazivSimb==token[3] && pomocne[j].indFajla==sviulaznif.size()-1)
                 t=j;             
              }
              for(int j=t+1;j<pomocne.size();j++){               
                  int pov=stoi(token[2]);
                  int pom1=hexudec(pomocne[j].pocetnaadr);
                  int sum=pov+pom1;
                  if(postojiSekcija(pomocne[j].nazivSimb)!=0)
                  pomocne[j].pocetnaadr=decuHex(sum);
              }
          }
        }
			}
      else if(token[0]=="TABELASIMBOLA" ){
        getline(in,linija);
				getline(in,linija);
        while(1){
          getline(in,linija);
          brisanjeK(linija);
          token=tokeni(linija);
          if(token[0]=="**********") break;
          if(token.size()>7){
            int p;
            if((p=postojiSimbol(token[6]))==0){
              Simbol sim=Simbol();
              sim.inic(brSimbola++,token[1],stoi(token[2]),token[3],token[4],token[5],token[6],token[7]=="1"?1:0,nullptr);
              velikatsimb.push_back(sim);
            }
            else{
              p=postojiSimbol(token[6]);
              for(int k=0;k<velikatsimb.size();k++){
                if(velikatsimb[k].num==p){
                  if(velikatsimb[k].defined==true && token[7]=="1" && velikatsimb[k].ndx!="UND" && token[6]!="UND" && velikatsimb[k].ndx!=token[6])
                    cout<<"GRESKA! SIMBOL DEFINISAN VISE PUTA!";
              
                  if(token[7]!="0"){  
                    velikatsimb[k].ndx=token[5];
                    velikatsimb[k].defined=(token[7]=="1"?1:0);
                    velikatsimb[k].value=token[1];
                  }
                }
              }
            }
        }
        }
      }
      else if(token[0]=="TABELAREL"){
        getline(in,linija);
        relazapisi.push_back(to_string(sviulaznif.size()-1));
        while(1){
          getline(in,linija);
          brisanjeK(linija);
          token=tokeni(linija);
          if(token[0]=="**********")break;
          relazapisi.push_back(linija);
        }
      }
      else if(token[0]=="RELZ"){
        getline(in,linija);
        brisanjeK(linija);
        token=tokeni(linija);
        int p=postojiSekcija(token[0]);
        trenutnaSekcija=token[0];
        string adr="0";
        Pomocna pomocna=Pomocna();
        
        for(int i=0;i<velikatsimb.size();i++){
          if(velikatsimb[i].ndx==trenutnaSekcija ){
            for(int t=0;t<pomocne.size();t++){
              if(pomocne[t].nazivSimb==trenutnaSekcija && pomocne[t].indFajla==sviulaznif.size()-1)
                adr=pomocne[t].pocetnaadr;
            }
            pomocna.nazivSimb=velikatsimb[i].name;
            int adres=hexudec(adr);
            int off=hexudec(velikatsimb[i].value);
            pomocna.pocetnaadr=decuHex(adres+off);
            pomocna.size=to_string(velikatsimb[i].size);
            pomocna.indFajla=sviulaznif.size()-1;
            getline(in,linija);
            brisanjeK(linija);
            token=tokeni(linija);
            for(int t=0;t<pomocne.size();t++){            
              if(pomocne[t].nazivSimb==trenutnaSekcija){
                for(int l=0;l<token.size();l++){
                  pomocne[t].zapis.push_back(token[l]);
                }
              }
            }
            if(postojiPomocna(pomocna.nazivSimb)==0){
              pomocne.push_back(pomocna);
            }  
          }       
        } 
      }
		}
	}
}

void Linker::prepravkaTs(){
  string adresa;
  for(int i=0;i<pomocne.size();i++){
    Sadrzaj h=Sadrzaj();
    if(pomocne[i].zapis.size()>0){
      for(int j=0;j<pomocne[i].zapis.size();j++){
        if(pomocne[i].nazivSimb==pomocne[i-1].nazivSimb) break;
        if(j==0) {
          adresa=pomocne[i].pocetnaadr;
          h.adresa=adresa;
          h.sadrzaj=pomocne[i].zapis[j];
        }
        else {
          int pov=hexudec(adresa);
          string ad=decuHex(pov+1);
          adresa=ad;
          h.adresa=ad;
          h.sadrzaj=pomocne[i].zapis[j];
        }
        h.simbol=pomocne[i].nazivSimb;
        izlaz.push_back(h);
          }    }
          
  }
 for(int i=0;i<velikatsimb.size();i++){
    for(int j=0;j<sekcije.size();j++){
      if(velikatsimb[i].name==sekcije[j].naziv ){
        velikatsimb[i].value=sekcije[j].adresa;
        velikatsimb[i].size=stoi(sekcije[j].duzina);
      }
    }
    for(int j=0;j<pomocne.size();j++){
      if(velikatsimb[i].name==pomocne[j].nazivSimb && velikatsimb[i].type!="SCTN"){
        velikatsimb[i].value=pomocne[j].pocetnaadr;
        velikatsimb[i].size=stoi(pomocne[j].size);
      }
    }
  }

}

void Linker::ispis(){
  ofstream out(izlazniFajl);
  /*cout<<"SVE SEKCIJE"<<'\n';
  for(int k=0;k<sekcije.size();k++){
		cout<<"num: "<<sekcije[k].num<<" adr: "<<sekcije[k].adresa<<" size: "<<sekcije[k].duzina<<" name: "<<sekcije[k].naziv<<'\n';
	}	*/
/* cout<<'\n';
  cout<<"TABELASIMBOLA"<<'\n';
		cout<< setfill(' ') <<setw(3)<< "NUM" <<setfill(' ')<<setw(15)<< "VALUE" <<setfill(' ')<<setw(15)<< "SIZE" <<setfill(' ')<<setw(15)
    << "TYPE"<<setfill(' ')<<setw(15)<< "BIND" <<setfill(' ')<<setw(15)<< "NDX" <<setfill(' ')<<setw(15)
    << "NAME" <<setfill(' ')<<setw(10)
    << "DEFINED" <<'\n';
  for (int i = 0;i <velikatsimb.size();i++) {
		cout <<setfill(' ')<<setw(3)<< velikatsimb[i].num<<setfill(' ')<<setw(15)<< velikatsimb[i].value <<setfill(' ')<<setw(15)
    <<decuHex(velikatsimb[i].size) <<setfill(' ')<<setw(15)<<setfill(' ')<<velikatsimb[i].type
    <<setfill(' ')<<setw(15)<<velikatsimb[i].bind<<setfill(' ')<<setw(15)<<velikatsimb[i].ndx<<setfill(' ')<<setw(15)<<velikatsimb[i].name
    <<setfill(' ')<<setw(10)<<velikatsimb[i].defined<<'\n';
	}  	*/
  /*cout<<'\n';
  cout<<"Pomocna"<<'\n';
  cout<<"naziv: "<<" ind: "<<" pocadr: "<<" size: "<<'\n';
  for(int k=0;k<pomocne.size();k++){
		cout<<pomocne[k].nazivSimb<<' '<<pomocne[k].indFajla<<' '<<pomocne[k].pocetnaadr<<' '<<pomocne[k].size<<'\n';
	}*/
  /*cout<<'\n';
  cout<<"Zapisi"<<'\n';
  for(int k=0;k<pomocne.size();k++){
    if(postojiSekcija(pomocne[k].nazivSimb)){
		  cout<<pomocne[k].nazivSimb<<' '<<pomocne[k].pocetnaadr<<'\n';
      for(int s=0;s<pomocne[k].zapis.size();s++){
        cout<<pomocne[k].zapis[s]<<' ';
      }
      cout<<'\n';
    }
	}*/
  //cout<<'\n';
  //cout<<"Izlaz"<<'\n';
  int po=8;
  string str;
  str=izlaz[0].adresa;
  while(str.size()<4){
    string nul="0";
    str=nul+str;
  }
  out<<str<<':';
  str="";
  for(int k=0;k<izlaz.size();k++){
    if(po==0) {
      out<<'\n';
      str=izlaz[k].adresa;
      while(str.size()<4){
        string nul="0";
        str=nul+str;
      }
      out<<str<<':';
      po=8;
      
    }
      out<<' '<<izlaz[k].sadrzaj;
      po--;
	}
  
}

int Linker::hexudec(string hexp) {
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

string Linker::decuHex(int locationCounter){
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

void Linker::brisanjeK(string &linija){
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

vector<string> Linker::tokeni(string linija){
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

void Linker::razresavanje(){
  int trfajl;
  string S,P,A;
  vector<string> token;
  for(int i=0;i<relazapisi.size();i++){
    token=tokeni(relazapisi[i]);
    if(token.size()==1) trfajl=stoi(token[0]);
    else{
      for(int t=0;t<pomocne.size();t++){
        if(pomocne[t].nazivSimb==token[0] && pomocne[t].indFajla==trfajl){
          P=decuHex(hexudec(pomocne[t].pocetnaadr)+hexudec(token[1]));
          A=(token[4]);
          int ind=token[3].find('(');
          string trazi=token[3].substr(ind+1,token[3].size()-ind-2);
          for(int f=0;f<velikatsimb.size();f++){
            if(velikatsimb[f].name==trazi)
              S=velikatsimb[f].value;
          }
          if(token[2]=="R_X86_32"){
            for(int pomoc=0;pomoc<izlaz.size();pomoc++){
              if(izlaz[pomoc].adresa==P){
                int pp1=hexudec(S);
                int pp2=hexudec(A);
                int proba=pp1+pp2;
                if(proba>=256*256) proba=proba-256*256;
                if(proba<0)proba=256*256+proba;
                string sad=decuHex(pp1+pp2);
                string str=sad;
                while(str.size()<4){
                  string nul="0";
                  str=nul+str;
                }
                sad=str;
                izlaz[pomoc].sadrzaj=sad.substr(0,2);
                izlaz[pomoc+1].sadrzaj=sad.substr(2,2);
                
              }
            }
          }
          else if(token[2]=="R_X86_PLT16"){
            for(int pomoc=0;pomoc<izlaz.size();pomoc++){
              if(izlaz[pomoc].adresa==P){
                int pp1=hexudec(S);
                int pp2=hexudec(A);
                int proba=pp1+pp2;
                if(proba>=256*256) proba=proba-256*256;
                if(proba<0)proba=256*256+proba;
                string sad=decuHex(pp1+pp2);
                string str=sad;
                while(str.size()<4){
                  string nul="0";
                  str=nul+str;
                }
                sad=str;
             //   if(izlaz[pomoc].sadrzaj=="3E" || izlaz[pomoc+1].sadrzaj=="3E") cout<<izlaz[pomoc].simbol<<'*'<<'\n';
                izlaz[pomoc].sadrzaj=sad.substr(2,2);
                izlaz[pomoc+1].sadrzaj=sad.substr(0,2);
              }
            }
          }
          else if(token[2]=="R_X86_PC16"){
            for(int pomoc=0;pomoc<izlaz.size();pomoc++){
              if(izlaz[pomoc].adresa==P){
                int pp1=hexudec(S);
                int pp2=hexudec(A);
                int pp3=hexudec(P);
                int proba=pp1+pp2-pp3;
                if(proba<0) proba=256*256+proba;
                if(proba>=256*256) proba=proba-256*256;
                string sad=decuHex(proba);
                string str=sad;
                while(str.size()<4){
                  string nul="0";
                  str=nul+str;
                }
                sad=str;
               // if(izlaz[pomoc].sadrzaj=="3E" || izlaz[pomoc+1].sadrzaj=="3E") cout<<izlaz[pomoc].simbol<<'*'<<'\n';
                izlaz[pomoc].sadrzaj=sad.substr(0,2);
                izlaz[pomoc+1].sadrzaj=sad.substr(2,2);
              }
            }
          }
        }
      }
    }
  }
  relazapisi.clear();
}

int Linker::postojiSekcija(string naziv){
	for(int i=0;i<sekcije.size();i++){
		if(sekcije[i].naziv==naziv)
			return sekcije[i].num;
	}
	return 0;
}

int Linker::postojiSimbol(string naziv){
  for(int i=0;i<velikatsimb.size() ;i++){
		if(velikatsimb[i].name==naziv)
			return velikatsimb[i].num;
	}
	return 0;
}

int Linker::postojiPomocna(string naziv){
  for(int i=0;i<pomocne.size() ;i++){
		if(pomocne[i].nazivSimb==naziv)
			return 1;
	}
	return 0;
}

int main(int argc, char* argv[]) {
	Linker l=Linker(argv[3]);
	
	for(int i=4;i<argc;i++){
		l.dodaj(argv[i]);
	}
  l.prepravkaTs();
  l.razresavanje();
  l.ispis();
}