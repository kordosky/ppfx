#ifndef MIPPNUMIYIELDSREWEIGHTER_H
#define MIPPNUMIYIELDSREWEIGHTER_H

#include "IInteractionChainReweighting.h"
#include "ParameterTable.h"

namespace NeutrinoFluxReweight{
  
  /*! \class MIPPNumiYieldsReweighter
   *  \brief Reweighter of MIPP Numi target HP
   */
  
  class MIPPNumiYieldsReweighter : public IInteractionChainReweighting{
  public:  
    MIPPNumiYieldsReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars);
    virtual ~MIPPNumiYieldsReweighter();
    virtual std::vector<bool> canReweight(const InteractionChainData& aa);
    virtual double calculateWeight(const InteractionChainData& aa);    
  private:
    int iUniv;
    ParameterTable cvPars;
    ParameterTable univPars;

  };



  
};
#endif
