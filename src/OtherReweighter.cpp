
#include "OtherReweighter.h"
#include <iostream>

namespace NeutrinoFluxReweight{
  
  OtherReweighter::OtherReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars):cvPars(cv_pars),univPars(univ_pars),iUniv(iuniv){
    
    // const boost::interprocess::flat_map<std::string, double>& dsig_table = univPars.getMap();
    inel_A_scaling = univPars.getParameterValue("inel_A_scaling");
    
  }
  
   OtherReweighter::~OtherReweighter(){
    
  }
  bool OtherReweighter::canReweight(const InteractionData& aa){
    return aa.Proc.find("Inelastic") != aa.Proc.npos;
  }
  
  double OtherReweighter::calculateWeight(const InteractionData& thisid){
    
    return inel_A_scaling;
    
  }
  

}
