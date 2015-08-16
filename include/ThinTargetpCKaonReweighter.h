
#ifndef THINTARGETPCKAONREWEIGHTER_H
#define THINTARGETPCKAONREWEIGHTER_H

#include "IInteractionReweighting.h"
#include "ThinTargetpCPionReweighter.h"

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
    virtual double calculateWeight(const InteractionData& aa);
    
    double data_prod_xs;
    std::vector<float> vbin_data_kap,vbin_data_kam;
    std::vector<float> mipp_vbin_data_kap_pip, mipp_vbin_data_kam_pim;
    double calculateDataScale(int inc_pdg, double inc_mom, int prod_pdg,double xf, double pt);
    double calculateMCProd(int genid, double inc_mom);
    
  private:
    int iUniv;
    const ParameterTable& cvPars;
    const ParameterTable& univPars;
    ThinTargetpCPionReweighter* tt_pCPionRew;
    InteractionData* aux_aa;
    
  };

  
};
#endif
