
#include "ThinTargetnucleonAReweighter.h"
#include <iostream>

namespace NeutrinoFluxReweight{
  
  ThinTargetnucleonAReweighter::ThinTargetnucleonAReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars):iUniv(iuniv),cvPars(cv_pars),univPars(univ_pars){
    
  }
  
   ThinTargetnucleonAReweighter::~ThinTargetnucleonAReweighter(){
    
  }
  bool ThinTargetnucleonAReweighter::canReweight(const InteractionData& aa){
    
    return false;
    
  }
  
  double ThinTargetnucleonAReweighter::calculateWeight(const InteractionData& thisid){
   return 1.0;
  }
  

}
