#include "ReweightDriver.h"
#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace NeutrinoFluxReweight{
  
  ReweightDriver::ReweightDriver(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars, std::string fileIn)
    : iUniv(iuniv), cvPars(cv_pars), univPars(univ_pars), fileOptions(fileIn)
  {
    ParseOptions();
    Configure();
    
  }
  
  void ReweightDriver::Configure(){
    
    //Creating the vector of reweighters:
    
    MIPP_NUMI_Universe = new MIPPNumiYieldsReweighter(iUniv,cvPars,univPars);
        
    MIPP_NUMI_KAONS_Universe = new MIPPNumiKaonsYieldsReweighter(iUniv,cvPars,univPars);

    TARG_ATT_Universe = new TargetAttenuationReweighter(iUniv,cvPars,univPars);

    NA49_Universe = new NA49Reweighter(iUniv,cvPars,univPars);
    
    MIPP_THIN_Universe = new MIPPThinTargetReweighter(iUniv,cvPars,univPars);

    THEORY_Universe = new TheoryThinTargetReweighter(iUniv,cvPars,univPars);
     
    VOL_ABS_Universe = new AbsorptionReweighter(iUniv,cvPars,univPars);

  }
  
  void ReweightDriver::ParseOptions(){
    //Parsing the file input:
    using boost::property_tree::ptree;
    ptree top;
    std::string val = "";
    read_xml(fileOptions.c_str(),top,2); // option 2 removes comment strings
    ptree& options = top.get_child("inputs.FlagReweighters");

    val = options.get<std::string>("MIPPNumiYields");
    if(val=="Yes")doMIPPNumiYields = true;
    else doMIPPNumiYields = false;

    val = options.get<std::string>("TargetAttenuation");
    if(val=="Yes")doTargetAttenuation = true;
    else doTargetAttenuation = false;
    
    val = options.get<std::string>("NA49");
    if(val=="Yes")doNA49 = true;
    else  doNA49 = false;

    val = options.get<std::string>("MIPPNumiKaonsYields");
    if(val=="Yes")doMIPPNumiKaonsYields = true;
    else  doMIPPNumiKaonsYields = false;

    val = options.get<std::string>("MIPPThinTarget");
    if(val=="Yes")doMIPPThinTarget = true;
    else  doMIPPThinTarget = false;

    val = options.get<std::string>("Absorption");
    if(val=="Yes")doAbsorption = true;
    else  doAbsorption = false;
    
    val = options.get<std::string>("TheoryThinTarget");
    if(val=="Yes")doTheoryThinTarget = true;
    else  doTheoryThinTarget = false;

  }
  double ReweightDriver::calculateWeight(const InteractionChainData& icd){

    double tot_wgt = 1.0;
    
    //Boolean flags: 
    std::vector<bool> interaction_nodes;
    std::vector<bool> attenuation_nodes;
    std::vector<bool> absorption_nodes;

    //First we look at MIPP and look absorption chain:
    interaction_nodes = MIPP_NUMI_Universe->canReweight(icd);
    
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
    if(doMIPPNumiYields) tot_wgt *= mipp_wgt;

    //Looking for target attenuation correction:
    attenuation_nodes = TARG_ATT_Universe->canReweight(icd);
    double att_wgt = 1.0;
    for(int ii=0;ii<attenuation_nodes.size();ii++){
      if(attenuation_nodes[ii]==true){
	att_wgt *= TARG_ATT_Universe->calculateWeight(icd);
	break;
      }
    }
    if(doTargetAttenuation) tot_wgt *= att_wgt;
    
    //Looking for the correction of the pion absorption in volumes (Al)
    absorption_nodes = VOL_ABS_Universe->canReweight(icd);
    double abs_wgt = 1.0;
    if(attenuation_nodes[0]==true){
      abs_wgt *= VOL_ABS_Universe->calculateWeight(icd);
    }
    if(doAbsorption)tot_wgt *= abs_wgt;

    
    //Looking for MIPP kaon extension:
    double mipp_kaons_wgt = 1.0;
    if(!has_mipp){
      interaction_nodes = MIPP_NUMI_KAONS_Universe->canReweight(icd);
      for(int ii=0;ii<interaction_nodes.size();ii++){
	if(interaction_nodes[ii]==true){
	  mipp_kaons_wgt = MIPP_NUMI_KAONS_Universe->calculateWeight(icd);
	  break;
	}
      }
    }
    if(doMIPPNumiKaonsYields) tot_wgt *= mipp_kaons_wgt;
    
    //Looking for NA49:
    double na49_wgt = 1.0;
    for(int ii=0;ii<interaction_nodes.size();ii++){
      if(interaction_nodes[ii]==false){
	bool can_na49 = NA49_Universe->canReweight((icd.interaction_chain)[ii]);
	if(can_na49){
	  interaction_nodes[ii]= true;
	  na49_wgt *= NA49_Universe->calculateWeight((icd.interaction_chain)[ii]);
	}
      }
    }
    if(doNA49) tot_wgt *= na49_wgt;
    
    //Looking for thin target MIPP:
    double mipp_thin_wgt = 1.0;
    for(int ii=0;ii<interaction_nodes.size();ii++){
      if(interaction_nodes[ii]==false){
	bool can_mipp_thin = MIPP_THIN_Universe->canReweight((icd.interaction_chain)[ii]);
	if(can_mipp_thin){
	  interaction_nodes[ii]= true;
	  mipp_thin_wgt *= MIPP_THIN_Universe->calculateWeight((icd.interaction_chain)[ii]);
	}
      }
    }
    if(doMIPPThinTarget) tot_wgt *= mipp_thin_wgt;
    
    //Looking for theory (model) prediction:
    double theory_wgt = 1.0;
    for(int ii=0;ii<interaction_nodes.size();ii++){
      if(interaction_nodes[ii]==false){
	bool can_theory = THEORY_Universe->canReweight((icd.interaction_chain)[ii]);
	if(can_theory){
	  interaction_nodes[ii]= true;
	  theory_wgt *= THEORY_Universe->calculateWeight((icd.interaction_chain)[ii]);
	}
      }
    }
    if(doTheoryThinTarget) tot_wgt *= theory_wgt;

    return tot_wgt;
    
  }
  
};

