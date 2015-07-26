
#include "MIPPNumiPionYieldsReweighter.h"
#include "DataHistos.h"
#include "CentralValuesAndUncertainties.h"

#include "MIPPNumiYieldsBins.h"
#include "MIPPNumiMC.h"

#include <iostream>

namespace NeutrinoFluxReweight{
  
  MIPPNumiPionYieldsReweighter::MIPPNumiPionYieldsReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars):iUniv(iuniv),cvPars(cv_pars),univPars(univ_pars){ 
    // do any other necessary initialization    
    
}
  MIPPNumiPionYieldsReweighter::~MIPPNumiPionYieldsReweighter(){
    
  }
  std::vector<bool> MIPPNumiPionYieldsReweighter::canReweight(const InteractionChainData& aa){
 
    MIPPNumiYieldsBins*  MIPPbins =  MIPPNumiYieldsBins::getInstance();
    std::vector<bool> this_nodes;
    for(int ii=0;ii<(aa.interaction_chain).size();ii++){
      this_nodes.push_back(false);
    }
   
    //Look for MIPP Numi events
    //if the code find a MIPP Numi event, it will look 
    //for how many interaction nodes covers
    //if not, return all nodes false.
    bool is_there_mipp = false;   
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
    
    MIPPNumiYieldsBins*  MIPPbins =  MIPPNumiYieldsBins::getInstance();
    MIPPNumiMC*  MCval =  MIPPNumiMC::getInstance();
    
    DataHistos*          dtH      =  DataHistos::getInstance();
    
    TargetData tar = aa.tar_info;
    int binID = MIPPbins->BinID(tar.Pz,tar.Pt,tar.Tar_pdg);
    
    if(binID<0){
      std::cout<<"BINID ZERO:" <<tar.Pz<<" "<<tar.Pt<<" " <<tar.Tar_pdg<<std::endl;
      return 1.0;
    }
    
    const char* ptype = "pip";
    if(tar.Tar_pdg == -211)ptype = "pim"; 
    char namepar_sys[100];
    char namepar_sta[100];
    sprintf(namepar_sys,"MIPP_NuMI_%s_sys_%d",ptype,binID);
    sprintf(namepar_sta,"MIPP_NuMI_%s_stats_%d",ptype,binID);
    
    std::map<std::string, double> cv_table = cvPars.table;
    std::map<std::string, double>::iterator cv_it = cv_table.begin();
    
    cv_it = cv_table.find(std::string(namepar_sys));
    double data_cv = cv_it->second;
    
    std::map<std::string, double> this_table = univPars.table;
    std::map<std::string, double>::iterator it = this_table.begin();

    it = this_table.find(std::string(namepar_sys));
    if(it == this_table.end()){
      std::cout<<"SYS NOT FOUND: " << namepar_sys  <<std::endl;
      return 1.0;    
    }
    double data_sys = it->second;
    
    it = this_table.begin();
    it = this_table.find(std::string(namepar_sta));
    if(it == this_table.end()){
      std::cout<<"STA NOT FOUND: " << namepar_sta  <<std::endl;
      return 1.0;
    }    
    double data_sta = it->second;
    
    it = this_table.begin();
    it = this_table.find("prt_no_interacting");
    if(it == this_table.end()){
      std::cout<<"PROTON NOT INTERACTING IN THE TARGET NOT FOUND: " << "prt_no_interacting"  <<std::endl;
      return 1.0;    
    }
    double prt_no_inter = it->second;
    data_sys /= (1.0-prt_no_inter);
    data_sta /= (1.0-prt_no_inter);
    data_cv  /= (1.0-prt_no_inter);
    
    //Getting MC:
    double binC = MCval->getMCval(tar.Pz,tar.Pt,tar.Tar_pdg);
    if(binC<1.e-18){
      std::cout<<"LOW MC VAL: "<<binC <<std::endl;
      return 1.0;
    }
      
    return (data_sys + data_sta - data_cv)/binC;
    
  }

}
