#ifndef MIPPNUMIKAONSYIELDSREWEIGHTER_H
#define MIPPNUMIKAONSYIELDSREWEIGHTER_H

#include "IInteractionChainReweighting.h"

namespace NeutrinoFluxReweight{
  
  /*! \class MIPPNumiKaonsYieldsReweighter
   *  \brief Reweighter of MIPP Numi target HP extended to high energy kaons
   */
  
  class MIPPNumiKaonsYieldsReweighter : public IInteractionChainReweighting{
  public:  
    MIPPNumiKaonsYieldsReweighter();
    ~MIPPNumiKaonsYieldsReweighter();
    std::vector<bool> canReweight(const InteractionChainData& aa);
    double calculateWeight(const InteractionChainData&, ParameterTable& cv_pars, ParameterTable& univ_pars);
    void ConfigureThisUniverse(int iuniv); 
 
  private:
    int UnivID;
    
  };



  
};
#endif
