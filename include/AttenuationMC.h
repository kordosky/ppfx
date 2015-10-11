
#ifndef ATTENUATIONMC_H
#define ATTENUATIONMC_H

#include "TH2D.h"
#include "TFile.h"
#include <vector>

namespace NeutrinoFluxReweight{
  
  class AttenuationMC{

  private:    
    AttenuationMC();
    ~AttenuationMC();

  public:
    static AttenuationMC* getInstance();
    
    TH1D* hXS_piAl;
    TH1D* hXS_prtC;
    TH1D* hXS_piC;
    TH1D* hXS_kapC;
    TH1D* hXS_kamC;
    
    std::vector<TH1D*> hzpostgt_pip_le, hzpostgt_pip_me;
    std::vector<TH1D*> hzpostgt_pim_le, hzpostgt_pim_me;
    std::vector<TH1D*> hzpostgt_kap_le, hzpostgt_kap_me;
    std::vector<TH1D*> hzpostgt_kam_le, hzpostgt_kam_me;

  private:
    static AttenuationMC* instance;
    TFile* fInelXS_MC;   
    TFile* fzpos_tgt_MC_le;
    TFile* fzpos_tgt_MC_me;
    
  };
  
};


#endif 
