
#include <cstdlib>
#include <iostream>
#include "ThinTargetMC.h"


namespace NeutrinoFluxReweight{ 
  
  ThinTargetMC* ThinTargetMC::instance = 0;
  
  ThinTargetMC::ThinTargetMC(){
    const char* ppfxDir = getenv("PPFX_DIR");
    char dirData[400]= "data"; 
    sprintf(dirData,"%s/data",ppfxDir);
    spart_prod.push_back("pip");
    spart_prod.push_back("pim");
    spart_prod.push_back("kap");
    spart_prod.push_back("kam");
    mom_inc.push_back(12);mom_inc.push_back(20);
    mom_inc.push_back(31);mom_inc.push_back(40);
    mom_inc.push_back(50);mom_inc.push_back(60);
    mom_inc.push_back(70);mom_inc.push_back(80);
    mom_inc.push_back(90);mom_inc.push_back(100);
    mom_inc.push_back(110);mom_inc.push_back(120);
    
    for(int i=0;i<spart_prod.size();i++){
      fpC_x[i] = new TFile(Form("%s/MC/FTFP/invxs_%s_FTFP_BERT.root",dirData,spart_prod[i].c_str()),"read");
      for(int j=0;j<mom_inc.size();j++)vpC_x[i].push_back((TH2D*)fpC_x[i]->Get(Form("xF_pT_%dGeV",mom_inc[j])));
    }
  }
  
  double ThinTargetMC::getMCval_pC_X(double incP, double xf,double pt, int pdgcode){

    //check:
    if(incP<12)return -1;    
    if(pdgcode!=211 && pdgcode!=-211 && pdgcode!=321 && pdgcode!=-321)return -1;    
    //idx:
    int idx_part = -1;
    int idx_lowp = -1;
    int idx_hip  = -1;
    for(int i=0;i<mom_inc.size()-1;i++){
      if(incP>=double(mom_inc[i]) && incP<=double(mom_inc[i+1])){
	idx_lowp=i;
	idx_hip =i+1;
      }
    }
    if(idx_lowp==-1){
      idx_lowp = mom_inc.size()-2;
      idx_hip  = mom_inc.size()-1;
    }
    if(pdgcode ==  211)idx_part=0;
    if(pdgcode == -211)idx_part=1;
    if(pdgcode ==  321)idx_part=2;
    if(pdgcode == -321)idx_part=3;
    
    int binlow   = vpC_x[idx_part][idx_lowp]->FindBin(xf,pt);
    int binhi    = vpC_x[idx_part][idx_hip]->FindBin(xf,pt);
    double xslow = vpC_x[idx_part][idx_lowp]->GetBinContent(binlow);
    double xshi  = vpC_x[idx_part][idx_hip]->GetBinContent(binhi);
    
    return xslow + (incP-double(mom_inc[idx_lowp]))*(xshi-xslow)/(double(mom_inc[idx_hip])-double(mom_inc[idx_lowp]));
    
  }
  
  
  ThinTargetMC* ThinTargetMC::getInstance(){
    if (instance == 0) instance = new ThinTargetMC;
    return instance;
  }
  
}
