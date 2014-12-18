
#include "MIPPThinTargetReweighter.h"
#include <iostream>

namespace NeutrinoFluxReweight{
  
  MIPPThinTargetReweighter::MIPPThinTargetReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars):iUniv(iuniv),cvPars(cv_pars),univPars(univ_pars){
 
  }
  
  MIPPThinTargetReweighter::~MIPPThinTargetReweighter(){
    
  }
  bool MIPPThinTargetReweighter::canReweight(const InteractionData& aa){
    
    //Currently we are correcting kaon production from proton interacions. 
    //K0L pdg code is 130 
    bool is_inter = (aa.Inc_pdg == 2212) && 
      (aa.Prod_pdg==321 || aa.Prod_pdg==-321 || aa.Prod_pdg==130);

    //The minimum incident momentum is 12 GeV:
    bool is_mom = aa.Inc_P >= 12.0;

    //Kinematics:    
    Bool_t is_data_covered = (aa.xF>0.2 && aa.xF<=0.5) && (aa.Pt>0.0 && aa.Pt<2.0);

    //Volume:
    bool in_Target = (aa.Vol == "TGT1") || (aa.Vol == "BudalMonitor");
    bool in_DPIP   = aa.Vol  == "DPIP";
    bool in_DVOL   = aa.Vol  == "DVOL";
    bool in_TGAR   = aa.Vol  == "TGAR";
    bool in_HORNIC = (aa.Vol == "HORN1IC") || (aa.Vol == "HORN2IC");    
    bool is_wanted_volume = in_Target || in_DPIP || in_DVOL || in_TGAR || in_HORNIC;
    
    if(is_inter && is_mom && is_data_covered && is_wanted_volume){
      return true;
    }
    else{
      return false;
    }
    
    
  }
  
  double MIPPThinTargetReweighter::calculateWeight(const InteractionData& thisid){

    return 1.0;
  }

}
