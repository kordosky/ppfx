
#include "OtherReweighter.h"
#include <iostream>

namespace NeutrinoFluxReweight{
  
  OtherReweighter::OtherReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars):iUniv(iuniv),cvPars(cv_pars),univPars(univ_pars){
    
    std::map<std::string, double> dsig_table = univPars.table;
    inel_A_scaling = dsig_table["inel_A_scaling"];
    
  }
  
   OtherReweighter::~OtherReweighter(){
    
  }
  bool OtherReweighter::canReweight(const InteractionData& aa){
 
    if(aa.Proc.find("Inelastic")<100){
      return true;
    }
    else return false;
    
  }
  
  double OtherReweighter::calculateWeight(const InteractionData& thisid){
    
    return inel_A_scaling;
    
  }
  

}
