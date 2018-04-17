
#ifndef THINTARGETNCPIONREWEIGHTER_H
#define THINTARGETNCPIONREWEIGHTER_H

#include "IInteractionReweighting.h"
#include "ThinTargetpCPionReweighter.h"

#include <vector>

#if defined __clang__
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wunused-private-field"
#endif
namespace NeutrinoFluxReweight{
  
  /*! \class ThinTargetnCPionReweighter
   *  \brief Reweighter of thin target nC interactions
   */
  
  class ThinTargetnCPionReweighter : public IInteractionReweighting{
  public:  
    ThinTargetnCPionReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars);
    virtual ~ThinTargetnCPionReweighter();
    virtual bool canReweight(const InteractionData& aa);
    virtual double calculateWeight(const InteractionData& aa);
    

  private:
    int iUniv;
    const ParameterTable& cvPars;
    const ParameterTable& univPars;

    ThinTargetpCPionReweighter* tt_pCPionRew;
    // InteractionData* aux_aa;
    
  };

  
}
#if defined __clang__
  #pragma clang diagnostic pop
#endif

#endif
