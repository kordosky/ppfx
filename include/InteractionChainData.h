#ifndef INTERACTIONCHAINDATA_H
#define INTERACTIONCHAINDATA_H

#include "InteractionData.h"
#include "TargetData.h"
#include <vector>

namespace NeutrinoFluxReweight{
  
  /*! \class InteractionChainData
   *  \brief Information about the chain of interactions leading to a neutrino 
  */

  class InteractionChainData{
  public:
    //! vector of neutrino ancestors 
    std::vector<InteractionData> interaction_chain;
    
    //! Target Data
    TargetData tar_info; 
  };
  
  
};
#endif
