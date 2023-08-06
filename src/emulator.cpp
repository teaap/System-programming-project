#include "../inc/emulator.hpp"

void Emulator::citanjeizfajla(){
	ifstream in(ulazni);
	int cnt=0;
	while(1){
		string linija;
		vector<string> token;
		if(in.eof()) break;
    getline(in,linija);
    token=tokeni(linija); 
		if(cnt<2){
			int cnt1=0;
			while(cnt1<token.size()-1){
				ivt.push_back(Memorija(prosirido4(decuHex(stoi(token[0],nullptr,16)+cnt1)),token[cnt1+1]));cnt1++;
			}
		}
    int cnt2=0;
		while(cnt2<token.size()-1){
			memorija.push_back(Memorija(prosirido4(decuHex(stoi(token[0],nullptr,16)+cnt2)),token[cnt2+1]));cnt2++;
		}
    cnt++;
	}
}

void Emulator::izvrsi(){
  r[7]=sadrzaj2bajta("0000");
  if(r[7]=="Greska") return;
  while(1){
    kod=sadrzajbajt(r[7]);
    if(kod=="Greska") {return;}
    if(kod=="00"){
      r[7]=decuHex(stoi(r[7],nullptr,16)+1);
      r[7]=prosirido4(r[7]);
      return;
    }
    else if(kod=="10"){
      r[7]=decuHex(stoi(r[7],nullptr,16)+1);
      r[7]=prosirido4(r[7]);
      string sadrzaj=sadrzajbajt(r[7]);
      if(sadrzaj=="Greska") return;
      r[7]=decuHex(stoi(r[7],nullptr,16)+1);
      r[7]=prosirido4(r[7]);
      string ps=pswUString();
      stavinastek(r[7].substr(0,2));
      stavinastek(r[7].substr(2,2));
      stavinastek(ps.substr(0,2));
      stavinastek(ps.substr(2,2));
      string broj=sadrzaj.substr(0,1);
      string ul=decuHex((stoi(r[stoi(broj)])%8)*2);
      ul=prosirido4(ul);
      r[7]=sadrzaj2bajta(ul);
    }
    else if(kod=="20"){
      string prvi=skinisasteka();
      string drugi=skinisasteka();
      stringUPSW(drugi+prvi);
      prvi=skinisasteka();
      drugi=skinisasteka();
      r[7]=drugi+prvi;
    }
    else if(kod=="40"){
      string prvi,drugi;
      prvi=skinisasteka();
      drugi=skinisasteka();
      r[7]=drugi+prvi;
    }
    else if(kod=="60"){
      r[7]=decuHex(stoi(r[7],nullptr,16)+1);
      r[7]=prosirido4(r[7]);
      string tmp=sadrzajbajt(r[7]).substr(0,1);
      tmp=r[stoi(tmp)];
      r[stoi(sadrzajbajt(r[7]).substr(0,1))]=r[stoi(sadrzajbajt(r[7]).substr(1,1))];
      r[stoi(sadrzajbajt(r[7]).substr(1,1))]=tmp;
      r[7]=decuHex(stoi(r[7],nullptr,16)+1);
      r[7]=prosirido4(r[7]);
    }
    else if(kod=="70"){
      r[7]=decuHex(stoi(r[7],nullptr,16)+1);
      r[7]=prosirido4(r[7]);
      int prvi=stoi(r[stoi(sadrzajbajt(r[7]).substr(0,1))],nullptr,16);
      int drugi=stoi(r[stoi(sadrzajbajt(r[7]).substr(1,1))],nullptr,16);
      int rez=prvi+drugi;
      r[stoi(sadrzajbajt(r[7]).substr(0,1))]=decuHex(rez);
      r[stoi(sadrzajbajt(r[7]).substr(0,1))]=prosirido4(r[stoi(sadrzajbajt(r[7]).substr(0,1))]);
      r[7]=decuHex(stoi(r[7],nullptr,16)+1);
      r[7]=prosirido4(r[7]);
    }
    else if(kod=="71"){
      r[7]=decuHex(stoi(r[7],nullptr,16)+1);
      r[7]=prosirido4(r[7]);
      r[stoi(sadrzajbajt(r[7]).substr(0,1))]=decuHex(stoi(r[stoi(sadrzajbajt(r[7]).substr(0,1))],nullptr,16)-stoi(r[stoi(sadrzajbajt(r[7]).substr(1,1))],nullptr,16));
      r[stoi(sadrzajbajt(r[7]).substr(0,1))]=prosirido4(r[stoi(sadrzajbajt(r[7]).substr(0,1))]);
      r[7]=decuHex(stoi(r[7],nullptr,16)+1);
      r[7]=prosirido4(r[7]);
    }
    else if(kod=="72"){
      r[7]=decuHex(stoi(r[7],nullptr,16)+1);
      r[7]=prosirido4(r[7]);
      r[stoi(sadrzajbajt(r[7]).substr(0,1))]=decuHex(stoi(r[stoi(sadrzajbajt(r[7]).substr(0,1))],nullptr,16)*stoi(r[stoi(sadrzajbajt(r[7]).substr(1,1))],nullptr,16));
      r[stoi(sadrzajbajt(r[7]).substr(0,1))]=prosirido4(r[stoi(sadrzajbajt(r[7]).substr(0,1))]);
      r[7]=decuHex(stoi(r[7],nullptr,16)+1);
      r[7]=prosirido4(r[7]);
    }
    else if(kod=="73"){
      r[7]=decuHex(stoi(r[7],nullptr,16)+1);
      r[7]=prosirido4(r[7]);
      int prvi=stoi(r[stoi(sadrzajbajt(r[7]).substr(0,1))],nullptr,16);
      int drugi=stoi(r[stoi(sadrzajbajt(r[7]).substr(1,1))],nullptr,16);
      int rez=prvi/drugi;
      string konacno=decuHex(rez);
      konacno=prosirido4(konacno);
      r[stoi(sadrzajbajt(r[7]).substr(0,1))]=konacno;
      r[stoi(sadrzajbajt(r[7]).substr(0,1))]=prosirido4(r[stoi(sadrzajbajt(r[7]).substr(0,1))]);
      r[7]=decuHex(stoi(r[7],nullptr,16)+1);
      r[7]=prosirido4(r[7]);
    }
    else if(kod=="74"){
      r[7]=decuHex(stoi(r[7],nullptr,16)+1);
      r[7]=prosirido4(r[7]);
      int temp1=stoi(r[stoi(sadrzajbajt(r[7]).substr(0,1))],nullptr,16);
      int temp2=stoi(r[stoi(sadrzajbajt(r[7]).substr(1,1))],nullptr,16);
      int temp=temp1-temp2;
      azurirajPSW(temp1,temp2,temp);
      r[7]=decuHex(stoi(r[7],nullptr,16)+1);
      r[7]=prosirido4(r[7]);
    }
    else if(kod=="80"){
      r[7]=decuHex(stoi(r[7],nullptr,16)+1);
      r[7]=prosirido4(r[7]);
      int ind=stoi(sadrzajbajt(r[7]).substr(0,1));    
      int vr=~stoi(r[ind],nullptr,16);
      if(vr<0) vr+=256*256;
     
      r[ind]=decuHex(vr);
      r[ind]=prosirido4(r[ind]);
      r[7]=decuHex(stoi(r[7],nullptr,16)+1);
      r[7]=prosirido4(r[7]);
    }
    else if(kod=="81"){
      r[7]=decuHex(stoi(r[7],nullptr,16)+1);
      r[7]=prosirido4(r[7]);
      int vr=stoi(r[stoi(sadrzajbajt(r[7]).substr(0,1))],nullptr,16) & 
                                              stoi(r[stoi(sadrzajbajt(r[7]).substr(1,1))],nullptr,16);
      if(vr<0) vr+=256*256;
      r[stoi(sadrzajbajt(r[7]).substr(0,1))]=decuHex(vr);
      r[stoi(sadrzajbajt(r[7]).substr(0,1))]=prosirido4(r[stoi(sadrzajbajt(r[7]).substr(0,1))]);
      r[7]=decuHex(stoi(r[7],nullptr,16)+1);
      r[7]=prosirido4(r[7]);
    }
    else if(kod=="82"){
      r[7]=decuHex(stoi(r[7],nullptr,16)+1);
      r[7]=prosirido4(r[7]);
      int vr=stoi(r[stoi(sadrzajbajt(r[7]).substr(0,1))],nullptr,16)|stoi(r[stoi(sadrzajbajt(r[7]).substr(1,1))],nullptr,16);
      r[stoi(sadrzajbajt(r[7]).substr(0,1))]=decuHex(vr);
      r[stoi(sadrzajbajt(r[7]).substr(0,1))]=prosirido4(r[stoi(sadrzajbajt(r[7]).substr(0,1))]);
      r[7]=decuHex(stoi(r[7],nullptr,16)+1);
      r[7]=prosirido4(r[7]);
    }
    else if(kod=="83"){
      r[7]=decuHex(stoi(r[7],nullptr,16)+1);
      r[7]=prosirido4(r[7]);
      int vr=stoi(r[stoi(sadrzajbajt(r[7]).substr(0,1))],nullptr,16) ^ 
                                              stoi(r[stoi(sadrzajbajt(r[7]).substr(1,1))],nullptr,16);
      if(vr<0) vr+=256;
      r[stoi(sadrzajbajt(r[7]).substr(0,1))]=decuHex(vr);
      r[stoi(sadrzajbajt(r[7]).substr(0,1))]=prosirido4(r[stoi(sadrzajbajt(r[7]).substr(0,1))]);
      r[7]=decuHex(stoi(r[7],nullptr,16)+1);
      r[7]=prosirido4(r[7]);
    }
    else if(kod=="84"){
      r[7]=decuHex(stoi(r[7],nullptr,16)+1);
      r[7]=prosirido4(r[7]);
      int vr=stoi(r[stoi(sadrzajbajt(r[7]).substr(0,1))],nullptr,16) & 
                                              stoi(r[stoi(sadrzajbajt(r[7]).substr(1,1))],nullptr,16);
      if(vr>=4096) psw[3]=1;
      if(vr==0) psw[0]=1;
      r[7]=decuHex(stoi(r[7],nullptr,16)+1);
      r[7]=prosirido4(r[7]);
    }
    else if(kod=="90"){
      r[7]=decuHex(stoi(r[7],nullptr,16)+1);
      r[7]=prosirido4(r[7]);
      int broj=stoi(r[stoi(sadrzajbajt(r[7]).substr(0,1))],nullptr,16)<<(stoi(r[stoi(sadrzajbajt(r[7]).substr(1,1))],nullptr,16)-1);
      if(broj>=32768) {psw[2]=1;}
      broj=broj<<1;
      if(broj>=256*256) {string br=decuHex(broj); broj=stoi(br.substr(1,br.size()-1),nullptr,16);}
      if(broj==0) psw[0]=1;
      r[stoi(sadrzajbajt(r[7]).substr(0,1))]=decuHex(broj);
      r[stoi(sadrzajbajt(r[7]).substr(0,1))]=prosirido4(r[stoi(sadrzajbajt(r[7]).substr(0,1))]);
      r[7]=decuHex(stoi(r[7],nullptr,16)+1);
      r[7]=prosirido4(r[7]);
    }
    else if(kod=="91"){
      r[7]=decuHex(stoi(r[7],nullptr,16)+1);
      r[7]=prosirido4(r[7]);
      int broj=stoi(r[stoi(sadrzajbajt(r[7]).substr(0,1))],nullptr,16)>>(stoi(r[stoi(sadrzajbajt(r[7]).substr(1,1))],nullptr,16)-1);
      if(broj%2==1) {psw[2]=1;}
      broj=broj>>1;
      if(broj==0) psw[0]=1;
      r[stoi(sadrzajbajt(r[7]).substr(0,1))]=decuHex(broj);
      r[stoi(sadrzajbajt(r[7]).substr(0,1))]=prosirido4(r[stoi(sadrzajbajt(r[7]).substr(0,1))]);
      r[7]=decuHex(stoi(r[7],nullptr,16)+1);
      r[7]=prosirido4(r[7]);
    }
    else if(kod=="30"){
      string operand=operandpc();
      r[7]=operand;
    }
    else if(kod=="50"){
      string operand=operandpc();
      r[7]=operand;
    }
    else if(kod=="51"){
      if(psw[1]==1){
        string operand=operandpc();
        r[7]=operand;
      }
      else{
        r[7]=decuHex(stoi(r[7],nullptr,16)+1);
        r[7]=prosirido4(r[7]);
      }
    }
    else if(kod=="52"){
      if(psw[1]==0){
        string operand=operandpc();
        r[7]=operand;
      }
      else{
        r[7]=decuHex(stoi(r[7],nullptr,16)+1);
        r[7]=prosirido4(r[7]);
      }
    }
    else if(kod=="53"){
      if(psw[3]==0){
        string operand=operandpc();
        r[7]=operand;
      }
      else{
        r[7]=decuHex(stoi(r[7],nullptr,16)+1);
        r[7]=prosirido4(r[7]);
      }
    }
    else if(kod=="A0"){
      string operand=operandpc();
      r[d]=operand;
      r[d]=prosirido4(r[d]);
      r[7]=decuHex(stoi(r[7],nullptr,16)+1);
      r[7]=prosirido4(r[7]);
    }
    else if(kod=="B0"){
      string operand=operandpcB0();
      r[d]=prosirido4(r[d]);
      
      if(stoi((operand),nullptr,16)+1>=256*256) operand=decuHex(1+stoi((operand),nullptr,16)-256*256);
      else if(1+stoi((operand),nullptr,16)<0) operand=decuHex(1+stoi((operand),nullptr,16)+256*256);
      else operand=decuHex(1+stoi((operand),nullptr,16));
      operand=prosirido4(operand);
      upisiumem(operand,r[d].substr(0,2));
      operand=decuHex(stoi((operand),nullptr,16)-1);
      operand=prosirido4(operand);
      upisiumem(operand,r[d].substr(2,2));
      r[7]=decuHex(stoi(r[7],nullptr,16)+1);
      r[7]=prosirido4(r[7]);
    }
    else return;
  } 

}

