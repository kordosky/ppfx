
#ifndef THINTARGETPAREWEIGHTER_H
#define THINTARGETPAREWEIGHTER_H

#include "IInteractionReweighting.h"

#include <vector>

namespace NeutrinoFluxReweight{
  
  /*! \class ThinTargetpAReweighter
   *  \brief Reweighter of thin target pA interactions
   */
  
  class ThinTargetpAReweighter : public IInteractionReweighting{
  public:  
    ThinTargetpAReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars);
    virtual ~ThinTargetpAReweighter();
    virtual bool canReweight(const InteractionData& aa);
    virtual double calculateWeight(const InteractionData& inter_data);
 
  private:
    int iUniv;
    ParameterTable cvPars;
    ParameterTable univPars;
  };

  
};
#endif
