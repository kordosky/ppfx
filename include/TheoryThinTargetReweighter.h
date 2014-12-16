
#ifndef THEORYTHINTARGETREWEIGHTER_H
#define THEORYTHINTARGETREWEIGHTER_H

#include "IInteractionReweighting.h"

#include "TH2D.h"
#include "TH2F.h"
#include "TH1D.h"
#include "TH1F.h"
#include "TFile.h"

#include <vector>

namespace NeutrinoFluxReweight{
  
  /*! \class TheoryThinTargetReweighter
   *  \brief Reweighter of no NA49-like and no MIPP interactions
   */
  
  class TheoryThinTargetReweighter : public IInteractionReweighting{
  public:  
    TheoryThinTargetReweighter();
    ~TheoryThinTargetReweighter();
    bool canReweight(const InteractionData& aa);
    double calculateWeight(const InteractionData& inter_data, ParameterTable& cv_pars, ParameterTable& univ_pars);
    void ConfigureThisUniverse(int iuniv);

  private:
    int UnivID;

  };

  
};
#endif
