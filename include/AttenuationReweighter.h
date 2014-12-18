#ifndef ATTENUATIONREWEIGHTER_H
#define ATTENUATIONREWEIGHTER_H

#include "IInteractionChainReweighting.h"

namespace NeutrinoFluxReweight{
  
  /*! \class AttenuationReweighter
   *  \brief Reweighter of beam attenuation 
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
