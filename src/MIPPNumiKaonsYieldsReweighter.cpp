
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
    //beeing more restrict with the coverage:
    if(tar.Pz<20.0 || tar.Pz>80.0 || tar.Pt>2.0)return this_nodes;
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
    
    MIPPNumiYieldsBins*  MIPPbins =  MIPPNumiYieldsBins::getInstance();
    DataHistos*          dtH      =  DataHistos::getInstance();
    double low_value = 1.e-18;      
    
    TargetData tar = aa.tar_info;

    if(tar.Tar_pdg != 321 && tar.Tar_pdg != -321)return 1.0;
    if(tar.Pz<20.0 || tar.Pz>80.0 || tar.Pt>2.0)return 1.0;

    int binID = MIPPbins->BinID(tar.Pz,tar.Pt,tar.Tar_pdg);
    if(binID<0){
      std::cout<<"=>Not K covered:" <<tar.Pz<<" "<<tar.Pt<<" " <<tar.Tar_pdg<<std::endl;
      return 1.0;
    }
    //Now looking for pion bin ID:
    int pi_wanted = 211;
    if(tar.Tar_pdg == -321)pi_wanted = -211;
    int pi_binID = MIPPbins->BinID(tar.Pz,tar.Pt,pi_wanted);
    if(pi_binID<0){
      std::cout<<"=>There is not MIPP pion matching the kaon kinematics"<<pi_wanted<<" "<<tar.Pz<<" "<<tar.Pt<<std::endl;
      return 1.0;
    }
    
    const char* pitype = "pip";
    const char* kptype   = "kap_pip";
    if(tar.Tar_pdg == -321){
      pitype = "pim"; 
      kptype = "kam_pim";
    }
    char namepar_pi_sys[100]; char namepar_pi_sta[100];
    char namepar_kp_sys[100]; char namepar_kp_sta[100];
    sprintf(namepar_pi_sys,"MIPP_NuMI_%s_sys_%d",pitype,pi_binID);
    sprintf(namepar_kp_sys,"MIPP_NuMI_%s_sys_%d",kptype,binID);
    sprintf(namepar_pi_sta,"MIPP_NuMI_%s_stats_%d",pitype,pi_binID);
    sprintf(namepar_kp_sta,"MIPP_NuMI_%s_stats_%d",kptype,binID);
    
    ///CV:
    std::map<std::string, double> cv_table = cvPars.table;
    std::map<std::string, double>::iterator cv_it = cv_table.begin();
    
    cv_it = cv_table.begin();    
    cv_it = cv_table.find(std::string(namepar_pi_sys));
    double data_cv_pi = cv_it->second;
    
    cv_it = cv_table.begin();
    cv_it = cv_table.find(std::string(namepar_kp_sys));
    double data_cv_kp = cv_it->second;
    
    //UNIV:
    std::map<std::string, double> this_table = univPars.table;
    std::map<std::string, double>::iterator it = this_table.begin();

    it = this_table.begin();
    it = this_table.find(std::string(namepar_pi_sys));
    double data_univ_pi_sys = it->second;
    
    it = this_table.begin();
    it = this_table.find(std::string(namepar_kp_sys));
    double data_univ_kp_sys = it->second;

    it = this_table.begin();
    it = this_table.find(std::string(namepar_pi_sta));
    double data_univ_pi_sta = it->second;

    it = this_table.begin();
    it = this_table.find(std::string(namepar_kp_sta));
    double data_univ_kp_sta = it->second;
    

    //Now, looking for the MC value:
    int i_use = 2;
    if(tar.Tar_pdg == -321)i_use = 3;
    int wanted_bin = dtH->hMIPP_MC[i_use]->FindBin(tar.Pz,tar.Pt);
    double binC = dtH->hMIPP_MC[i_use]->GetBinContent(wanted_bin);
    if(binC<low_value){
      std::cout<<"LOW MC VAL: "<<binC <<std::endl;
      return 1.0;
    }

    //First assumption: no correlation at all between Sharon & Jon results:
    double K_data_cv = data_cv_pi*data_cv_kp;
    double K_data_univ_sys = data_univ_pi_sys*data_univ_kp_sys;
    double K_data_univ_sta = data_univ_pi_sta*data_univ_kp_sta;

    if(K_data_cv<low_value || K_data_univ_sys<low_value || K_data_univ_sta<low_value){
      std::cout<<"LOW DATA VAL: "<<K_data_cv<<" "<<K_data_univ_sys<<" "<<K_data_univ_sta<<" "<<tar.Tar_pdg<<" "<<tar.Pz<<" "<<tar.Pt<<std::endl;
      return 1.0;
    }
     
    return (K_data_univ_sys + K_data_univ_sta - K_data_cv)/binC;

  }

}
