
#ifndef OTHERTREWEIGHTER_H
#define OTHERTREWEIGHTER_H

#include "IInteractionReweighting.h"

#include <vector>

namespace NeutrinoFluxReweight{
  
  /*! \class TheoryThinTargetReweighter
   *  \brief Reweighter of no NA49-like and no MIPP interactions
   */
  
  class OtherReweighter : public IInteractionReweighting{
  public:  
    OtherReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars);
    virtual ~OtherReweighter();
    virtual bool canReweight(const InteractionData& aa);
    virtual double calculateWeight(const InteractionData& inter_data);
 
  private:
    int iUniv;
    ParameterTable cvPars;
    ParameterTable univPars;
  };

  
};
#endif