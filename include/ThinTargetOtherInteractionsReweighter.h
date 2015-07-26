
#ifndef THINTARGETOTHERINTERACTIONSREWEIGHTER_H
#define THINTARGETOTHERINTERACTIONSREWEIGHTER_H

#include "IInteractionReweighting.h"

#include <vector>

namespace NeutrinoFluxReweight{
  
  /*! \class ThinTargetOtherInteractionsReweighter
   *  \brief Reweighter of thin target any other interactions no covered yet
   */
  
  class ThinTargetOtherInteractionsReweighter : public IInteractionReweighting{
  public:  
    ThinTargetOtherInteractionsReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars);
    virtual ~ThinTargetOtherInteractionsReweighter();
    virtual bool canReweight(const InteractionData& aa);
    virtual double calculateWeight(const InteractionData& inter_data);
    const ParameterTable& cvPars;
    const ParameterTable& univPars;
  private:
    int iUniv;
    
  };

  
};
#endif
