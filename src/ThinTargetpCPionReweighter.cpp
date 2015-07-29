
#include "ThinTargetpCPionReweighter.h"
#include "CentralValuesAndUncertainties.h"
#include "ThinTargetMC.h"
#include "ThinTargetBins.h"
#include "DataHistos.h"

#include <iostream>
namespace NeutrinoFluxReweight{
  
  ThinTargetpCPionReweighter::ThinTargetpCPionReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars):iUniv(iuniv),cvPars(cv_pars),univPars(univ_pars){
    
  }
  
   ThinTargetpCPionReweighter::~ThinTargetpCPionReweighter(){
    
  }
  bool ThinTargetpCPionReweighter::canReweight(const InteractionData& aa){

    //checking:
    if(aa.Inc_pdg != 2212)return false;
    if(aa.Inc_P < 12.0)return false;
    if(aa.Vol != "TGT1" && aa.Vol != "BudalMonitor")return false;
    
    ThinTargetBins*  Thinbins =  ThinTargetBins::getInstance();
    int bin = Thinbins->BinID_pC_X(aa.xF,aa.Pt,aa.Prod_pdg);
    if(bin<0)return false;
    if(bin>=0)return true;
  }
  
  double ThinTargetpCPionReweighter::calculateWeight(const InteractionData& aa){
    
    double wgt = 1.0;
    
    ThinTargetBins*  Thinbins =  ThinTargetBins::getInstance();
    int bin = Thinbins->BinID_pC_X(aa.xF,aa.Pt,aa.Prod_pdg);

    if(bin<0)return wgt;;
    
    const char* ptype = "pip";
    if(aa.Prod_pdg == -211)ptype = "pim"; 
    char namepar_sys[100];
    char namepar_sta[100];
    sprintf(namepar_sys,"ThinTarget_pC_%s_sys_%d",ptype,bin);
    sprintf(namepar_sta,"ThinTarget_pC_%s_stats_%d",ptype,bin);

    std::map<std::string, double> cv_table   = cvPars.table;
    std::map<std::string, double> univ_table = univPars.table;
    std::map<std::string, double>::iterator it;
    
    it = cv_table.find(std::string(namepar_sys));
    double data_cv = it->second;
    it = univ_table.find(std::string(namepar_sys));
    double data_sys = it->second;
    it = univ_table.find(std::string(namepar_sta));
    double data_sta = it->second;
    
    //Calculating the scale:
    double data_scale = calculateDataScale(aa.Inc_pdg,aa.Inc_P,aa.Prod_pdg,aa.xF,aa.Pt);

    data_cv  *= data_scale;
    data_sys *= data_scale;
    data_sta *= data_scale;
    
    //dividing over the production cross section:
    it = univ_table.find("prod_prtC_xsec");
    double data_prod_xs = it->second;
    data_cv  /= 226.5;
    data_sys /= data_prod_xs;
    data_sta /= data_prod_xs;

    ThinTargetMC*  mc =  ThinTargetMC::getInstance();
    double mc_cv = mc->getMCval_pC_X(aa.Inc_P,aa.xF,aa.Pt,aa.Prod_pdg);
    
    mc_cv /= calculateMCProd(aa.Inc_P);
    
     if(mc_cv<1.e-12)return wgt;
    wgt = (data_sys + data_sta - data_cv)/mc_cv;
    return wgt;
  }
  
  double ThinTargetpCPionReweighter::calculateMCProd(double inc_mom){
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
    
    return frac_m*243.2435;
    
  }
  double ThinTargetpCPionReweighter::calculateDataScale(int inc_pdg, double inc_mom, int prod_pdg,double xf, double pt){
    double scaling_violation = 1.0;
    DataHistos*  dtH =  DataHistos::getInstance();
    //temporary:
    const int Nscl = 11;
    const int moms[Nscl] = {12,20,31,40,50,60,70,80,100,120,158};
    
    int idx_part = -1;
    if(prod_pdg == 211)idx_part = 0;
    if(prod_pdg ==-211)idx_part = 1;
    if(idx_part<0){
      std::cout<<"Error in the prod particle"<<std::endl;
      return 1.0;
    }
    
    int binid = dtH->hNA49Scl[idx_part][Nscl-1]->FindBin(xf,pt);
    double scl_ref158 = dtH->hNA49Scl[idx_part][Nscl-1]->GetBinContent(binid);    
    
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
    double scl_low = dtH->hNA49Scl[idx_part][idx_lowp]->GetBinContent(binid);
    double scl_hi  = dtH->hNA49Scl[idx_part][idx_hip]->GetBinContent(binid);
    double scl_m   =  scl_low + (inc_mom-double(moms[idx_lowp]))*(scl_hi-scl_low)/(double(moms[idx_hip])-double(moms[idx_lowp]));
    if(scl_ref158<1.e-10){
      std::cout<<"ref158 zero!!! "<<scl_ref158<<std::endl;
      return 1.0;
    }
    scaling_violation = scl_m/scl_ref158;
    return scaling_violation;
  }

}
