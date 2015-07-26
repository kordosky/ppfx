#ifndef NA49REWEIGHTER_H
#define NA49REWEIGHTER_H

#include "IInteractionReweighting.h"
#include <cmath>
#include <stdlib.h>

namespace NeutrinoFluxReweight{
  
  /*! \class NA49Reweighter
   *  \brief Reweighter of NA49-like interactions
   */
  
  class NA49Reweighter : public IInteractionReweighting{
  public:  
    NA49Reweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars);
    virtual ~NA49Reweighter();
    virtual bool canReweight(const InteractionData& aa);
    virtual double calculateWeight(const InteractionData& inter_data);

    const ParameterTable& cvPars;
    const ParameterTable& univPars;

  private:
    int iUniv;
    
  };

  
};
#endif
