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
     
    VOL_ABS_IC_Universe = new AbsorptionICReweighter(iUniv,cvPars,univPars);
    VOL_ABS_DPIP_Universe = new AbsorptionDPIPReweighter(iUniv,cvPars,univPars);
    VOL_ABS_DVOL_Universe = new AbsorptionDVOLReweighter(iUniv,cvPars,univPars);

    VOL_ABS_NUCLEON_Universe = new NucleonAbsorptionOutOfTargetReweighter(iUniv,cvPars,univPars);
    VOL_ABS_OTHER_Universe = new OtherAbsorptionOutOfTargetReweighter(iUniv,cvPars,univPars);
    
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

    val = options.get<std::string>("AbsorptionIC");
    if(val=="Yes")doAbsorptionIC = true;
    else  doAbsorptionIC = false;

    val = options.get<std::string>("AbsorptionDPIP");
    if(val=="Yes")doAbsorptionDPIP = true;
    else  doAbsorptionDPIP = false;
    
    val = options.get<std::string>("AbsorptionDVOL");
    if(val=="Yes")doAbsorptionDVOL = true;
    else  doAbsorptionDVOL = false;
    
    val = options.get<std::string>("NucleonAbsorptionOutOfTarget");
    if(val=="Yes")doAbsorptionNucleon = true;
    else  doAbsorptionNucleon = false;

    val = options.get<std::string>("OtherAbsorptionOutOfTarget");
    if(val=="Yes")doAbsorptionOther = true;
    else  doAbsorptionOther = false;
    
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
    mipp_wgt = 1.0;
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

    //Looking for MIPP kaon extension:
    mipp_kaons_wgt = 1.0;
    if(!has_mipp){
      interaction_nodes = MIPP_NUMI_KAONS_Universe->canReweight(icd);
      for(int ii=0;ii<interaction_nodes.size();ii++){
	if(interaction_nodes[ii]==true){
	  has_mipp = true;
	  mipp_kaons_wgt = MIPP_NUMI_KAONS_Universe->calculateWeight(icd);
	  break;
	}
      }
    }
    if(doMIPPNumiKaonsYields) tot_wgt *= mipp_kaons_wgt;

    //Looking for target attenuation correction:
    attenuation_nodes = TARG_ATT_Universe->canReweight(icd);
    att_wgt = 1.0;
    for(int ii=0;ii<attenuation_nodes.size();ii++){
      if(attenuation_nodes[ii]==true){
	att_wgt *= TARG_ATT_Universe->calculateWeight(icd);
	break;
      }
    }
    if(doTargetAttenuation) tot_wgt *= att_wgt;
    
    //Looking for the correction of the pi/K absorption in volumes (Al)
    absorption_nodes = VOL_ABS_IC_Universe->canReweight(icd);
    abs_ic_wgt = 1.0;
    if(attenuation_nodes[0]==true){
      abs_ic_wgt *= VOL_ABS_IC_Universe->calculateWeight(icd);
    }
    if(doAbsorptionIC)tot_wgt *= abs_ic_wgt;

     //Looking for the correction of the pi/K absorption in volumes (Fe)
    absorption_nodes = VOL_ABS_DPIP_Universe->canReweight(icd);
    abs_dpip_wgt = 1.0;
    if(attenuation_nodes[0]==true){
      abs_dpip_wgt *= VOL_ABS_DPIP_Universe->calculateWeight(icd);
    }
    if(doAbsorptionDPIP)tot_wgt *= abs_dpip_wgt;

    //Looking for the correction of the pi/K absorption in volumes (He)
    absorption_nodes = VOL_ABS_DVOL_Universe->canReweight(icd);
    abs_dvol_wgt = 1.0;
    if(attenuation_nodes[0]==true){
      abs_dvol_wgt *= VOL_ABS_DVOL_Universe->calculateWeight(icd);
    }
    if(doAbsorptionDVOL)tot_wgt *= abs_dvol_wgt;

    //Looking for the correction of nucleons on Al, Fe and He.
    absorption_nodes = VOL_ABS_NUCLEON_Universe->canReweight(icd);
    abs_nucleon_wgt = 1.0;
    if(attenuation_nodes[0]==true){
      abs_nucleon_wgt *= VOL_ABS_NUCLEON_Universe->calculateWeight(icd);
    }
    if(doAbsorptionNucleon)tot_wgt *= abs_nucleon_wgt;

    //Looking for the correction of any other particle on Al, Fe and He.
    absorption_nodes = VOL_ABS_OTHER_Universe->canReweight(icd);
    abs_other_wgt = 1.0;
    if(attenuation_nodes[0]==true){
      abs_other_wgt *= VOL_ABS_OTHER_Universe->calculateWeight(icd);
    }
    if(doAbsorptionOther)tot_wgt *= abs_other_wgt;

    
    
    //Looking for NA49:
    na49_wgt = 1.0;
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
    mipp_thin_wgt = 1.0;
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
    theory_wgt = 1.0;
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

