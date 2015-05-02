
#include "MIPPNumiKaonsYieldsReweighter.h"
#include "DataHistos.h"
#include "CentralValuesAndUncertainties.h"

#include "MIPPNumiYieldsBins.h"
#include <iostream>

namespace NeutrinoFluxReweight{
  
  MIPPNumiKaonsYieldsReweighter::MIPPNumiKaonsYieldsReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars): iUniv(iuniv), cvPars(cv_pars), univPars(univ_pars){
    
  }
  MIPPNumiKaonsYieldsReweighter::~MIPPNumiKaonsYieldsReweighter(){
    
  }
  std::vector<bool> MIPPNumiKaonsYieldsReweighter::canReweight(const InteractionChainData& aa){
    
    MIPPNumiYieldsBins*  MIPPbins =  MIPPNumiYieldsBins::getInstance();
    std::vector<bool> this_nodes;
    for(int ii=0;ii<(aa.interaction_chain).size();ii++){
      this_nodes.push_back(false);
    }
    
    //Look for MIPP Numi events for kaons
    //if the code find a MIPP Numi event, it will look 
    //for how many interaction nodes covers
    //if not, return all nodes false.
    bool is_there_mipp = false;   
    TargetData tar = aa.tar_info;
    
    //Cheking if the particle is a kaon plus or kaon minus:
    if(tar.Tar_pdg != 321 && tar.Tar_pdg != -321)return this_nodes;
    int binID = MIPPbins->BinID(tar.Pz,tar.Pt,tar.Tar_pdg);
    if(binID<0) return this_nodes;
    
    //Now that we know that we have a MIPP Numi event, 
    //we will see how many nodes are covered.
    std::vector<InteractionData> this_interactions = aa.interaction_chain; 
    
    //The first hypotesis is the following:
    //the MIPP particle was born in the last interaction
    //happens in Carbon.
    //I need to think how to do it better with dk2nu
    //perhaps store the ancestry position of the MIPP particle.
    //and use this here?? 


    ///////////////
    int pos_inter = -1;
    for(int ii=(this_interactions.size()-1);ii>=0;ii--){
      bool is_tgt_candidate = this_interactions[ii].Vol == "TGT1" || this_interactions[ii].Vol == "BudalMonitor" || 
	this_interactions[ii].Vol == "pvTargetMother";
      if(is_tgt_candidate){
	pos_inter = ii;
	break;
      }
    }
    /////

    //In a quick counting , I missed 0.4% total events.
    //Let's assume that the first particle produced 
    //that match the pdg of "tar" is the wanted particle.
  
    if(pos_inter<0){
      for(int ii=0;ii<this_interactions.size();ii++){
	if(tar.Tar_pdg == this_interactions[ii].Prod_pdg){
	  pos_inter = ii;
	  break;
	}	
      }
    }
    
    //more work needs to be done to be sure that we catch a right hadron that exit the target!!
    
    if(pos_inter<0){
      std::cout<<"==>>SOMETHING WEIRD WITH MIPP NUMI "<<pos_inter<<" "<<tar.Tar_pdg<<std::endl;
    }
    
    if(pos_inter>=0){
      for(int ii=0;ii<=pos_inter;ii++){
	this_nodes[ii] = true;
      }
    }
    
    return this_nodes;
  }
  double MIPPNumiKaonsYieldsReweighter::calculateWeight(const InteractionChainData& aa){
      return 1.0;
  }

}