string Emulator::operandpc(){
  string operand,novi;
      r[7]=decuHex(stoi(r[7],nullptr,16)+1);
      r[7]=prosirido4(r[7]);
      string sadr=sadrzajbajt(r[7]);
      ind=stoi(sadr.substr(1,1),nullptr,16);
      d=stoi(sadr.substr(0,1),nullptr,16);
      r[7]=decuHex(stoi(r[7],nullptr,16)+1);
      r[7]=prosirido4(r[7]);
      sadr=sadrzajbajt(r[7]);
      azurirajReg(ind,stoi(sadr.substr(0,1),nullptr,16));
      int adr=stoi(sadr.substr(1,1),nullptr,16);
      switch(adr){
        case 0:{
          string prvi,drugi;         
          r[7]=decuHex(stoi(r[7],nullptr,16)+1);
          r[7]=prosirido4(r[7]);
          prvi=sadrzajbajt(r[7]);
          r[7]=decuHex(stoi(r[7],nullptr,16)+1);
          r[7]=prosirido4(r[7]);
          drugi=sadrzajbajt(r[7]);
          novi=prvi+drugi;
        }break;
        case 1:{
          novi=r[ind];
        }break;
        case 5:{
          string prvi,drugi;
          r[7]=decuHex(stoi(r[7],nullptr,16)+1);
          r[7]=prosirido4(r[7]);
          prvi=sadrzajbajt(r[7]);
          r[7]=decuHex(stoi(r[7],nullptr,16)+1);
          r[7]=prosirido4(r[7]);
          drugi=sadrzajbajt(r[7]);
          int pomeraj=stoi(r[ind],nullptr,16);
          if(ind==7)pomeraj+=1;
          if(pomeraj+stoi((prvi+drugi),nullptr,16)>=256*256) novi=decuHex(pomeraj+stoi((prvi+drugi),nullptr,16)-256*256);
          else if(pomeraj+stoi((prvi+drugi),nullptr,16)<0) novi=decuHex(pomeraj+stoi((prvi+drugi),nullptr,16)+256*256);
          else novi=decuHex(pomeraj+stoi((prvi+drugi),nullptr,16));
          novi=prosirido4(novi);
        }break;
        case 2:{
          string pomocni=r[ind];
          novi=r[ind];          
          if(ind==6) novi=sadrzaj2stek(novi);
          novi=sadrzaj2bajta(novi);
          novi=prosirido4(novi);
        }break;
        case 3:{
          string prvi,drugi;
          r[7]=decuHex(stoi(r[7],nullptr,16)+1);
          r[7]=prosirido4(r[7]);
          prvi=sadrzajbajt(r[7]);
          r[7]=decuHex(stoi(r[7],nullptr,16)+1);
          r[7]=prosirido4(r[7]);
          drugi=sadrzajbajt(r[7]);
          int pomeraj=stoi((r[ind]),nullptr,16);
          if(pomeraj+stoi((prvi+drugi),nullptr,16)>=256*256) novi=decuHex(pomeraj+stoi((prvi+drugi),nullptr,16)-256*256);
          else if(pomeraj+stoi((prvi+drugi),nullptr,16)<0) novi=decuHex(pomeraj+stoi((prvi+drugi),nullptr,16)+256*256);
          else novi=decuHex(pomeraj+stoi((prvi+drugi),nullptr,16));
          novi=prosirido4(novi);
          if(ind==6) novi=sadrzaj2stek(novi);
          else novi=sadrzaj2bajta(novi);
        }break;
        case 4:{
          string prvi,drugi;
          r[7]=decuHex(stoi(r[7],nullptr,16)+1);
          r[7]=prosirido4(r[7]);
          prvi=sadrzajbajt(r[7]);
          r[7]=decuHex(stoi(r[7],nullptr,16)+1);
          r[7]=prosirido4(r[7]);
          drugi=sadrzajbajt(r[7]);
          novi=prvi+drugi;
          novi=prosirido4(novi);
          if(ind==6) novi=sadrzaj2stek(novi);
          else novi=sadrzaj2bajta(novi);
        }break;
      }
      if(kod=="30"){
        r[7]=decuHex(stoi(r[7],nullptr,16)+1);
        r[7]=prosirido4(r[7]);
        stavinastek(r[7].substr(0,2));
        stavinastek(r[7].substr(2,2));
      }
      if(umanjiza2){
        umanjiza2=false;
        if(stoi(r[ind],nullptr,16)-2<0) r[ind]=decuHex(stoi(r[ind],nullptr,16)-2+256*256);
        else if(stoi(r[ind],nullptr,16)-2>=256*256) r[ind]=decuHex(stoi(r[ind],nullptr,16)-2-256*256);
        else r[ind]=decuHex(stoi(r[ind],nullptr,16)-2);
        r[ind]=prosirido4(r[ind]);
      }
      if(uvecajza2){
        uvecajza2=false;
        if(stoi(r[ind],nullptr,16)+2<0) r[ind]=decuHex(stoi(r[ind],nullptr,16)+2+256*256);
        else if(stoi(r[ind],nullptr,16)+2>=256*256) r[ind]=decuHex(stoi(r[ind],nullptr,16)+2-256*256);
        else r[ind]=decuHex(stoi(r[ind],nullptr,16)+2);
        r[ind]=prosirido4(r[ind]);
      }
      return novi;
}

