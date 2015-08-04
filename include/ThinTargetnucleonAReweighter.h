
#ifndef THINTARGETNUCLEONAREWEIGHTER_H
#define THINTARGETNUCLEONAREWEIGHTER_H

#include "IInteractionReweighting.h"

#include <vector>

namespace NeutrinoFluxReweight{
  
  /*! \class ThinTargetnucleonAReweighter
   *  \brief Reweighter of thin target nucleonA interactions
   */
  
  class ThinTargetnucleonAReweighter : public IInteractionReweighting{
  public:  
    ThinTargetnucleonAReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars);
    virtual ~ThinTargetnucleonAReweighter();
    virtual bool canReweight(const InteractionData& aa);
    virtual double calculateWeight(const InteractionData& inter_data);
    const ParameterTable& cvPars;
    const ParameterTable& univPars;

  private:
    int iUniv;
    
  };

  
};
#endif
