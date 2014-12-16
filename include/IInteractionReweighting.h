#ifndef IINTERACTIONREWEIGHTING_H
#define IINTERACTIONREWEIGHTING_H

#include "InteractionData.h"
#include "ParameterTable.h"

namespace NeutrinoFluxReweight{


  /** \class InteractionReweightingInterface
   * \brief This is the interface for classes that reweight interactions.  
   */ 
  class IInteractionReweighting{
  public:    
    virtual ~IInteractionReweighting(){};
    
    //! can the particular instance of this class reweight this interaction?  
    virtual bool canReweight(const InteractionData& aa) = 0;
    
    //! calculate a weight for this interaction given the central value parameters and the parameters for this universe. The weight is something like: f(cv)/f(MC) * f(univ)/f(cv)  where cv in this case  corresponds to the best value of the parameter, given the data. If univ_pars=cv_pars then we are calculating a central value weight 
    virtual double calculateWeight(const InteractionData& inter_data, ParameterTable& cv_pars, ParameterTable& univ_pars) =0;
    virtual void ConfigureThisUniverse(int iuniv) = 0; 
  };
  
}
#endif
