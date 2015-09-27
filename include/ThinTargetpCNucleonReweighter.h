
#ifndef THINTARGETPCNUCLEONREWEIGHTER_H
#define THINTARGETPCNUCLEONREWEIGHTER_H

#include "IInteractionReweighting.h"

#include <vector>

namespace NeutrinoFluxReweight{
  
  /*! \class ThinTargetpCNucleonReweighter
   *  \brief Reweighter of thin target p,n production
   */
  
  class ThinTargetpCNucleonReweighter : public IInteractionReweighting{
  public:  
    ThinTargetpCNucleonReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars);
    virtual ~ThinTargetpCNucleonReweighter();
    virtual bool canReweight(const InteractionData& aa);
    virtual double calculateWeight(const InteractionData& aa);
    
    double calculateDataScale(int inc_pdg, double inc_mom, int prod_pdg,double xf, double pt);

    double data_prod_xs;
    std::vector<float> vbin_data_p, vbin_data_n;
    
  private:
    int iUniv;
    const ParameterTable& cvPars;
    const ParameterTable& univPars;

  };

  
};
#endif
