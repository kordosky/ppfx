#include <cstdlib>

#include "AttenuationMC.h"

namespace NeutrinoFluxReweight{
  
  AttenuationMC* AttenuationMC::instance = 0;
  
  AttenuationMC::AttenuationMC(){
    const char* ppfxDir = getenv("PPFX_DIR");
    char dirData[400]; 
    sprintf(dirData,"%s/data",ppfxDir);

    AttenuationMC::fInelXS_MC = new TFile(Form("%s/MC/InelXS_geant4.root",dirData),"read");

    AttenuationMC::fzpos_tgt_MC_le = new TFile(Form("%s/MIPP/tarpos_yield_le.root",dirData),"read");
    AttenuationMC::fzpos_tgt_MC_me = new TFile(Form("%s/MIPP/tarpos_yield_me.root",dirData),"read");
    
    //Loading the geant4 inelastic cross section for pion on Aluminum
    AttenuationMC::hXS_piAl = (TH1D*)AttenuationMC::fInelXS_MC->Get("pip/hpip_Al");
    AttenuationMC::hXS_prtC = (TH1D*)AttenuationMC::fInelXS_MC->Get("prt/hprt_C");
    AttenuationMC::hXS_piC  = (TH1D*)AttenuationMC::fInelXS_MC->Get("pip/hpip_C");

    //Loading z postition histograms from MC g4NuMI:
    //These are yields per z position per MIPP particle per MIPP bin.
    //per 2.5E8 POT
    //For pip: 124 bins, for pim: 119
    char namehist[100];
    for(int ibin=0;ibin<124;ibin++){
      sprintf(namehist,"pip/htar_ydz_pip_bin%d",ibin);
      AttenuationMC::hzpostgt_pip_le.push_back((TH1D*)AttenuationMC::fzpos_tgt_MC_le->Get(namehist));
      AttenuationMC::hzpostgt_pip_me.push_back((TH1D*)AttenuationMC::fzpos_tgt_MC_me->Get(namehist));
    }
    for(int ibin=0;ibin<119;ibin++){
      sprintf(namehist,"pim/htar_ydz_pim_bin%d",ibin);
      AttenuationMC::hzpostgt_pim_le.push_back((TH1D*)AttenuationMC::fzpos_tgt_MC_le->Get(namehist));
      AttenuationMC::hzpostgt_pim_me.push_back((TH1D*)AttenuationMC::fzpos_tgt_MC_me->Get(namehist));
    }
    //for kap and kam, we are going to get just thoese what we need (p_{Z} > 20 GeV/c)
    //that means, for kap and kam do not consider the first 78 hist and for kam
    for(int ibin=78;ibin<124;ibin++){
      sprintf(namehist,"kap/htar_ydz_kap_bin%d",ibin);
      AttenuationMC::hzpostgt_kap_le.push_back((TH1D*)AttenuationMC::fzpos_tgt_MC_le->Get(namehist));
      AttenuationMC::hzpostgt_kap_me.push_back((TH1D*)AttenuationMC::fzpos_tgt_MC_me->Get(namehist));
      sprintf(namehist,"kam/htar_ydz_kam_bin%d",ibin);
      AttenuationMC::hzpostgt_kam_le.push_back((TH1D*)AttenuationMC::fzpos_tgt_MC_le->Get(namehist));
      AttenuationMC::hzpostgt_kam_me.push_back((TH1D*)AttenuationMC::fzpos_tgt_MC_me->Get(namehist));
    }

  }
  
  AttenuationMC::~AttenuationMC(){ 
  }
  
  AttenuationMC* AttenuationMC::getInstance(){
    if (instance == 0) instance = new AttenuationMC;
    return instance;
  }
  
};
