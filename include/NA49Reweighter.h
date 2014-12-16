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
    NA49Reweighter();
    ~NA49Reweighter();
    bool canReweight(const InteractionData& aa);
    double calculateWeight(const InteractionData& inter_data, ParameterTable& cv_pars, ParameterTable& univ_pars);
    void ConfigureThisUniverse(int iuniv);

  private:
     int UnivID;
  };

  
};
#endif
