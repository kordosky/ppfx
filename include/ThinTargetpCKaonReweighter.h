
#ifndef THINTARGETPCKAONREWEIGHTER_H
#define THINTARGETPCKAONREWEIGHTER_H

#include "IInteractionReweighting.h"

#include <vector>

namespace NeutrinoFluxReweight{
  
  /*! \class ThinTargetpCKaonReweighter
   *  \brief Reweighter of thin target K production
   */
  
  class ThinTargetpCKaonReweighter : public IInteractionReweighting{
  public:  
    ThinTargetpCKaonReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars);
    virtual ~ThinTargetpCKaonReweighter();
    virtual bool canReweight(const InteractionData& aa);
    virtual double calculateWeight(const InteractionData& inter_data);
 
  private:
    int iUniv;
    ParameterTable cvPars;
    ParameterTable univPars;
  };

  
};
#endif
