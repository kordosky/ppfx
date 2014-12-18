
#include "TheoryThinTargetReweighter.h"
#include <iostream>

namespace NeutrinoFluxReweight{
  
  TheoryThinTargetReweighter::TheoryThinTargetReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars):iUniv(iuniv),cvPars(cv_pars),univPars(univ_pars){
    
  }
  
   TheoryThinTargetReweighter::~TheoryThinTargetReweighter(){
    
  }
  bool TheoryThinTargetReweighter::canReweight(const InteractionData& aa){
    
    if(aa.Proc.find("Inelastic")<40){
      return true;
    }
    else return false;
    
  }
  
  double TheoryThinTargetReweighter::calculateWeight(const InteractionData& thisid){
    
    return 1.0;
  }
  

}
