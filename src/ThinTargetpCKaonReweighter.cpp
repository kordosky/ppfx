
#include "ThinTargetpCKaonReweighter.h"
#include <iostream>

namespace NeutrinoFluxReweight{
  
  ThinTargetpCKaonReweighter::ThinTargetpCKaonReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars):iUniv(iuniv),cvPars(cv_pars),univPars(univ_pars){
    
  }
  
  ThinTargetpCKaonReweighter::~ThinTargetpCKaonReweighter(){
    
  }
  bool ThinTargetpCKaonReweighter::canReweight(const InteractionData& aa){
    
    return false;
    
  }
  
  double ThinTargetpCKaonReweighter::calculateWeight(const InteractionData& thisid){
   return 1.0;
  }
  

}
