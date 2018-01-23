
#ifndef OTHERTREWEIGHTER_H
#define OTHERTREWEIGHTER_H

#include "IInteractionReweighting.h"

#include <vector>

namespace NeutrinoFluxReweight{
  
  /*! \class OtherReweighter
   *  \brief Reweighter of no thin target and no MIPP interactions
   */
  
  class OtherReweighter : public IInteractionReweighting{
  public:  
    OtherReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars);
    virtual ~OtherReweighter();
    virtual bool canReweight(const InteractionData& aa);
    virtual double calculateWeight(const InteractionData& inter_data);
 
    const ParameterTable& cvPars;
    const ParameterTable& univPars;

  private:
    int iUniv;
    float inel_A_scaling;
  };

  
}
#endif
