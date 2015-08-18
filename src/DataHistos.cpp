#include <cstdlib>

#include "DataHistos.h"

namespace NeutrinoFluxReweight{
  
  DataHistos* DataHistos::instance = 0;
  
  DataHistos::DataHistos(){
    const char* ppfxDir = getenv("PPFX_DIR");
    char dirData[200]= "data"; 
    sprintf(dirData,"%s/data",ppfxDir);
    DataHistos::fMIPP_NUMI_PI = new TFile(Form("%s/MIPP/fpi_mipp_replica.root",dirData),"read");
    DataHistos::fMIPP_MC = new TFile(Form("%s/MIPP/fmc_tar.root",dirData),"read");
    
    DataHistos::fMIPP_NUMI_K  = new TFile(Form("%s/MIPP/fK_mipp_replica.root",dirData), "read");
    
    DataHistos::fInelXS_MC = new TFile(Form("%s/MC/InelXS_geant4.root",dirData),"read");

    DataHistos::fzpos_tgt_MC_le = new TFile(Form("%s/MIPP/tarpos_yield_le.root",dirData),"read");
    DataHistos::fzpos_tgt_MC_me = new TFile(Form("%s/MIPP/tarpos_yield_me.root",dirData),"read");

    //For MIPP Numi, I fill a vector of yield histograms in this order:
    //pip, pim, kap, kam, k0L
    TFile* f_use;
    const int Nmipp = 5;
    const char* cmipp_numi_parts[Nmipp] = {"pip","pim","kap","kam","K0L"};
    for(int ii=0;ii<Nmipp;ii++){
      if(ii<2){
	f_use = DataHistos::fMIPP_NUMI_PI; 
      }
      else if(ii>=2){
	f_use = DataHistos::fMIPP_NUMI_K; 
      }
      DataHistos::hMIPP_NUMI[ii] = (TH2D*)f_use->Get(Form("data/hdata_%s",cmipp_numi_parts[ii]));
      DataHistos::hMIPP_MC[ii]   = (TH2D*)DataHistos::fMIPP_MC->Get(Form("h2%s",cmipp_numi_parts[ii]));
      
    } 
    
    //For NA49 and scaling values, the order will be:
    //pip, pim, kap, kam, prt
    const int Nna49 = 5;
    const char* cna49_parts[Nna49] = {"pip","pim","kap","kam","prt"};
    const char* CNA49_PARTS[Nna49] = {"PIP","PIM","KAP","KAM","PRT"};
   
    const int Nscale = 11;
    const int mom_scale[Nscale] = {12,20,31,40,50,60,70,80,100,120,158};
  
    const int Nmc = 12;
    const int mom_mc[Nmc] = {12,20,31,40,50,60,70,80,90,100,110,120};
    
    DataHistos::fNA49_piK = new TFile(Form("%s/NA49/data_pC_XS.root",dirData));
    DataHistos::fNA49_prt = new TFile(Form("%s/NA49/data_pC_prt_XS.root",dirData));
    
    for(Int_t ipart=0;ipart<Nna49;ipart++){
      DataHistos::fNA49scale.push_back(new TFile(Form("%s/SCALING/%s_scaling.root",dirData,cna49_parts[ipart])));
    }
    DataHistos::fNA49scale.push_back(new TFile(Form("%s/SCALING/%s_scaling.root",dirData,"neu")));

    //Loading NA49 Data:  
    for(Int_t ipart=0;ipart<Nna49;ipart++){
      if(std::string(CNA49_PARTS[ipart]) != "PRT"){
	DataHistos::hNA49Data.push_back((TH2F*)DataHistos::fNA49_piK->Get(Form("%s_xs_data",CNA49_PARTS[ipart])));
      }
      else{
	DataHistos::hNA49Data.push_back((TH2F*)DataHistos::fNA49_prt->Get(Form("%s_xs_data",CNA49_PARTS[ipart])));
      }
    }
    
    //Loading scale histograms:
    for(Int_t ipart=0;ipart<Nna49;ipart++){
      std::vector<TH2F*> tmp_scl;
      for(int imom=0;imom<Nscale;imom++){
	tmp_scl.push_back((TH2F*)DataHistos::fNA49scale[ipart]->Get(Form("xF_pT_%dGeV",mom_scale[imom])));
      }
      hNA49Scl.push_back(tmp_scl);
      tmp_scl.clear(); 
    }
    //neutron scaling
    for(int imom=0;imom<Nscale;imom++){
      hNA49Scl_n.push_back((TH1F*)DataHistos::fNA49scale[Nna49]->Get(Form("xF_%dGeV",mom_scale[imom])));
    }
    
    //Loading the geant4 inelastic cross section for pion on Aluminum
    DataHistos::hXS_piAl = (TH1D*)DataHistos::fInelXS_MC->Get("pip/hpip_Al");
    DataHistos::hXS_prtC = (TH1D*)DataHistos::fInelXS_MC->Get("prt/hprt_C");
    DataHistos::hXS_piC  = (TH1D*)DataHistos::fInelXS_MC->Get("pip/hpip_C");

    //Loading z postition histograms from MC g4NuMI:
    //These are yields per z position per MIPP particle per MIPP bin.
    //per 2.5E8 POT
    //For pip: 124 bins, for pim: 119
    char namehist[100];
    for(int ibin=0;ibin<124;ibin++){
      sprintf(namehist,"pip/htar_ydz_pip_bin%d",ibin);
      DataHistos::hzpostgt_pip_le.push_back((TH1D*)DataHistos::fzpos_tgt_MC_le->Get(namehist));
      DataHistos::hzpostgt_pip_me.push_back((TH1D*)DataHistos::fzpos_tgt_MC_me->Get(namehist));
    }
    for(int ibin=0;ibin<119;ibin++){
      sprintf(namehist,"pim/htar_ydz_pim_bin%d",ibin);
      DataHistos::hzpostgt_pim_le.push_back((TH1D*)DataHistos::fzpos_tgt_MC_le->Get(namehist));
      DataHistos::hzpostgt_pim_me.push_back((TH1D*)DataHistos::fzpos_tgt_MC_me->Get(namehist));
    }
    //for kap and kam, we are going to get just thoese what we need (p_{Z} > 20 GeV/c)
    //that means, for kap and kam do not consider the first 78 hist and for kam
    for(int ibin=78;ibin<124;ibin++){
      sprintf(namehist,"kap/htar_ydz_kap_bin%d",ibin);
      DataHistos::hzpostgt_kap_le.push_back((TH1D*)DataHistos::fzpos_tgt_MC_le->Get(namehist));
      DataHistos::hzpostgt_kap_me.push_back((TH1D*)DataHistos::fzpos_tgt_MC_me->Get(namehist));
      sprintf(namehist,"kam/htar_ydz_kam_bin%d",ibin);
      DataHistos::hzpostgt_kam_le.push_back((TH1D*)DataHistos::fzpos_tgt_MC_le->Get(namehist));
      DataHistos::hzpostgt_kam_me.push_back((TH1D*)DataHistos::fzpos_tgt_MC_me->Get(namehist));
    }

  }
  
  DataHistos::~DataHistos(){ 
  }
  
  DataHistos* DataHistos::getInstance(){
    if (instance == 0) instance = new DataHistos;
    return instance;
  }
  
};
