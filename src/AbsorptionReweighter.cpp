

#include "AbsorptionReweighter.h"
#include "DataHistos.h"
#include "CentralValuesAndUncertainties.h"

#include "HistoContainer.h"
#include "ExtractInfo.h"

#include <iostream>

namespace NeutrinoFluxReweight{
  
  AbsorptionReweighter::AbsorptionReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars):iUniv(iuniv),cvPars(cv_pars),univPars(univ_pars){ 
    // do any other necessary initialization    
  }
  AbsorptionReweighter::~AbsorptionReweighter(){
    
  }
  std::vector<bool> AbsorptionReweighter::canReweight(const InteractionChainData& aa){
 
    std::vector<bool> this_nodes;
    
    return this_nodes;
  }
  double AbsorptionReweighter::calculateWeight(const InteractionChainData& aa){
    
    
    return 1.0;
    
  }

}
