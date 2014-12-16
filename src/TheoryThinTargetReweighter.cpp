
#include "TheoryThinTargetReweighter.h"
#include <iostream>

namespace NeutrinoFluxReweight{
  
  TheoryThinTargetReweighter::TheoryThinTargetReweighter(){
    
  }
  
   TheoryThinTargetReweighter::~TheoryThinTargetReweighter(){
    
  }
  bool TheoryThinTargetReweighter::canReweight(const InteractionData& aa){
    
    if(aa.Proc.find("Inelastic")<40){
      return true;
    }
    else return false;
    
  }
  
  double TheoryThinTargetReweighter::calculateWeight(const InteractionData& thisid, ParameterTable& cv_pars, ParameterTable& univ_pars){
    
    return 1.0;
  }
  
  void TheoryThinTargetReweighter::ConfigureThisUniverse(int iuniv){
    UnivID = iuniv;
  }

}
