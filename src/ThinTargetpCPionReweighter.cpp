
#include "ThinTargetpCPionReweighter.h"
#include <iostream>

namespace NeutrinoFluxReweight{
  
  ThinTargetpCPionReweighter::ThinTargetpCPionReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars):iUniv(iuniv),cvPars(cv_pars),univPars(univ_pars){
    
  }
  
   ThinTargetpCPionReweighter::~ThinTargetpCPionReweighter(){
    
  }
  bool ThinTargetpCPionReweighter::canReweight(const InteractionData& aa){
    
    return false;
    
  }
  
  double ThinTargetpCPionReweighter::calculateWeight(const InteractionData& thisid){
   return 1.0;
  }
  

}