string Emulator::operandpcB0(){
  string operand,novi;
      r[7]=decuHex(stoi(r[7],nullptr,16)+1);
      r[7]=prosirido4(r[7]);
      string sadr=sadrzajbajt(r[7]);
      ind=stoi(sadr.substr(1,1),nullptr,16);
      d=stoi(sadr.substr(0,1),nullptr,16);
      r[7]=decuHex(stoi(r[7],nullptr,16)+1);
      r[7]=prosirido4(r[7]);
      sadr=sadrzajbajt(r[7]);
      azurirajReg(ind,stoi(sadr.substr(0,1),nullptr,16));
      int adr=stoi(sadr.substr(1,1),nullptr,16);
      switch(adr){
        case 0:{
          string prvi,drugi;         
          r[7]=decuHex(stoi(r[7],nullptr,16)+1);
          r[7]=prosirido4(r[7]);
          prvi=sadrzajbajt(r[7]);
          r[7]=decuHex(stoi(r[7],nullptr,16)+1);
          r[7]=prosirido4(r[7]);
          drugi=sadrzajbajt(r[7]);
          novi=prvi+drugi;
        }break;
        case 1:{
          novi=r[ind];
        }break;
        case 5:{
          string prvi,drugi;
          r[7]=decuHex(stoi(r[7],nullptr,16)+1);
          r[7]=prosirido4(r[7]);
          prvi=sadrzajbajt(r[7]);
          r[7]=decuHex(stoi(r[7],nullptr,16)+1);
          r[7]=prosirido4(r[7]);
          drugi=sadrzajbajt(r[7]);
          int pomeraj=stoi(r[ind],nullptr,16);
          if(pomeraj+stoi((prvi+drugi),nullptr,16)>=256*256) novi=decuHex(pomeraj+stoi((prvi+drugi),nullptr,16)-256*256);
          else if(pomeraj+stoi((prvi+drugi),nullptr,16)<0) novi=decuHex(pomeraj+stoi((prvi+drugi),nullptr,16)+256*256);
          else novi=decuHex(pomeraj+stoi((prvi+drugi),nullptr,16));
          novi=prosirido4(novi);
        }break;
        case 2:{
          novi=r[ind];
          //novi=sadrzaj2bajta(pomocni);
          novi=prosirido4(novi);
        }break;
        case 3:{
          string prvi,drugi;
          r[7]=decuHex(stoi(r[7],nullptr,16)+1);
          r[7]=prosirido4(r[7]);
          prvi=sadrzajbajt(r[7]);
          r[7]=decuHex(stoi(r[7],nullptr,16)+1);
          r[7]=prosirido4(r[7]);
          drugi=sadrzajbajt(r[7]);
          int pomeraj=stoi(r[ind],nullptr,16);
          if(pomeraj+stoi((prvi+drugi),nullptr,16)>=256*256) novi=decuHex(pomeraj+stoi((prvi+drugi),nullptr,16)-256*256);
          else if(pomeraj+stoi((prvi+drugi),nullptr,16)<0) novi=decuHex(pomeraj+stoi((prvi+drugi),nullptr,16)+256*256);
          else novi=decuHex(pomeraj+stoi((prvi+drugi),nullptr,16));
          novi=prosirido4(novi);
          novi=sadrzaj2bajta(novi);
        }break;
        case 4:{
          string prvi,drugi;
          r[7]=decuHex(stoi(r[7],nullptr,16)+1);
          r[7]=prosirido4(r[7]);
          prvi=sadrzajbajt(r[7]);
          r[7]=decuHex(stoi(r[7],nullptr,16)+1);
          r[7]=prosirido4(r[7]);
          drugi=sadrzajbajt(r[7]);
          novi=prvi+drugi;
          novi=prosirido4(novi);
         // novi=sadrzaj2bajta(novi);
        }break;
      }
      if(kod=="30"){
        r[7]=decuHex(stoi(r[7],nullptr,16)+1);
        r[7]=prosirido4(r[7]);
        stavinastek(r[7].substr(0,2));
        stavinastek(r[7].substr(2,2));
      }
      if(umanjiza2){
        umanjiza2=false;
        if(stoi(r[ind],nullptr,16)-2<0) r[ind]=decuHex(stoi(r[ind],nullptr,16)-2+256*256);
        else if(stoi(r[ind],nullptr,16)-2>=256*256) r[ind]=decuHex(stoi(r[ind],nullptr,16)-2-256*256);
        else r[ind]=decuHex(stoi(r[ind],nullptr,16)-2);
        r[ind]=prosirido4(r[ind]);
      }
      if(uvecajza2){
        uvecajza2=false;
        if(stoi(r[ind],nullptr,16)+2<0) r[ind]=decuHex(stoi(r[ind],nullptr,16)+2+256*256);
        else if(stoi(r[ind],nullptr,16)+2>=256*256) r[ind]=decuHex(stoi(r[ind],nullptr,16)+2-256*256);
        else r[ind]=decuHex(stoi(r[ind],nullptr,16)+2);
        r[ind]=prosirido4(r[ind]);
      }
      return novi;
}


