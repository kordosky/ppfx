
#ifndef DATAHISTOS_H
#define DATAHISTOS_H

#include "TH2D.h"
#include "TFile.h"
#include <vector>

namespace NeutrinoFluxReweight{
  
  class DataHistos{
  public:
    
    DataHistos();
    ~DataHistos();
    static DataHistos* getInstance();
    
    TH2D* hMIPP_NUMI[5];
    TH2D* hMIPP_MC[5];
    std::vector<TH2F*> hNA49Data;
    std::vector< std::vector<TH2F*> > hNA49Scl; 
    
    TH1D* hXS_piAl;
    TH1D* hXS_prtC;
    TH1D* hXS_piC;
    
    std::vector<TH1D*> hzpostgt_pip_le, hzpostgt_pip_me;
    std::vector<TH1D*> hzpostgt_pim_le, hzpostgt_pim_me;
    std::vector<TH1D*> hzpostgt_kap_le, hzpostgt_kap_me;
    std::vector<TH1D*> hzpostgt_kam_le, hzpostgt_kam_me;

  private:
    static DataHistos* instance;
    TFile* fMIPP_NUMI_PI;
    TFile* fMIPP_NUMI_K;
    TFile* fNA49_piK;
    TFile* fNA49_prt;
    std::vector<TFile*> fMCscale;

    TFile* fMIPP_MC;
    TFile* fInelXS_MC;   
    TFile* fzpos_tgt_MC_le;
    TFile* fzpos_tgt_MC_me;
    
  };
  
};

#endif 
