
#include "MIPPNumiKaonsYieldsReweighter.h"
#include "DataHistos.h"
#include <iostream>

namespace NeutrinoFluxReweight{
  
  MIPPNumiKaonsYieldsReweighter::MIPPNumiKaonsYieldsReweighter(){
    
  }
  MIPPNumiKaonsYieldsReweighter::~MIPPNumiKaonsYieldsReweighter(){
    
  }
  std::vector<bool> MIPPNumiKaonsYieldsReweighter::canReweight(const InteractionChainData& aa){
 
    DataHistos*  dtH =  DataHistos::getInstance();
    
    std::vector<bool> this_nodes;
    for(int ii=0;ii<(aa.interaction_chain).size();ii++){
      this_nodes.push_back(false);
    }
   
    //Look for MIPP Numi eventsfor kaons
    //if the code find a MIPP Numi event, it will look 
    //for how many interaction nodes covers
    //if not, return all nodes false.
    bool is_there_mipp = false;   
    TargetData tar = aa.tar_info;
    
    int  i_use = -1;
    //Basic kinematic coveraged by MIPP (Pz,Pt) in GeV/c extended to kaons:
    bool is_kin_K  = tar.Pz>=20. && tar.Pz<=80. && tar.Pt>=0. && tar.Pt<=0.5;

    if(tar.Tar_pdg ==  321 && is_kin_K)i_use = 2;
    else if(tar.Tar_pdg == -321 && is_kin_K)i_use = 3;
    else if(tar.Tar_pdg ==  130 && is_kin_K)i_use = 4;
    else{
      return this_nodes;
    }

    double binC = dtH->hMIPP_NUMI[i_use]->GetBinContent( dtH->hMIPP_NUMI[i_use]->FindBin(tar.Pz,tar.Pt) );
    if(binC>0){
      is_there_mipp = true;
    }
    else{
      return this_nodes;
    }

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
  double MIPPNumiKaonsYieldsReweighter::calculateWeight(const InteractionChainData&, ParameterTable& cv_pars, ParameterTable& univ_pars){
      return 1.0;
  }
  void MIPPNumiKaonsYieldsReweighter::ConfigureThisUniverse(int iuniv){
    UnivID = iuniv;
  }
}
