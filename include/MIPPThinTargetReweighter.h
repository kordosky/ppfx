#ifndef MIPPTHINTARGETREWEIGHTER_H
#define MIPPTHINTARGETREWEIGHTER_H

#include "IInteractionReweighting.h"

#include "TH2D.h"
#include "TH2F.h"
#include "TH1D.h"
#include "TH1F.h"
#include "TFile.h"

#include <vector>

namespace NeutrinoFluxReweight{
  
  /*! \class MIPPThinTargetReweighter
   *  \brief Reweighter of MIPP thin target
   */
  
  class MIPPThinTargetReweighter : public IInteractionReweighting{
  public:  
    MIPPThinTargetReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars);
    virtual ~MIPPThinTargetReweighter();
    virtual bool canReweight(const InteractionData& aa);
    virtual double calculateWeight(const InteractionData& inter_data);
    
    const ParameterTable& cvPars;
    const ParameterTable& univPars;

  private:
    int iUniv;
    

  };

  
}
#endif
