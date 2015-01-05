

#include "AbsorptionReweighter.h"
#include "DataHistos.h"
#include "CentralValuesAndUncertainties.h"

#include "HistoContainer.h"
#include "ExtractInfo.h"

#include <iostream>

namespace NeutrinoFluxReweight{
  
  AbsorptionReweighter::AbsorptionReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars):iUniv(iuniv),cvPars(cv_pars),univPars(univ_pars){ 
    // do any other necessary initialization    
  }
  AbsorptionReweighter::~AbsorptionReweighter(){
    
  }
  std::vector<bool> AbsorptionReweighter::canReweight(const InteractionChainData& aa){
 
    std::vector<bool> this_nodes;
    double low_val = 1.E-20;    
    bool passIC = false;
    
    std::vector<ParticlesThroughVolumesData>  vec_ptv = aa.ptv_info;
    for(int ii=0;ii<vec_ptv.size();ii++){
      passIC = passIC || (vec_ptv[ii].DistXdens[0]>low_val);
      passIC = passIC || (vec_ptv[ii].DistXdens[1]>low_val);
      passIC = passIC || (vec_ptv[ii].DistXdens[2]>low_val);
    }
    if(passIC)this_nodes.push_back(passIC);
    return this_nodes;
  }
  double AbsorptionReweighter::calculateWeight(const InteractionChainData& aa){
    /*
    std::map<std::string, double> cv_table = cvPars.table;

    std::map<std::string, double>::iterator cv_it = cv_table.begin();
    cv_it = cv_table.find(std::string("inel_pipAl_xsec_lowP"));
    double shift_low = cv_it->second;

    cv_it = cv_table.begin();
    cv_it = cv_table.find(std::string("inel_pipAl_xsec_highP"));
    double shift_high = cv_it->second; 

    std::cout<<shift_low<<" "<<shift_high <<std::endl;
    */
    return 1.0;
    
  }

}
