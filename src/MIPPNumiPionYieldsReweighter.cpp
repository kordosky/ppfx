
#include "MIPPNumiPionYieldsReweighter.h"
#include "CentralValuesAndUncertainties.h"

#include "MIPPNumiYieldsBins.h"
#include "MIPPNumiMC.h"

#include <iostream>

namespace NeutrinoFluxReweight{
  
  MIPPNumiPionYieldsReweighter::MIPPNumiPionYieldsReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars):cvPars(cv_pars),univPars(univ_pars),iUniv(iuniv){ 

     MIPPNumiYieldsBins* MIPPbins =  MIPPNumiYieldsBins::getInstance();
     vbin_data_pip.reserve(MIPPbins->GetNbins_pip_MIPPNuMI());
     vbin_data_pim.reserve(MIPPbins->GetNbins_pim_MIPPNuMI());
     
     //const boost::interprocess::flat_map<std::string, double>& cv_table = cvPars.getMap();
     //const boost::interprocess::flat_map<std::string, double>& univ_table = univPars.getMap();
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
      vbin_data_pip.push_back(data_sys + data_sta - data_cv);
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
      vbin_data_pim.push_back(data_sys + data_sta - data_cv);
    }

    
}
  MIPPNumiPionYieldsReweighter::~MIPPNumiPionYieldsReweighter(){
    
  }
  std::vector<bool> MIPPNumiPionYieldsReweighter::canReweight(const InteractionChainData& aa){
 
    MIPPNumiYieldsBins*  MIPPbins =  MIPPNumiYieldsBins::getInstance();
    std::vector<bool> this_nodes;
    for(size_t ii=0;ii<(aa.interaction_chain).size();ii++){
      this_nodes.push_back(false);
    }
   
    //Look for MIPP Numi events
    //if the code find a MIPP Numi event, it will look 
    //for how many interaction nodes covers
    //if not, return all nodes false.
    //bool is_there_mipp = false;   
    TargetData tar = aa.tar_info;
    //Cheking if the particle is a pion plus and pion minus:
    if(tar.Tar_pdg != 211 && tar.Tar_pdg != -211)return this_nodes;
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
      //  std::cout<<"==>>SOMETHING WEIRD WITH MIPP NUMI "<<tar.Idx_ancestry<<" "<<tar.Tar_pdg<<std::endl;
    }
    
    return this_nodes;
  }
  double MIPPNumiPionYieldsReweighter::calculateWeight(const InteractionChainData& aa){
    
    double wgt = 1.0;
    
    MIPPNumiYieldsBins*  MIPPbins =  MIPPNumiYieldsBins::getInstance();
    MIPPNumiMC*  MCval =  MIPPNumiMC::getInstance();    
    
    TargetData tar = aa.tar_info;
    int binID = MIPPbins->BinID(tar.Pz,tar.Pt,tar.Tar_pdg);
    
    if(binID<0){
      std::cout<<"BINID ZERO:" <<tar.Pz<<" "<<tar.Pt<<" " <<tar.Tar_pdg<<std::endl;
      return 1.0;
    }
    
    //Getting MC:
    double binC = MCval->getMCval(tar.Pz,tar.Pt,tar.Tar_pdg);
    if(binC<1.e-18){
      std::cout<<"LOW MC VAL: "<<binC <<std::endl;
      return 1.0;
    }
    
    if(tar.Tar_pdg == 211)wgt = vbin_data_pip[binID]/binC;
    if(tar.Tar_pdg ==-211)wgt = vbin_data_pim[binID]/binC;
    
    if(wgt<0)std::cout<<"TTMIPPPI check wgt(<0) "<<iUniv<<" "<<tar.Pz<<" "<<tar.Pt<<" "<<tar.Tar_pdg<<std::endl;
    return wgt;
    
  }

}
