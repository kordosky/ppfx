
#include "AttenuationReweighter.h"
#include <iostream>

namespace NeutrinoFluxReweight{
  
  AttenuationReweighter::AttenuationReweighter(){
    
  }
  AttenuationReweighter::~AttenuationReweighter(){
    
  }
  std::vector<bool> AttenuationReweighter::canReweight(const InteractionChainData& aa){
    std::vector<bool> can_rws;
    std::vector<InteractionData> vec_inter = aa.interaction_chain;
    int ninter = vec_inter.size();
    
    //Looking if there is a proton-Target interaction:
    for(int ii=0;ii<ninter;ii++){
  
      //first interaction:
      if(ii==0){
	bool is_tgt_int = vec_inter[0].Vol == "BudalMonitor" || vec_inter[0].Vol == "TGT1";
	if(is_tgt_int)can_rws.push_back(true);
	else if(vec_inter[0].Inc_pdg == 2212)can_rws.push_back(true);
	else can_rws.push_back(false);
      }
      //Absorption in the target:
      else{
	bool starts_tgt = vec_inter[ii-1].Vol == "BudalMonitor" || vec_inter[ii-1].Vol == "TGT1";
	bool ends_tgt   = vec_inter[ii].Vol   == "BudalMonitor" || vec_inter[ii].Vol   == "TGT1";
	if(starts_tgt && ends_tgt){
	  can_rws.push_back(true);
	}
	else{
	  can_rws.push_back(false);
	}
      }
    }  
    return can_rws;
  }
  double AttenuationReweighter::calculateWeight(const InteractionChainData&, ParameterTable& cv_pars, ParameterTable& univ_pars){
    return 1.0;
  }
  void AttenuationReweighter::ConfigureThisUniverse(int iuniv){
    UnivID = iuniv;
  }
}
