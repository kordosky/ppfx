
#include "MIPPThinTargetReweighter.h"
#include <iostream>
#include <cstdlib>

namespace NeutrinoFluxReweight{
  
  MIPPThinTargetReweighter::MIPPThinTargetReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars):iUniv(iuniv),cvPars(cv_pars),univPars(univ_pars){
 
  }
  
  MIPPThinTargetReweighter::~MIPPThinTargetReweighter(){
    
  }
  bool MIPPThinTargetReweighter::canReweight(const InteractionData& aa){
    
    //Currently we are correcting kaon production from proton interacions. 
    //K0L pdg code is 130 
    std::string mode(getenv("MODE"));
    bool is_inter = (aa.Inc_pdg == 2212) && 
      (aa.Prod_pdg==321 || aa.Prod_pdg==-321 || aa.Prod_pdg==130);

    //The minimum incident momentum is 12 GeV:
    bool is_mom = aa.Inc_P >= 12.0;

    //Kinematics:    
    Bool_t is_data_covered = (aa.xF>0.2 && aa.xF<=0.5) && (aa.Pt>0.0 && aa.Pt<2.0);

    //Volume:
    //need to account for different volumes for decay pipe and decay volume
    bool in_Target = false;
    bool in_DPIP = false;
    bool in_DVOL = false;
    bool in_TGAR = false;
    bool in_HORNIC = false;
    if(mode=="NUMI"){
     in_Target = (aa.Vol == "TGT1") || (aa.Vol == "BudalMonitor");
     in_DPIP   = aa.Vol  == "DPIP";
     in_DVOL   = aa.Vol  == "DVOL";
     in_TGAR   = aa.Vol  == "TGAR";
     in_HORNIC = (aa.Vol == "HORN1IC") || (aa.Vol == "HORN2IC");
    }
    if((mode=="REF")||(mode=="OPT")){
     in_Target = (aa.Vol == "TargetNoSplitSegment") || (aa.Vol == "TargetFinHorizontal");
     in_DPIP   = aa.Vol  == "DecayPipe";
     in_DVOL   = aa.Vol  == "DecayVolume";
     in_TGAR   = aa.Vol  == "TGAR";
     in_HORNIC = (aa.Vol == "Horn1IC") || (aa.Vol == "Horn2IC");     
    }    
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
