
#include "ThinTargetpCNucleonReweighter.h"
#include <iostream>

namespace NeutrinoFluxReweight{
  
  ThinTargetpCNucleonReweighter::ThinTargetpCNucleonReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars):iUniv(iuniv),cvPars(cv_pars),univPars(univ_pars){
    
  }
  
   ThinTargetpCNucleonReweighter::~ThinTargetpCNucleonReweighter(){
    
  }
  bool ThinTargetpCNucleonReweighter::canReweight(const InteractionData& aa){
    
    return false;
    
  }
  
  double ThinTargetpCNucleonReweighter::calculateWeight(const InteractionData& thisid){
   return 1.0;
  }
  

}
