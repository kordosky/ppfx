#ifndef ATTENUATIONREWEIGHTER_H
#define ATTENUATIONREWEIGHTER_H

#include "IInteractionChainReweighting.h"

namespace NeutrinoFluxReweight{
  
  /*! \class AttenuationReweighter
   *  \brief Reweight to account for attenuation of the beam in the target
   *
   * If the MC does not get the reaction cross-section correct the number of interactions in the target, the number of 
   * primary protons which do not interact in the target, and the distribution of interactions along z are affected. MIPP
   * measures yields per incoming proton, not per interaction, so their measurment includes the probability of interaction
   * and also the yield per interaction.
   */
  
  class AttenuationReweighter : public IInteractionChainReweighting{
  public:  
    AttenuationReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars);
    virtual ~AttenuationReweighter();
    virtual std::vector<bool> canReweight(const InteractionChainData& aa);
    virtual double calculateWeight(const InteractionChainData&);

  private:
    int iUniv;
    ParameterTable cvPars;
    ParameterTable univPars;
  };



  
};
#endif
