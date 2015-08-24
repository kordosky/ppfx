
#ifndef THINTARGETNUCLEONAREWEIGHTER_H
#define THINTARGETNUCLEONAREWEIGHTER_H

#include "IInteractionReweighting.h"
#include "ThinTargetpCPionReweighter.h"
#include "ThinTargetnCPionReweighter.h"
#include "ThinTargetpCKaonReweighter.h"

#include <vector>

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
    
  private:
    int iUniv;
    const ParameterTable& cvPars;
    const ParameterTable& univPars;
    
    ThinTargetpCPionReweighter*    tt_pCPionRew;
    ThinTargetnCPionReweighter*    tt_nCPionRew;
    ThinTargetpCKaonReweighter*    tt_pCKaonRew;

    InteractionData* aux_aa;

  };

  
};
#endif
