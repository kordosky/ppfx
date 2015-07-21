
#ifndef THINTARGETPCNUCLEONREWEIGHTER_H
#define THINTARGETPCNUCLEONREWEIGHTER_H

#include "IInteractionReweighting.h"

#include <vector>

namespace NeutrinoFluxReweight{
  
  /*! \class ThinTargetpCNucleonReweighter
   *  \brief Reweighter of thin target p,n production
   */
  
  class ThinTargetpCNucleonReweighter : public IInteractionReweighting{
  public:  
    ThinTargetpCNucleonReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars);
    virtual ~ThinTargetpCNucleonReweighter();
    virtual bool canReweight(const InteractionData& aa);
    virtual double calculateWeight(const InteractionData& inter_data);
 
  private:
    int iUniv;
    ParameterTable cvPars;
    ParameterTable univPars;
  };

  
};
#endif
