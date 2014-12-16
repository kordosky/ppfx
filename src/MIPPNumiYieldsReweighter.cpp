

#include "MIPPNumiYieldsReweighter.h"
#include "DataHistos.h"
#include "CentralValuesAndUncertainties.h"

#include "MIPPNumiYieldsBins.h"
#include "HistoContainer.h"
#include "ExtractInfo.h"

#include <iostream>

namespace NeutrinoFluxReweight{
  
  MIPPNumiYieldsReweighter::MIPPNumiYieldsReweighter(){
    
  }
  MIPPNumiYieldsReweighter::~MIPPNumiYieldsReweighter(){
    
  }
  std::vector<bool> MIPPNumiYieldsReweighter::canReweight(const InteractionChainData& aa){
 
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

    //In a quick counting , I missed 0.4% events.
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
  double MIPPNumiYieldsReweighter::calculateWeight(const InteractionChainData& aa, ParameterTable& cv_pars, ParameterTable& univ_pars){
    CentralValuesAndUncertainties*  CVU = CentralValuesAndUncertainties::getInstance();
    CVU->setBaseSeed(99);
    
    ParameterTable this_parTable = CVU->calculateParsForUniverse(UnivID);
    
    MIPPNumiYieldsBins*  MIPPbins =  MIPPNumiYieldsBins::getInstance();
    DataHistos*          dtH      =  DataHistos::getInstance();
    HistoContainer*      histos   =  HistoContainer::getInstance();
    ExtractInfo*         info     =  ExtractInfo::getInstance();
    
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
    
    ////////// Testing getting thr cv and substract:
    ParameterTable cv_parTable   = CVU->getCVPars();
    std::map<std::string, double> cv_table = cv_parTable.table;
    std::map<std::string, double>::iterator cv_it = cv_table.begin();
    cv_it = cv_table.find(std::string(namepar_sys));
    double data_cv = cv_it->second;

    //for plots: data_cv +- 50% of the cv.
    double plot_min = 0.5*data_cv;
    double plot_max = 1.5*data_cv;
 
    //////////

    std::map<std::string, double> this_table = this_parTable.table;
    std::map<std::string, double>::iterator it = this_table.begin();

    it = this_table.find(std::string(namepar_sys));
    if(it == this_table.end()){
      std::cout<<"SYS NOT FOUND:" << namepar_sys  <<std::endl;
      return 1.0;    
    }
    double data_sys = it->second;
    
    histos->plot1D(data_sys,"wgt_" + std::string(namepar_sys),"",1000,plot_min,plot_max,1.0);

    it = this_table.begin();
    it = this_table.find(std::string(namepar_sta));
    if(it == this_table.end()){
      std::cout<<"STA NOT FOUD:" << namepar_sta  <<std::endl;
      return 1.0;
    }    
    double data_sta = it->second;
    histos->plot1D(data_sta,"wgt_" + std::string(namepar_sta),"",1000,plot_min,plot_max,1.0);
    
    //Getting MC:
    int i_use = 0;
    if(tar.Tar_pdg == -211)i_use = 1;
    double binC = dtH->hMIPP_MC[i_use]->GetBinContent(dtH->hMIPP_MC[i_use]->FindBin(tar.Pz,tar.Pt) );
    if(binC<1.e-18){
      std::cout<<"LOW MC VAL: "<<binC <<std::endl;
      return 1.0;
    }
    
    ///Filling info:
    char cunivID[3]; 
    sprintf(cunivID,"%03d",UnivID);
    info->FillInfo("univ" + std::string(cunivID) + "_" + std::string(namepar_sys),data_sys/binC);
    info->FillInfo("univ" + std::string(cunivID) + "_" + std::string(namepar_sta),data_sta/binC);
    ///

    
    return (data_sys + data_sta - data_cv)/binC;
    
  }
  void MIPPNumiYieldsReweighter::ConfigureThisUniverse(int iuniv){
    UnivID = iuniv;
  }
}
