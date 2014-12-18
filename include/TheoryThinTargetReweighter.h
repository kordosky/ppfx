
#ifndef THEORYTHINTARGETREWEIGHTER_H
#define THEORYTHINTARGETREWEIGHTER_H

#include "IInteractionReweighting.h"

#include <vector>

namespace NeutrinoFluxReweight{
  
  /*! \class TheoryThinTargetReweighter
   *  \brief Reweighter of no NA49-like and no MIPP interactions
   */
  
  class TheoryThinTargetReweighter : public IInteractionReweighting{
  public:  
    TheoryThinTargetReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars);
    virtual ~TheoryThinTargetReweighter();
    virtual bool canReweight(const InteractionData& aa);
    virtual double calculateWeight(const InteractionData& inter_data);
 
  private:
    int iUniv;
    ParameterTable cvPars;
    ParameterTable univPars;
  };

  
};
#endif
