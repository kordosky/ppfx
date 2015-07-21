
#include "ThinTargetOtherInteractionsReweighter.h"
#include <iostream>

namespace NeutrinoFluxReweight{
  
  ThinTargetOtherInteractionsReweighter::ThinTargetOtherInteractionsReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars):iUniv(iuniv),cvPars(cv_pars),univPars(univ_pars){
    
  }
  
  ThinTargetOtherInteractionsReweighter::~ThinTargetOtherInteractionsReweighter(){
    
  }
  bool ThinTargetOtherInteractionsReweighter::canReweight(const InteractionData& aa){
    
    return false;
    
  }
  
  double ThinTargetOtherInteractionsReweighter::calculateWeight(const InteractionData& thisid){
   return 1.0;
  }
  

}
