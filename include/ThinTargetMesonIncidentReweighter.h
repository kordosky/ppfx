
#ifndef THINTARGETMESONINCIDENTREWEIGHTER_H
#define THINTARGETMESONINCIDENTREWEIGHTER_H

#include "IInteractionReweighting.h"

#include <vector>

namespace NeutrinoFluxReweight{
  
  /*! \class ThinTargetMesonIncidentReweighter
   *  \brief Reweighter of thin target meson production
   */
  
  class ThinTargetMesonIncidentReweighter : public IInteractionReweighting{
  public:  
    ThinTargetMesonIncidentReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars);
    virtual ~ThinTargetMesonIncidentReweighter();
    virtual bool canReweight(const InteractionData& aa);
    virtual double calculateWeight(const InteractionData& inter_data);
 
  private:
    int iUniv;
    ParameterTable cvPars;
    ParameterTable univPars;
  };

  
};
#endif
