#include "ReweightDriver.h"
#include <iostream>

namespace NeutrinoFluxReweight{
  
  ReweightDriver::ReweightDriver(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars)
    : iUniv(iuniv), cvPars(cv_pars), univPars(univ_pars)
  {
    Configure();
  }
  
  void ReweightDriver::Configure(){
    
    //Creating the vector of reweighters:

    MIPP_NUMI_Universe = new MIPPNumiYieldsReweighter(iUniv,cvPars,univPars);
        
    MIPP_NUMI_KAONS_Universe = new MIPPNumiKaonsYieldsReweighter(iUniv,cvPars,univPars);

    ATT_Universe = new AttenuationReweighter(iUniv,cvPars,univPars);

    NA49_Universe = new NA49Reweighter(iUniv,cvPars,univPars);
    
    MIPP_THIN_Universe = new MIPPThinTargetReweighter(iUniv,cvPars,univPars);

    THEORY_Universe = new TheoryThinTargetReweighter(iUniv,cvPars,univPars);
     
  }
  
  double ReweightDriver::calculateWeight(const InteractionChainData& icd){
 
    double tot_wgt = 1.0;
    
    //Boolean flags: 
    std::vector<bool> interaction_nodes;
    std::vector<bool> attenuation_nodes;

    //First we look at MIPP and look absorption chain:
    interaction_nodes = MIPP_NUMI_Universe->canReweight(icd);
    attenuation_nodes = ATT_Universe->canReweight(icd);

    //Looking for MIPP:
    double mipp_wgt = 1.0;
    bool has_mipp = false;
    for(int ii=0;ii<interaction_nodes.size();ii++){
      if(interaction_nodes[ii]==true){
	has_mipp = true;
	mipp_wgt = MIPP_NUMI_Universe->calculateWeight(icd);
	//	std::cout<<"ReweightDriver "<<mipp_wgt<<std::endl;
	break; // break after the first interaction node
	// we don't want to compute the same mipp_wgt over and over
      }
    }
    tot_wgt *= mipp_wgt;

    /*
    //Looking for MIPP kaon extension:
    double mipp_kaons_wgt = 1.0;
    if(!has_mipp){
      interaction_nodes = MIPP_NUMI_KAONS_Universe->canReweight(icd);
      for(int ii=0;ii<interaction_nodes.size();ii++){
	if(interaction_nodes[ii]==true){
	  mipp_kaons_wgt = MIPP_NUMI_KAONS_Universe->calculateWeight(icd,cv_pars,univ_pars);
	  break;
	}
      }
    }
    tot_wgt *= mipp_kaons_wgt;
    
    //Looking for NA49:
    double na49_wgt = 1.0;
    for(int ii=0;ii<interaction_nodes.size();ii++){
      if(interaction_nodes[ii]==false){
	bool can_na49 = NA49_Universe->canReweight((icd.interaction_chain)[ii]);
	if(can_na49){
	  interaction_nodes[ii]= true;
	  na49_wgt *= NA49_Universe->calculateWeight((icd.interaction_chain)[ii],cv_pars,univ_pars);
	}
      }
    }
    tot_wgt *= na49_wgt;
    
    //Looking for thin target MIPP:
    double mipp_thin_wgt = 1.0;
    for(int ii=0;ii<interaction_nodes.size();ii++){
      if(interaction_nodes[ii]==false){
	bool can_mipp_thin = MIPP_THIN_Universe->canReweight((icd.interaction_chain)[ii]);
	if(can_mipp_thin){
	  interaction_nodes[ii]= true;
	  mipp_thin_wgt *= MIPP_THIN_Universe->calculateWeight((icd.interaction_chain)[ii],cv_pars,univ_pars);
	}
      }
    }
    tot_wgt *= mipp_thin_wgt;
    
    //Looking for theory (model) prediction:
    double theory_wgt = 1.0;
    for(int ii=0;ii<interaction_nodes.size();ii++){
      if(interaction_nodes[ii]==false){
	bool can_theory = THEORY_Universe->canReweight((icd.interaction_chain)[ii]);
	if(can_theory){
	  interaction_nodes[ii]= true;
	  theory_wgt *= THEORY_Universe->calculateWeight((icd.interaction_chain)[ii],cv_pars,univ_pars);
	}
      }
    }
    tot_wgt *= theory_wgt;
    

    //Looking for attenuation (absorption) correction:
    double att_wgt = 1.0;
    for(int ii=0;ii<attenuation_nodes.size();ii++){
      if(attenuation_nodes[ii]==true){
	att_wgt *= ATT_Universe->calculateWeight(icd,cv_pars,univ_pars);
	break;
      }
    }
    tot_wgt *= att_wgt;
    */

    return tot_wgt;
    
  }
  
};