void Emulator::azurirajReg(int ind,int upmod){
  switch(upmod){
    case 0: break;
    case 1: {
      if(stoi(r[ind],nullptr,16)-2<0) r[ind]=decuHex(stoi(r[ind],nullptr,16)-2+256*256);
      else if(stoi(r[ind],nullptr,16)-2>=256*256) r[ind]=decuHex(stoi(r[ind],nullptr,16)-2-256*256);
      else r[ind]=decuHex(stoi(r[ind],nullptr,16)-2);
      r[ind]=prosirido4(r[ind]);
    }break;
    case 2:{
      if(stoi(r[ind],nullptr,16)+2<0) r[ind]=decuHex(stoi(r[ind],nullptr,16)+2+256*256);
      else if(stoi(r[ind],nullptr,16)+2>=256*256) r[ind]=decuHex(stoi(r[ind],nullptr,16)+2-256*256);
      else r[ind]=decuHex(stoi(r[ind],nullptr,16)+2);
      r[ind]=prosirido4(r[ind]);
    }break;
    case 3:{umanjiza2=true;}break;
    case 4:{uvecajza2=true;}break;
  }
}


void Emulator::azurirajPSW(int prvi,int drugi,int broj){
  if(broj<0) {
    psw[3]=1;
    psw[2]=1;
  }
  if(broj==0){
    psw[0]=1;
  }
  if(broj>=256*256)
    psw[2]=1;
  if(broj<0 && (prvi>0 && drugi<0)) psw[1]=1;
  if(broj>0 && (prvi<0 && drugi>0)) psw[1]=1;
}

