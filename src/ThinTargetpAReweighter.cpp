
#include "ThinTargetpAReweighter.h"
#include <iostream>

namespace NeutrinoFluxReweight{
  
  ThinTargetpAReweighter::ThinTargetpAReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars):iUniv(iuniv),cvPars(cv_pars),univPars(univ_pars){
    
  }
  
   ThinTargetpAReweighter::~ThinTargetpAReweighter(){
    
  }
  bool ThinTargetpAReweighter::canReweight(const InteractionData& aa){
    
    return false;
    
  }
  
  double ThinTargetpAReweighter::calculateWeight(const InteractionData& thisid){
   return 1.0;
  }
  

}
