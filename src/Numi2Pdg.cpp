
#include "Numi2Pdg.h"

Numi2Pdg::Numi2Pdg(){
  
}

int Numi2Pdg::GetPdg(int numipart){
  
  int pdg = 0;
  
  switch(numipart){
  case 5:
    pdg = -13; break;
  case 6:
    pdg =  13; break;
  case 7:
    pdg = 111; break;
  case 8:
    pdg = 211; break;  
  case 9:
    pdg = -211; break; 
  case 10:
    pdg = 130; break;
  case 11:
    pdg = 321; break;
  case 12:
    pdg = -321; break;
  case 13:
    pdg = 2112; break;  
  case 14:
    pdg = 2212; break;
  case 15:
    pdg = -2212; break;
  case 16:
    pdg = 310; break;
  case 17:
    pdg = 221; break;
  case 18:
    pdg = 3122; break;  
  case 19:
    pdg = 3222; break;
  case 20:
    pdg = 3212; break;
  case 21:
    pdg = 3112; break;
  case 22:
    pdg = 3322; break;
  case 23:
    pdg = 3312; break;  
  case 24:
    pdg = 3334; break;
  case 25:
    pdg = -2112; break;
  case 26:
    pdg = -3122; break;
  case 27:
    pdg = -3112; break;
  case 28:
    pdg = -3212; break;  
  case 29:
    pdg = -3222; break;
  case 30:
    pdg = -3322; break;
  case 31:
    pdg = -3312; break;
  case 52:
    pdg = -12; break;
  case 53:
    pdg = 12; break;  
  case 55:
    pdg = -14;  break;
  case 56:
    pdg = 14; break;
  case 99:
    pdg = 0; break;
  }  

  return pdg;

}

Numi2Pdg::~Numi2Pdg(){
  
}
 
