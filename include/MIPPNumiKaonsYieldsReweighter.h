#ifndef MIPPNUMIKAONSYIELDSREWEIGHTER_H
#define MIPPNUMIKAONSYIELDSREWEIGHTER_H

#include "IInteractionChainReweighting.h"

namespace NeutrinoFluxReweight{
  
  /*! \class MIPPNumiKaonsYieldsReweighter
   *  \brief Reweighter of MIPP Numi target HP extended to high energy kaons
   */
  
  class MIPPNumiKaonsYieldsReweighter : public IInteractionChainReweighting{
  public:  
    MIPPNumiKaonsYieldsReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars);
    virtual ~MIPPNumiKaonsYieldsReweighter();
    virtual std::vector<bool> canReweight(const InteractionChainData& aa);
    virtual double calculateWeight(const InteractionChainData& aa);
 
  private:
    int iUniv;
    ParameterTable cvPars;
    ParameterTable univPars;
    
  };



  
};
#endif
