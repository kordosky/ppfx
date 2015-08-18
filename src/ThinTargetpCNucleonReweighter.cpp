
#include "ThinTargetpCNucleonReweighter.h"
#include <iostream>
#include "ThinTargetMC.h"
#include "ThinTargetBins.h"
#include "DataHistos.h"

namespace NeutrinoFluxReweight{
  
  ThinTargetpCNucleonReweighter::ThinTargetpCNucleonReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars):iUniv(iuniv),cvPars(cv_pars),univPars(univ_pars){
    
    vbin_data_p.reserve(11399);
    vbin_data_n.reserve(161);
    std::map<std::string, double> cv_table   = cvPars.table;
    std::map<std::string, double> univ_table = univPars.table;
    
     data_prod_xs = univ_table["prod_prtC_xsec"];
    
    //the number of bins needs to be written from the xmls files 
    char namepar[100];
    for(int ii=0;ii<11399;ii++){
      
      sprintf(namepar,"ThinTarget_pC_%s_sys_%d","p",ii);
      double data_cv  = cv_table[std::string(namepar)];
      double data_sys = univ_table[std::string(namepar)];
      sprintf(namepar,"ThinTarget_pC_%s_stats_%d","p",ii);
      double data_sta = univ_table[std::string(namepar)];       
      vbin_data_p.push_back(data_sta + data_sys - data_cv);
      
    }    

    for(int ii=0;ii<161;ii++){
      
      sprintf(namepar,"ThinTarget_pC_%s_sys_%d","n",ii);
      double data_cv  = cv_table[std::string(namepar)];
      double data_sys = univ_table[std::string(namepar)];
      sprintf(namepar,"ThinTarget_pC_%s_stats_%d","n",ii);
      double data_sta = univ_table[std::string(namepar)];       
      vbin_data_n.push_back(data_sta + data_sys - data_cv);
      
    }   

    
  }
  
   ThinTargetpCNucleonReweighter::~ThinTargetpCNucleonReweighter(){
    
  }
  bool ThinTargetpCNucleonReweighter::canReweight(const InteractionData& aa){

    //checking:
    if(aa.Inc_pdg != 2212)return false;
    if(aa.Inc_P < 12.0)return false;
    if(aa.Vol != "TGT1" && aa.Vol != "BudalMonitor")return false;
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
      std::cout<<"Not bin found "<<std::endl;
      return wgt;
    }
    //Calculating the scale:
    double data_scale = calculateDataScale(aa.Inc_pdg,aa.Inc_P,aa.Prod_pdg,aa.xF,aa.Pt);
    double dataval = -1;
    if(aa.Prod_pdg==2212)dataval = vbin_data_p[bin_p];
    if(aa.Prod_pdg==2112)dataval = vbin_data_n[bin_n];
    
    if(dataval<1.e-12){
      std::cout<<"Not data found "<<std::endl;
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
    mc_cv /= calculateMCProd(aa.gen,aa.Prod_pdg,aa.Inc_P);
    if(mc_cv<1.e-12)return wgt;
    wgt = dataval/mc_cv;
    if(wgt<0){
      std::cout<<"TTPCNU check wgt(<0) "<<iUniv<<" "<<wgt<<" "<<aa.Inc_P<<" "<<aa.xF<<" "<<aa.Pt<<" "<<aa.Prod_pdg<<" "<<calculateMCProd(aa.gen,aa.Prod_pdg,aa.Inc_P)<<std::endl;
      return 1.0;
    }
    
    return wgt;
  }
  
  double ThinTargetpCNucleonReweighter::calculateMCProd(int genid, int pdg, double inc_mom){
    double xx[13] ={12,20,31,40,50,60,70,80,90,100,110,120,158};
    double yy[13] ={153386793./197812683.,160302538./197811564.,164508480./197831250.,166391359./197784915.,
		    167860919./197822312.,168882647./197807739.,169681805./197803099.,170311264./197811098.,
		    170860912./197822002.,171309291./197834756.,171651963./197811822.,171991260./197823012.,
		  172902228./197804669.};
    
    int idx_lowp = -1;
    int idx_hip  = -1;
    for(int i=0;i<12;i++){
      if(inc_mom>=double(xx[i]) && inc_mom<double(xx[i+1])){
	idx_lowp=i;
	idx_hip =i+1;
      }
    }
    double frac_low = yy[idx_lowp];
    double frac_hi  = yy[idx_hip];
    double frac_m   =  frac_low + (inc_mom-double(xx[idx_lowp]))*(frac_hi-frac_low)/(double(xx[idx_hip])-double(xx[idx_lowp]));
    
    if(genid==0 && pdg==2212)return frac_m*243.2435;
    if(genid>0  && pdg==2212)return frac_m;
    if(genid==0 && pdg==2112)return frac_m;
    if(genid>0  && pdg==2112)return frac_m/243.2435;
    return 1.0;

  }
  double ThinTargetpCNucleonReweighter::calculateDataScale(int inc_pdg, double inc_mom, int prod_pdg,double xf, double pt){
 
    double scaling_violation = 1.0;
    DataHistos*  dtH =  DataHistos::getInstance();
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
      int binid = dtH->hNA49Scl[idx_part][Nscl-1]->FindBin(xf,pt);
      scl_ref158 = dtH->hNA49Scl[idx_part][Nscl-1]->GetBinContent(binid);    
      //just provisional... the scaling just reach up to xF=0.8975... consering a close xf value
      if(xf>0.8975){
	binid = dtH->hNA49Scl[idx_part][Nscl-1]->FindBin(0.89,pt);
	scl_ref158 = dtH->hNA49Scl[idx_part][Nscl-1]->GetBinContent(binid); 
      }
      double scl_low = dtH->hNA49Scl[idx_part][idx_lowp]->GetBinContent(binid);
      double scl_hi  = dtH->hNA49Scl[idx_part][idx_hip]->GetBinContent(binid);
      scl_m   =  scl_low + (inc_mom-double(moms[idx_lowp]))*(scl_hi-scl_low)/(double(moms[idx_hip])-double(moms[idx_lowp]));
    }
    else if(idx_part==5){
      int binid = dtH->hNA49Scl_n[Nscl-1]->FindBin(xf);
      scl_ref158 = (double)dtH->hNA49Scl_n[Nscl-1]->GetBinContent(binid); 
      double scl_low = dtH->hNA49Scl_n[idx_lowp]->GetBinContent(binid);
      double scl_hi  = dtH->hNA49Scl_n[idx_hip]->GetBinContent(binid);
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