void Emulator::stavinastek(string bajt){
  if(stoi(r[6],nullptr,16)-1<0) r[6]=decuHex(stoi(r[6],nullptr,16)-1+256*256);
  else if(stoi(r[6],nullptr,16)-1>256*256) r[6]=decuHex(stoi(r[6],nullptr,16)-1-256*256);
  else r[6]=decuHex(stoi(r[6],nullptr,16)-1);
  r[6]=prosirido4(r[6]);
  upisiumem(r[6],bajt);
}

string Emulator::skinisasteka(){
  string ret=sadrzajbajt(r[6]);
  if(stoi(r[6],nullptr,16)+1<0) r[6]=decuHex(stoi(r[6],nullptr,16)+1+256*256);
  else if(stoi(r[6],nullptr,16)+1>=256*256) r[6]=decuHex(stoi(r[6],nullptr,16)+1-256*256);
  else r[6]=decuHex(stoi(r[6],nullptr,16)+1);
  r[6]=prosirido4(r[6]);
  return ret;
}

void Emulator::stringUPSW(string s){
    int x = stoi(s, nullptr, 16);
    for (int i = 15; i >= 0; i--) {
        psw[i] = (x % 2);
        x=x>>1;  
    }
}

string Emulator::pswUString(){
    string ret,s2;
    int cnt=0,suma,s1=0;
    char hex[8];
    for(int i=0;i<16;i=i+4){
      suma=psw[i]*8+psw[i+1]*4+psw[i+2]*2+psw[i+3];
      ret+=decuHex(suma); 
      cnt=0;suma=0;
    } 
    return ret;
}

