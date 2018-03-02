
#include "MIPPNumiKaonYieldsReweighter.h"
#include "CentralValuesAndUncertainties.h"

#include "MIPPNumiYieldsBins.h"
#include "MIPPNumiMC.h"
#include <iostream>

namespace NeutrinoFluxReweight{
  
  MIPPNumiKaonYieldsReweighter::MIPPNumiKaonYieldsReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars): cvPars(cv_pars), univPars(univ_pars), iUniv(iuniv){
    
    MIPPNumiYieldsBins* MIPPbins =  MIPPNumiYieldsBins::getInstance();
    vbin_datacv_pip.reserve(MIPPbins->GetNbins_pip_MIPPNuMI());
    vbin_datacv_pim.reserve(MIPPbins->GetNbins_pim_MIPPNuMI());
    vbin_datasys_pip.reserve(MIPPbins->GetNbins_pip_MIPPNuMI());
    vbin_datasys_pim.reserve(MIPPbins->GetNbins_pim_MIPPNuMI());
    vbin_datasta_pip.reserve(MIPPbins->GetNbins_pip_MIPPNuMI());
    vbin_datasta_pim.reserve(MIPPbins->GetNbins_pim_MIPPNuMI());
    
    vbin_datacv_kap_pip.reserve(MIPPbins->GetNbins_K_MIPPNuMI());
    vbin_datacv_kam_pim.reserve(MIPPbins->GetNbins_K_MIPPNuMI());
    vbin_datasys_kap_pip.reserve(MIPPbins->GetNbins_K_MIPPNuMI());
    vbin_datasys_kam_pim.reserve(MIPPbins->GetNbins_K_MIPPNuMI());
    vbin_datasta_kap_pip.reserve(MIPPbins->GetNbins_K_MIPPNuMI());
    vbin_datasta_kam_pim.reserve(MIPPbins->GetNbins_K_MIPPNuMI());

    //const boost::interprocess::flat_map<std::string, double>& univ_table = univPars.getMap();
    //const boost::interprocess::flat_map<std::string, double>& cv_table = cvPars.getMap();
     prt_no_inter = univPars.getParameterValue("prt_no_interacting");
     
     char namepar[100];
     for(int ii=0;ii<MIPPbins->GetNbins_pip_MIPPNuMI();ii++){
      sprintf(namepar,"MIPP_NuMI_%s_sys_%d","pip",ii);
      double data_cv  = cvPars.getParameterValue(std::string(namepar));
      double data_sys = univPars.getParameterValue(std::string(namepar));
      sprintf(namepar,"MIPP_NuMI_%s_stats_%d","pip",ii);
      double data_sta = univPars.getParameterValue(std::string(namepar));
      data_sys /= (1.0-prt_no_inter);
      data_sta /= (1.0-prt_no_inter);
      data_cv  /= (1.0-prt_no_inter);
      vbin_datacv_pip.push_back(data_cv);
      vbin_datasys_pip.push_back(data_sys);
      vbin_datasta_pip.push_back(data_sta);
     }
     
     for(int ii=0;ii<MIPPbins->GetNbins_pim_MIPPNuMI();ii++){
      sprintf(namepar,"MIPP_NuMI_%s_sys_%d","pim",ii);
      double data_cv  = cvPars.getParameterValue(std::string(namepar));
      double data_sys = univPars.getParameterValue(std::string(namepar));
      sprintf(namepar,"MIPP_NuMI_%s_stats_%d","pim",ii);
      double data_sta = univPars.getParameterValue(std::string(namepar));
      data_sys /= (1.0-prt_no_inter);
      data_sta /= (1.0-prt_no_inter);
      data_cv  /= (1.0-prt_no_inter);
      vbin_datacv_pim.push_back(data_cv);
      vbin_datasys_pim.push_back(data_sys);
      vbin_datasta_pim.push_back(data_sta);
     }
    for(int ii=0;ii<MIPPbins->GetNbins_K_MIPPNuMI();ii++){
      sprintf(namepar,"MIPP_NuMI_%s_sys_%d","kap_pip",ii);
      double data_cv  = cvPars.getParameterValue(std::string(namepar));
      double data_sys = univPars.getParameterValue(std::string(namepar));
      sprintf(namepar,"MIPP_NuMI_%s_stats_%d","kap_pip",ii);
      double data_sta = univPars.getParameterValue(std::string(namepar));
      vbin_datacv_kap_pip.push_back(data_cv);
      vbin_datasys_kap_pip.push_back(data_sys);
      vbin_datasta_kap_pip.push_back(data_sta);
      
      sprintf(namepar,"MIPP_NuMI_%s_sys_%d","kam_pim",ii);
      data_cv  = cvPars.getParameterValue(std::string(namepar));
      data_sys = univPars.getParameterValue(std::string(namepar));
      sprintf(namepar,"MIPP_NuMI_%s_stats_%d","kam_pim",ii);
      data_sta = univPars.getParameterValue(std::string(namepar));
      vbin_datacv_kam_pim.push_back(data_cv);
      vbin_datasys_kam_pim.push_back(data_sys);
      vbin_datasta_kam_pim.push_back(data_sta);
    } 
    aux_par = univPars.getParameterValue("aux_parameter");
    if(aux_par<1.e-15)aux_par = 1.0;
    
  }
  MIPPNumiKaonYieldsReweighter::~MIPPNumiKaonYieldsReweighter(){
    
  }
  std::vector<bool> MIPPNumiKaonYieldsReweighter::canReweight(const InteractionChainData& aa){
    
    MIPPNumiYieldsBins*  MIPPbins =  MIPPNumiYieldsBins::getInstance();
    std::vector<bool> this_nodes;
    for(size_t ii=0;ii<(aa.interaction_chain).size();ii++){
      this_nodes.push_back(false);
    }
    
    //Look for MIPP Numi events for kaons
    //if the code find a MIPP Numi event, it will look 
    //for how many interaction nodes covers
    //if not, return all nodes false.
    //bool is_there_mipp = false;   
    TargetData tar = aa.tar_info;
    
    //Cheking if the particle is a kaon plus or kaon minus or neutral kaon:
    if(tar.Tar_pdg != 321 && tar.Tar_pdg != -321 && tar.Tar_pdg != 130 && tar.Tar_pdg != 310)return this_nodes;

    //kinematic coverage:
    if(tar.Pz<20.0 || tar.Pz>80.0 || tar.Pt>2.0)return this_nodes;

    //data:
    //Kaons:
    int binID = MIPPbins->BinID(tar.Pz,tar.Pt,tar.Tar_pdg);
    if(binID<0) return this_nodes;

    //Looking for a pion data:
    int pip_bin = MIPPbins->BinID(tar.Pz,tar.Pt, 211);
    int pim_bin = MIPPbins->BinID(tar.Pz,tar.Pt,-211);
    if(tar.Tar_pdg == 321 && pip_bin<0)return this_nodes;
    if(tar.Tar_pdg ==-321 && pim_bin<0)return this_nodes;
    if(tar.Tar_pdg == 130 || tar.Tar_pdg == 310){
      if(pip_bin<0 || pim_bin<0)return this_nodes;
    }
    
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
    
    return this_nodes;
  }
  double MIPPNumiKaonYieldsReweighter::calculateWeight(const InteractionChainData& aa){
    
    double wgt = 1.0;

    MIPPNumiYieldsBins*  MIPPbins =  MIPPNumiYieldsBins::getInstance();
    MIPPNumiMC*  MCval =  MIPPNumiMC::getInstance();
    double low_value = 1.e-18;      
    
    TargetData tar = aa.tar_info;

    //fast check:
    if(tar.Tar_pdg != 321 && tar.Tar_pdg != -321 && tar.Tar_pdg != 130 && tar.Tar_pdg != 310)return aux_par;
    if(tar.Pz<20.0 || tar.Pz>80.0 || tar.Pt>2.0)return aux_par;
    int binID = MIPPbins->BinID(tar.Pz,tar.Pt,tar.Tar_pdg);
    if(binID<0){
      return aux_par;
    }
    //Now looking for pions bin ID:
    int pip_bin = MIPPbins->BinID(tar.Pz,tar.Pt, 211);
    int pim_bin = MIPPbins->BinID(tar.Pz,tar.Pt,-211);
    if(tar.Tar_pdg == 321 && pip_bin<0)return aux_par;
    if(tar.Tar_pdg ==-321 && pim_bin<0)return aux_par;
    if(tar.Tar_pdg == 130 || tar.Tar_pdg == 310){
      if(pip_bin<0 || pim_bin<0)return aux_par;
    }
    
    //Now, looking for the MC value:
    double binC = MCval->getMCval(tar.Pz,tar.Pt,tar.Tar_pdg);
    if(binC<low_value){
      //std::cout<<"LOW MC VAL: "<<binC <<std::endl;
      return aux_par;
    }

    float K_data_cv  = -1.0;
    float K_data_sys = -1.0;
    float K_data_sta = -1.0;
    float K_data     = -1.0;
    float K_aux      = -1.0;
    
    if(tar.Tar_pdg == 321){
      K_data_cv  = vbin_datacv_pip[pip_bin] *vbin_datacv_kap_pip[binID];
      K_data_sys = vbin_datasys_pip[pip_bin]*vbin_datasys_kap_pip[binID];
      K_data_sta = vbin_datasta_pip[pip_bin]*vbin_datasta_kap_pip[binID];
      K_data = K_data_sys + K_data_sta - K_data_cv;
      
    }
    else if(tar.Tar_pdg ==-321){
      K_data_cv  = vbin_datacv_pim[pim_bin] *vbin_datacv_kam_pim[binID];
      K_data_sys = vbin_datasys_pim[pim_bin]*vbin_datasys_kam_pim[binID];
      K_data_sta = vbin_datasta_pim[pim_bin]*vbin_datasta_kam_pim[binID];
      K_data = K_data_sys + K_data_sta - K_data_cv;
    }
    else if(tar.Tar_pdg ==310 || tar.Tar_pdg ==130){
      //pip:
      K_data_cv  = vbin_datacv_pip[pip_bin] *vbin_datacv_kap_pip[binID];
      K_data_sys = vbin_datasys_pip[pip_bin]*vbin_datasys_kap_pip[binID];
      K_data_sta = vbin_datasta_pip[pip_bin]*vbin_datasta_kap_pip[binID];
      K_aux      = K_data_sys + K_data_sta - K_data_cv;
      //pim:
      K_data_cv  = vbin_datacv_pim[pim_bin] *vbin_datacv_kam_pim[binID];
      K_data_sys = vbin_datasys_pim[pim_bin]*vbin_datasys_kam_pim[binID];
      K_data_sta = vbin_datasta_pim[pim_bin]*vbin_datasta_kam_pim[binID];
      K_data     = K_data_sys + K_data_sta - K_data_cv;
      K_data    *= 3.;
      K_data    += K_aux;
      K_data    *= 0.25;
    }
    
    //check:
    if(K_data_cv<low_value || K_data_sys<low_value || K_data_sta<low_value || K_data<low_value){
      return 1.0;
    }
    
    wgt = double(K_data)/binC;
    
    if(wgt<low_value){
      //      std::cout<<"TTMIPPK check wgt(<0) "<<iUniv<<" "<<tar.Pz<<" "<<tar.Pt<<" "<<tar.Tar_pdg<<std::endl;
      return aux_par;
    }
    return wgt;

  }

}
