
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
    
    std::vector<float> vbin_pip_inc_pip, vbin_pip_inc_pim, vbin_pip_inc_kap, vbin_pip_inc_kam, vbin_pip_inc_k0, vbin_pip_inc_p, vbin_pip_inc_n;
    std::vector<float> vbin_pim_inc_pip, vbin_pim_inc_pim, vbin_pim_inc_kap, vbin_pim_inc_kam, vbin_pim_inc_k0, vbin_pim_inc_p, vbin_pim_inc_n;
    std::vector<float> vbin_kap_inc_pip, vbin_kap_inc_pim, vbin_kap_inc_kap, vbin_kap_inc_kam, vbin_kap_inc_k0, vbin_kap_inc_p, vbin_kap_inc_n;
    std::vector<float> vbin_kam_inc_pip, vbin_kam_inc_pim, vbin_kam_inc_kap, vbin_kam_inc_kam, vbin_kam_inc_k0, vbin_kam_inc_p, vbin_kam_inc_n;
    std::vector<float> vbin_k0_inc_pip,  vbin_k0_inc_pim,  vbin_k0_inc_kap,  vbin_k0_inc_kam,  vbin_k0_inc_k0,  vbin_k0_inc_p,  vbin_k0_inc_n;
    float bin_mesonleftover_inc;
  private:
    int iUniv;
    const ParameterTable& cvPars;
    const ParameterTable& univPars;
    
  };

  
}
#endif
