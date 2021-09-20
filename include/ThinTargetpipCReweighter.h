
#ifndef THINTARGETPIPCREWEIGHTER_H
#define THINTARGETPIPCREWEIGHTER_H

#include "IInteractionReweighting.h"

#include <vector>

#if defined __clang__
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wunused-private-field"
#endif
namespace NeutrinoFluxReweight{
  
  /*! \class ThinTargetpipCReweighter
   *  \brief Reweighter of NA61 pip Interactions 
  */
  
  class ThinTargetpipCReweighter : public IInteractionReweighting{
  public:  
    ThinTargetpipCReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars);
    virtual ~ThinTargetpipCReweighter();
    virtual bool canReweight(const InteractionData& aa);
    virtual double calculateWeight(const InteractionData& aa);
    
    
  private:
    int iUniv;
    const ParameterTable& cvPars;
    const ParameterTable& univPars;
    
  };

  
}
#if defined __clang__
  #pragma clang diagnostic pop
#endif

#endif
