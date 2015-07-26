
#ifndef THINTARGETPCPIONREWEIGHTER_H
#define THINTARGETPCPIONREWEIGHTER_H

#include "IInteractionReweighting.h"
#include "ParameterTable.h"

namespace NeutrinoFluxReweight{
  
  /*! \class ThinTargetpCPionReweighter
   *  \brief Reweighter of thin target pion production
   */
  
  class ThinTargetpCPionReweighter : public IInteractionReweighting{
  public:  
    ThinTargetpCPionReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars);
    virtual ~ThinTargetpCPionReweighter();
    virtual bool canReweight(const InteractionData& aa);
    virtual double calculateWeight(const InteractionData& inter_data);
    const ParameterTable& cvPars;
    const ParameterTable& univPars;

  private:
    int iUniv;
    
  };

  
};
#endif
