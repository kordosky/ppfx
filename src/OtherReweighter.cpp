
#include "OtherReweighter.h"
#include <iostream>

namespace NeutrinoFluxReweight{
  
  OtherReweighter::OtherReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars):iUniv(iuniv),cvPars(cv_pars),univPars(univ_pars){
    
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

    std::map<std::string, double> dsig_table = univPars.table;
    return dsig_table.find("inel_A_scaling")->second;
  }
  

}
