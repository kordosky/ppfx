
#include "MIPPNumiKaonYieldsReweighter.h"
#include "DataHistos.h"
#include "CentralValuesAndUncertainties.h"

#include "MIPPNumiYieldsBins.h"
#include "MIPPNumiMC.h"
#include <iostream>

namespace NeutrinoFluxReweight{
  
  MIPPNumiKaonYieldsReweighter::MIPPNumiKaonYieldsReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars): iUniv(iuniv), cvPars(cv_pars), univPars(univ_pars){
  
     std::map<std::string, double> univ_table = univPars.table;
     std::map<std::string, double> cv_table = cvPars.table;
     float prt_no_inter = univ_table["prt_no_interacting"];
     
     std::vector<float> vbin_data_pip,vbin_data_pim;
     std::vector<float> vbin_data_kap_pip,vbin_data_kam_pim;

     char namepar[100];
     for(int ii=0;ii<124;ii++){
      sprintf(namepar,"MIPP_NuMI_%s_sys_%d","pip",ii);
      double data_cv  = cv_table[std::string(namepar)];
      double data_sys = univ_table[std::string(namepar)];
      sprintf(namepar,"MIPP_NuMI_%s_stats_%d","pip",ii);
      double data_sta = univ_table[std::string(namepar)];
      data_sys /= (1.0-prt_no_inter);
      data_sta /= (1.0-prt_no_inter);
      data_cv  /= (1.0-prt_no_inter);
      vbin_datacv_pip.push_back(data_cv);
      vbin_datasys_pip.push_back(data_sys);
      vbin_datasta_pip.push_back(data_sta);
     }
     
     for(int ii=0;ii<119;ii++){
      sprintf(namepar,"MIPP_NuMI_%s_sys_%d","pim",ii);
      double data_cv  = cv_table[std::string(namepar)];
      double data_sys = univ_table[std::string(namepar)];
      sprintf(namepar,"MIPP_NuMI_%s_stats_%d","pim",ii);
      double data_sta = univ_table[std::string(namepar)];
      data_sys /= (1.0-prt_no_inter);
      data_sta /= (1.0-prt_no_inter);
      data_cv  /= (1.0-prt_no_inter);
      vbin_datacv_pim.push_back(data_cv);
      vbin_datasys_pim.push_back(data_sys);
      vbin_datasta_pim.push_back(data_sta);
     }
    for(int ii=0;ii<24;ii++){
      sprintf(namepar,"MIPP_NuMI_%s_sys_%d","kap_pip",ii);
      double data_cv  = cv_table[std::string(namepar)];
      double data_sys = univ_table[std::string(namepar)];
      sprintf(namepar,"MIPP_NuMI_%s_stats_%d","kap_pip",ii);
      double data_sta = univ_table[std::string(namepar)];
      vbin_datacv_kap_pip.push_back(data_cv);
      vbin_datasys_kap_pip.push_back(data_sys);
      vbin_datasta_kap_pip.push_back(data_sta);
      
      sprintf(namepar,"MIPP_NuMI_%s_sys_%d","kam_pim",ii);
      data_cv  = cv_table[std::string(namepar)];
      data_sys = univ_table[std::string(namepar)];
      sprintf(namepar,"MIPP_NuMI_%s_stats_%d","kam_pim",ii);
      data_sta = univ_table[std::string(namepar)];
      vbin_datacv_kam_pim.push_back(data_cv);
      vbin_datasys_kam_pim.push_back(data_sys);
      vbin_datasta_kam_pim.push_back(data_sta);
    } 
    
  }
  MIPPNumiKaonYieldsReweighter::~MIPPNumiKaonYieldsReweighter(){
    
  }
  std::vector<bool> MIPPNumiKaonYieldsReweighter::canReweight(const InteractionChainData& aa){
    
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
    
    //Now we have the index of the hadron that exit the target in the 
    //ancesty chain:
    if(tar.Idx_ancestry>=0){
      for(int ii=0;ii<tar.Idx_ancestry;ii++){
	this_nodes[ii] = true;
      }
    }
    else{
      //      std::cout<<"==>>SOMETHING WEIRD WITH MIPP NUMI "<<tar.Idx_ancestry<<" "<<tar.Tar_pdg<<std::endl;
    }
    
    return this_nodes;
  }
  double MIPPNumiKaonYieldsReweighter::calculateWeight(const InteractionChainData& aa){
    
    MIPPNumiYieldsBins*  MIPPbins =  MIPPNumiYieldsBins::getInstance();
    MIPPNumiMC*  MCval =  MIPPNumiMC::getInstance();
    double low_value = 1.e-18;      
    
    TargetData tar = aa.tar_info;

    if(tar.Tar_pdg != 321 && tar.Tar_pdg != -321)return 1.0;
    if(tar.Pz<20.0 || tar.Pz>80.0 || tar.Pt>2.0)return 1.0;

    int binID = MIPPbins->BinID(tar.Pz,tar.Pt,tar.Tar_pdg);
    if(binID<0){
      //std::cout<<"=>Not K covered:" <<tar.Pz<<" "<<tar.Pt<<" " <<tar.Tar_pdg<<std::endl;
      return 1.0;
    }
    //Now looking for pion bin ID:
    int pi_wanted = 211;
    if(tar.Tar_pdg == -321)pi_wanted = -211;
    int pi_binID = MIPPbins->BinID(tar.Pz,tar.Pt,pi_wanted);
    if(pi_binID<0){
      //std::cout<<"=>There is not MIPP pion matching the kaon kinematics"<<pi_wanted<<" "<<tar.Pz<<" "<<tar.Pt<<std::endl;
      return 1.0;
    }
    
    //Now, looking for the MC value:
    double binC = MCval->getMCval(tar.Pz,tar.Pt,tar.Tar_pdg);
    if(binC<low_value){
      //std::cout<<"LOW MC VAL: "<<binC <<std::endl;
      return 1.0;
    }

    float K_data_cv  = -1.0;
    float K_data_sys = -1.0;
    float K_data_sta = -1.0;
    float K_data     = -1.0;
    
    if(tar.Tar_pdg == 321){
      K_data_cv  = vbin_datacv_pip[pi_binID] *vbin_datacv_kap_pip[binID];
      K_data_sys = vbin_datasys_pip[pi_binID]*vbin_datasys_kap_pip[binID];
      K_data_sta = vbin_datasta_pip[pi_binID]*vbin_datasta_kap_pip[binID];
      K_data = K_data_sys + K_data_sta - K_data_cv;
      
    }
    else if(tar.Tar_pdg ==-321){
      K_data_cv  = vbin_datacv_pim[pi_binID] *vbin_datacv_kam_pim[binID];
      K_data_sys = vbin_datasys_pim[pi_binID]*vbin_datasys_kam_pim[binID];
      K_data_sta = vbin_datasta_pim[pi_binID]*vbin_datasta_kam_pim[binID];
      K_data = K_data_sys + K_data_sta - K_data_cv;
    }
    //check:
    if(K_data_cv<low_value || K_data_sys<low_value || K_data_sta<low_value || K_data<low_value){
      //std::cout<<"LOW DATA VAL: "<<K_data_cv<<" "<<K_data_sys<<" "<<K_data_sta<<" "<<tar.Tar_pdg<<" "<<tar.Pz<<" "<<tar.Pt<<std::endl;
      return 1.0;
    }
    
    return double(K_data)/binC;

  }

}
