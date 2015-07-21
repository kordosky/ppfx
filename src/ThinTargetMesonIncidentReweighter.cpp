
#include "ThinTargetMesonIncidentReweighter.h"
#include <iostream>

namespace NeutrinoFluxReweight{
  
  ThinTargetMesonIncidentReweighter::ThinTargetMesonIncidentReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars):iUniv(iuniv),cvPars(cv_pars),univPars(univ_pars){
    
  }
  
   ThinTargetMesonIncidentReweighter::~ThinTargetMesonIncidentReweighter(){
    
  }
  bool ThinTargetMesonIncidentReweighter::canReweight(const InteractionData& aa){
    
    return false;
    
  }
  
  double ThinTargetMesonIncidentReweighter::calculateWeight(const InteractionData& thisid){
   return 1.0;
  }
  

}
