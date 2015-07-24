#ifndef THINTARGETMC_H
#define THINTARGETMC_H

#include "TH2D.h"
#include "TFile.h"
#include <vector>
#include <string>

namespace NeutrinoFluxReweight{
  
  /*! \class ThinTargetMC
   *  \brief A class to manage the MC value for thin target 
   */
  class ThinTargetMC{
  public:
    ThinTargetMC();
    static ThinTargetMC* getInstance();
    
    //! MC value for this HP production
    double getMCval_pC_X(double incP, double xf,double pt, int pdgcode);
    
  private:
    //convention: 0: pip. 1: pim    
    TFile* fpC_pi[2];
    std::vector<TH2D*> vpC_pi[2];
    
    std::vector<std::string> spart_prod;
    std::vector<int> mom_inc;
    
    static ThinTargetMC* instance;
    
  };

  
  
};
#endif
