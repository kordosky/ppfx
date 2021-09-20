#include "ThinTargetpipCReweighter.h"
#include <iostream>
#include "ThinTargetBins.h"

namespace NeutrinoFluxReweight{
  
  ThinTargetpipCReweighter::ThinTargetpipCReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars):iUniv(iuniv),cvPars(cv_pars),univPars(univ_pars){
    
    ThinTargetBins* Thinbins =  ThinTargetBins::getInstance();
    
    
    //1 incident particles, 8 produced particles:
    
 }
 
  ThinTargetpipCReweighter::~ThinTargetpipCReweighter(){
    
  }
  bool ThinTargetpipCReweighter::canReweight(const InteractionData& aa){
    
  
    return false;
    
  }
  	
  double ThinTargetpipCReweighter::calculateWeight(const InteractionData& aa){

    double wgt = 1.0;
   
    return wgt;

  }
  

}
