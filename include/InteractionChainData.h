#ifndef INTERACTIONCHAINDATA_H
#define INTERACTIONCHAINDATA_H

#include "InteractionData.h"
#include "TargetData.h"
#include <vector>
#include "nu_g4numi.h" 

namespace NeutrinoFluxReweight{
  
  /*! \class InteractionChainData
   *  \brief Information about the chain of interactions leading to a neutrino 
  */

  class InteractionChainData{
  public:

    //! boring old default constructor
    InteractionChainData();

    //! create an interaction chain from the old g4numi nudata structure
    InteractionChainData(nu_g4numi* nu);

    //! vector of neutrino ancestors 
    std::vector<InteractionData> interaction_chain;
    
    //! Target Data
    TargetData tar_info; 
  };
  
  
};
#endif
