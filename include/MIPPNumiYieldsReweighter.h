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
    MIPPNumiYieldsReweighter();
    ~MIPPNumiYieldsReweighter();
    std::vector<bool> canReweight(const InteractionChainData& aa);
    double calculateWeight(const InteractionChainData& aa, ParameterTable& cv_pars, ParameterTable& univ_pars);
    void ConfigureThisUniverse(int iuniv); 
    
  private:
    int UnivID;
    
  };



  
};
#endif
