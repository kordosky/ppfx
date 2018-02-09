
#include "ThinTargetpCNucleonReweighter.h"
#include <iostream>
#include <cstdlib>
#include "ThinTargetMC.h"
#include "ThinTargetBins.h"

namespace NeutrinoFluxReweight{
  
  ThinTargetpCNucleonReweighter::ThinTargetpCNucleonReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars):iUniv(iuniv),cvPars(cv_pars),univPars(univ_pars){
    
    ThinTargetBins* Thinbins =  ThinTargetBins::getInstance();
    
    vbin_data_p.reserve(Thinbins->GetNbins_pC_pX_NA49());
    vbin_data_n.reserve(Thinbins->GetNbins_pC_nX_NA49());
    // const boost::interprocess::flat_map<std::string, double>& cv_table   = cvPars.getMap();
    // const boost::interprocess::flat_map<std::string, double>& univ_table = univPars.getMap();
    
     data_prod_xs = univPars.getParameterValue("prod_prtC_xsec");
    
    //the number of bins needs to be written from the xmls files 
    char namepar[100];
    for(int ii=0;ii<Thinbins->GetNbins_pC_pX_NA49();ii++){
      
      sprintf(namepar,"ThinTarget_pC_%s_sys_%d","p",ii);
      double data_cv  = cvPars.getParameterValue(std::string(namepar));
      double data_sys = univPars.getParameterValue(std::string(namepar));
      sprintf(namepar,"ThinTarget_pC_%s_stats_%d","p",ii);
      double data_sta = univPars.getParameterValue(std::string(namepar));       
      vbin_data_p.push_back(data_sta + data_sys - data_cv);
      
    }    

    for(int ii=0;ii<Thinbins->GetNbins_pC_nX_NA49();ii++){
      
      sprintf(namepar,"ThinTarget_pC_%s_sys_%d","n",ii);
      double data_cv  = cvPars.getParameterValue(std::string(namepar));
      double data_sys = univPars.getParameterValue(std::string(namepar));
      sprintf(namepar,"ThinTarget_pC_%s_stats_%d","n",ii);
      double data_sta = univPars.getParameterValue(std::string(namepar));       
      vbin_data_n.push_back(data_sta + data_sys - data_cv);
      
    }   

    
  }
  
   ThinTargetpCNucleonReweighter::~ThinTargetpCNucleonReweighter(){
    
  }
  bool ThinTargetpCNucleonReweighter::canReweight(const InteractionData& aa){

    //checking:
    std::string mode(getenv("MODE"));
    if(aa.Inc_pdg != 2212)return false;
    if(aa.Inc_P < 12.0)return false;
    //volume check: 
    bool is_wrong_volume = aa.Vol != "TGT1" && aa.Vol != "BudalMonitor" && aa.Vol != "Budal_HFVS" && aa.Vol != "Budal_VFHS";
    if( (mode=="REF") || (mode=="OPT") ){
      is_wrong_volume = aa.Vol != "TargetFinHorizontal" && aa.Vol != "TargetNoSplitSegment";
    }
    if(is_wrong_volume)return false;
    //
    if(aa.Prod_pdg != 2212 && aa.Prod_pdg != 2112)return false;
    
    ThinTargetBins*  Thinbins =  ThinTargetBins::getInstance();
    int bin_p      = Thinbins->BinID_pC_p(aa.xF,aa.Pt,aa.Prod_pdg);
    int bin_n      = Thinbins->BinID_pC_n(aa.xF,aa.Prod_pdg);
    if(bin_p < 0 && bin_n < 0)return false;
    else return true;
    
  }
  
  double ThinTargetpCNucleonReweighter::calculateWeight(const InteractionData& aa){
 
    //quick check:
    double wgt = 1.0;
    ThinTargetBins*  Thinbins =  ThinTargetBins::getInstance();
    int bin_p = Thinbins->BinID_pC_p(aa.xF,aa.Pt,aa.Prod_pdg);
    int bin_n = Thinbins->BinID_pC_n(aa.xF,aa.Prod_pdg);
    if(bin_p < 0 && bin_n < 0){
      //std::cout<<"Not bin found "<<std::endl;
      return wgt;
    }

    //Calculating the scale:
    double data_scale = calculateDataScale(aa.Inc_pdg,aa.Inc_P,aa.Prod_pdg,aa.xF,aa.Pt);
    double dataval = -1;
    if(aa.Prod_pdg==2212)dataval = vbin_data_p[bin_p];
    if(aa.Prod_pdg==2112)dataval = vbin_data_n[bin_n];
    
    if(dataval<1.e-12){
      //std::cout<<"Not data found "<<std::endl;
      return wgt;
    }

    //checking if this is the first interaction:
    if(aa.gen==0 && aa.Prod_pdg==2212)dataval /= data_prod_xs;
    if(aa.gen>0  && aa.Prod_pdg==2212)dataval /= 1.0;
    if(aa.gen==0 && aa.Prod_pdg==2112)dataval /= 1.0;
    if(aa.gen>0  && aa.Prod_pdg==2112)dataval *= data_prod_xs;
    
    dataval *= data_scale;
 
    ThinTargetMC*  mc =  ThinTargetMC::getInstance(); 
    double mc_cv = mc->getMCval_pC_X(aa.Inc_P,aa.xF,aa.Pt,aa.Prod_pdg); 
    double mc_prod = mc->getMCxs_pC_nucleon(aa.gen,aa.Prod_pdg,aa.Inc_P);    
    mc_cv /= mc_prod;
    if(mc_cv<1.e-12)return wgt;
    wgt = dataval/mc_cv;
    if(wgt<0){
      std::cout<<"TTPCNU check wgt(<0) "<<iUniv<<" "<<wgt<<" "<<aa.Inc_P<<" "<<aa.xF<<" "<<aa.Pt<<" "<<aa.Prod_pdg<<" "<<(mc->getMCxs_pC_nucleon(aa.gen,aa.Prod_pdg,aa.Inc_P))<<std::endl;
      return 1.0;
    }

    return wgt;
  }
  
  double ThinTargetpCNucleonReweighter::calculateDataScale(int inc_pdg, double inc_mom, int prod_pdg,double xf, double pt){
 
    double scaling_violation = 1.0;
    ThinTargetMC*  dtH =  ThinTargetMC::getInstance();
    //temporary:
    const int Nscl = 11;
    const int moms[Nscl] = {12,20,31,40,50,60,70,80,100,120,158};
    
    int idx_part = -1;
    if(prod_pdg == 2212)idx_part = 4;
    else if(prod_pdg == 2112)idx_part = 5;
    else{
      std::cout<<"Error in the prod particle"<<std::endl;
      return 1.0;
    }
    
    int idx_lowp = -1;
    int idx_hip  = -1;
    for(int i=0;i<Nscl-1;i++){
      if(inc_mom>=double(moms[i]) && inc_mom<double(moms[i+1])){
	idx_lowp=i;
	idx_hip =i+1;
      }
    }
    if(idx_lowp<0 || idx_hip<0){
      std::cout<<"Error calculating the scaling"<<std::endl;
      return 1.0;
    }

    double scl_ref158 = -1.0;
    double scl_m      =  0.0;

    if(idx_part==4){
      int binid = dtH->hTTScl[idx_part][Nscl-1]->FindBin(xf,pt);
      scl_ref158 = dtH->hTTScl[idx_part][Nscl-1]->GetBinContent(binid);    
      //just provisional... the scaling just reach up to xF=0.8975... consering a close xf value
      if(xf>0.8975){
	binid = dtH->hTTScl[idx_part][Nscl-1]->FindBin(0.89,pt);
	scl_ref158 = dtH->hTTScl[idx_part][Nscl-1]->GetBinContent(binid); 
      }
      double scl_low = dtH->hTTScl[idx_part][idx_lowp]->GetBinContent(binid);
      double scl_hi  = dtH->hTTScl[idx_part][idx_hip]->GetBinContent(binid);
      scl_m   =  scl_low + (inc_mom-double(moms[idx_lowp]))*(scl_hi-scl_low)/(double(moms[idx_hip])-double(moms[idx_lowp]));
    }
    else if(idx_part==5){
      int binid = dtH->hTTScl_n[Nscl-1]->FindBin(xf);
      scl_ref158 = (double)dtH->hTTScl_n[Nscl-1]->GetBinContent(binid); 
      double scl_low = dtH->hTTScl_n[idx_lowp]->GetBinContent(binid);
      double scl_hi  = dtH->hTTScl_n[idx_hip]->GetBinContent(binid);
      scl_m   =  scl_low + (inc_mom-double(moms[idx_lowp]))*(scl_hi-scl_low)/(double(moms[idx_hip])-double(moms[idx_lowp]));
    }
    else{
      std::cout<<"still error, not expected here!!"<<std::endl;
      return 1.0;
    }
    
    if(scl_ref158 < 1.e-10 || scl_m<1.e-10){
      std::cout<<"scale problems: "<<scl_ref158<<" "<<scl_m<<" "<<inc_pdg<<" "<<inc_mom<<" "<<prod_pdg<<" "<<xf<<" "<<pt<<std::endl;
      return 1.0;
    }   
    scaling_violation = scl_m/scl_ref158;
    return scaling_violation;
  }
  
}

