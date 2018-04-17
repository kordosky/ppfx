
#ifndef THINTARGETNUCLEONAREWEIGHTER_H
#define THINTARGETNUCLEONAREWEIGHTER_H

#include "IInteractionReweighting.h"
#include "ThinTargetpCPionReweighter.h"
#include "ThinTargetnCPionReweighter.h"
#include "ThinTargetpCKaonReweighter.h"

#include <vector>

#if defined __clang__
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wunused-private-field"
#endif
namespace NeutrinoFluxReweight{
  
  /*! \class ThinTargetnucleonAReweighter
   *  \brief Reweighter of thin target nucleonA interactions
   */
  
  class ThinTargetnucleonAReweighter : public IInteractionReweighting{
  public:  
    ThinTargetnucleonAReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars);
    virtual ~ThinTargetnucleonAReweighter();
    virtual bool canReweight(const InteractionData& aa);
    virtual double calculateWeight(const InteractionData& aa);
    std::vector<float> vbin_data_pip, vbin_data_pim, vbin_data_kap,vbin_data_kam;
    double data_prod_xs;
    std::vector<float> vbin_prt_inc_pip, vbin_prt_inc_pim, vbin_prt_inc_kap, vbin_prt_inc_kam, vbin_prt_inc_k0, vbin_prt_inc_p, vbin_prt_inc_n;
    std::vector<float> vbin_neu_inc_pip, vbin_neu_inc_pim, vbin_neu_inc_kap, vbin_neu_inc_kam, vbin_neu_inc_k0, vbin_neu_inc_p, vbin_neu_inc_n;
    float bin_prtleftover_inc, bin_neuleftover_inc;
  private:
    int iUniv;
    const ParameterTable& cvPars;
    const ParameterTable& univPars;
    
    ThinTargetpCPionReweighter*    tt_pCPionRew;
    ThinTargetnCPionReweighter*    tt_nCPionRew;
    ThinTargetpCKaonReweighter*    tt_pCKaonRew;

    // InteractionData* aux_aa;

  };

  
}
#if defined __clang__
  #pragma clang diagnostic pop
#endif

#endif
