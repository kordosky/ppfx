
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
    virtual double calculateWeight(const InteractionData& aa);
    
    std::vector<float> vbin_data_pip_inc, vbin_data_pim_inc, vbin_data_kap_inc,vbin_data_kam_inc, vbin_data_k0_inc;
    
  private:
    int iUniv;
    const ParameterTable& cvPars;
    const ParameterTable& univPars;
    
  };

  
};
#endif