string Emulator::sadrzaj2bajta(string adresa){
  for(int i=0;i<memorija.size();i++){
    if(memorija[i].adresa==adresa){
      string drugi=memorija[i].sadrzaj;
      if(drugi.size()<2) drugi="0"+drugi;
      string prvi=memorija[i+1].sadrzaj;
      if(prvi.size()<2) prvi="0"+prvi;
      return prvi+drugi;
    }
  }
  return "Greska";
}

string Emulator::sadrzaj2stek(string adresa){
  for(int i=0;i<memorija.size();i++){
    if(memorija[i].adresa==adresa){
      string drugi=memorija[i].sadrzaj;
      if(drugi.size()<2) drugi="0"+drugi;
      string prvi=memorija[i-1].sadrzaj;
      if(prvi.size()<2) prvi="0"+prvi;
      return prvi+drugi;
    }
  }
  cout<<"Nepostojeca adresa";
  return "Greska";
}

void Emulator::ispis(){
  cout<<"Emulated processor executed halt instruction"<<'\n'
  <<"Emulated processor state: "<<"psw=0b";
  for(int i=0;i<16;i++){
    cout<<psw[i];
  }
  /*for(int i=0;i<memorija.size();i++){
    cout<<memorija[i].adresa<<':'<<memorija[i].sadrzaj<<'\n';
  }*/
  cout<<'\n';
  cout<<"r"<<"0"<<"=0x"<<r[0]<<'\t'<<"r"<<"1"<<"=0x"<<r[1]<<'\t'<<"r"<<"2"<<"=0x"<<r[2]<<'\t'<<"r"<<"3"<<"=0x"<<r[3]<<'\n';
  cout<<"r"<<"4"<<"=0x"<<r[4]<<'\t'<<"r"<<"5"<<"=0x"<<r[5]<<'\t'<<"r"<<"6"<<"=0x"<<r[6]<<'\t'<<"r"<<"7"<<"=0x"<<r[7]<<'\n';
  return;
}

string Emulator::sadrzajbajt(string adresa){
  for(int i=0;i<memorija.size();i++){
    if(memorija[i].adresa==adresa){
      string k=memorija[i].sadrzaj;
      if(k.size()<2) k="0"+k;
      return k;     
    }
  }
  cout<<"Nepostojeca adresa";
  return "Greska";
}

string Emulator::prosirido4(string rec){
  while(rec.size()<4)
    rec="0"+rec;
  return rec;
}

string Emulator::decuHex(int locationCounter){
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

vector<string> Emulator::tokeni(string linija){
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

void Emulator::upisiumem(string adr,string sad){
  for(int i=0;i<memorija.size();i++){
    if(memorija[i].adresa==adr){
      memorija[i].sadrzaj=sad;
      return;
    }
  }
  memorija.push_back(Memorija(adr,sad));
}

int main(int argc, char* argv[]) {
	Emulator e=Emulator(argv[1]);
	
}