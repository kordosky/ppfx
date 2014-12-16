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
    MIPPThinTargetReweighter();
    ~MIPPThinTargetReweighter();
    bool canReweight(const InteractionData& aa);
    double calculateWeight(const InteractionData& inter_data, ParameterTable& cv_pars, ParameterTable& univ_pars);
    void ConfigureThisUniverse(int iuniv); 
  private:
    int UnivID;
    
  };

  
};
#endif
