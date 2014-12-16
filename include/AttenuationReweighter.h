#ifndef ATTENUATIONREWEIGHTER_H
#define ATTENUATIONREWEIGHTER_H

#include "IInteractionChainReweighting.h"

namespace NeutrinoFluxReweight{
  
  /*! \class AttenuationReweighter
   *  \brief Reweighter of beam attenuation 
   */
  
  class AttenuationReweighter : public IInteractionChainReweighting{
  public:  
    AttenuationReweighter();
    ~AttenuationReweighter();
    std::vector<bool> canReweight(const InteractionChainData& aa);
    double calculateWeight(const InteractionChainData&, ParameterTable& cv_pars, ParameterTable& univ_pars);
     void ConfigureThisUniverse(int iuniv);

  private:
     int UnivID; 
  };



  
};
#endif
