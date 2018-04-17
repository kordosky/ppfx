
#ifndef OTHERTREWEIGHTER_H
#define OTHERTREWEIGHTER_H

#include "IInteractionReweighting.h"

#include <vector>

#if defined __clang__
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wunused-private-field"
#endif
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
#if defined __clang__
  #pragma clang diagnostic pop
#endif

#endif
