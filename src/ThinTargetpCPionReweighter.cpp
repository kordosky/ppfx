
#include "ThinTargetpCPionReweighter.h"
#include "CentralValuesAndUncertainties.h"
#include "ThinTargetMC.h"
#include "ThinTargetBins.h"

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
       
    ThinTargetMC*  mc =  ThinTargetMC::getInstance();
    double mc_cv = mc->getMCval_pC_X(aa.Inc_P,aa.xF,aa.Pt,aa.Prod_pdg);
    
    if(mc_cv<1.e-12)return wgt;;
    
    wgt = (data_sys + data_sta - data_cv)/mc_cv;
    
    return wgt;
  }
  

}
