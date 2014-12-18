#ifndef IINTERACTIONCHAINREWEIGHTING_H
#define IINTERACTIONCHAINREWEIGHTING_H

#include "InteractionChainData.h"
#include "ParameterTable.h"
#include <vector>

namespace NeutrinoFluxReweight{

  /*! \class InteractionChainReweightingInterface
   *  \brief The interface for classes that reweight a chain of interactions 
   * The constructor for these classes should look like myChainReweighting( ParameterTable& cv_pars, ParameterTable& univ_pars);
   * 
   */  
  class IInteractionChainReweighting{
  public:    
  
    virtual ~IInteractionChainReweighting(){};
    
    //! Look through the InteractionChainData input and identify those Interactions that can be reweighted as part of a chain. We return a vector indicating which elements will be assigned a weight by calculateWeight
    virtual std::vector<bool> canReweight(const InteractionChainData&) = 0;
    
    //! calculate a weight for this interaction chain given the central value parameters and the parameters for this universe. The weight is something like: f(cv)/f(MC) * f(univ)/f(cv) where cv in this case  corresponds to the best value of the parameter, given the data. If univ_pars=cv_pars then we are calculating a central value weight. Note, canReweight() should be called to determine which elements of the chain are covered by the weight returned by calculateWeight()
    virtual double calculateWeight(const InteractionChainData&) =0;
    
  };
  
}
#endif
