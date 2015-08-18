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
    //convention: 0: pip. 1: pim.  2: kap. 3: kam. 4: prt  
    TFile* fpC_x[6];
    std::vector<TH2D*> vpC_x[5];
    std::vector<TH1D*> vpC_n;
    
    std::vector<std::string> spart_prod;
    std::vector<int> mom_inc;
    
    //convention: 0: prt. 1: neu. 
    std::vector<std::string> spart_qe_corr;
    TFile* fqe_corr[2];
    std::vector<TH2D*> vqe_corr_p;
    std::vector<TH1D*> vqe_corr_n;


    static ThinTargetMC* instance;
    
  };

  
  
};
#endif
