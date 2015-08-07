#ifndef THINTARGETBINS_H
#define THINTARGETBINS_H

#include <utility>
#include <map>
#include <vector>

namespace NeutrinoFluxReweight{
  
  /*! \class ThinTargetBins
   *  \brief A class to manage the bin definitions for MIPP Numi Yields
   */
  class ThinTargetBins{
  public:
    
    ThinTargetBins();
    static ThinTargetBins* getInstance();
    
    //! Read a data pip xml file name to parse the bins
    void pC_pi_from_xml(const char* filename);

    //!Barton:
    void barton_pC_pi_from_xml(const char* filename);
    
    //! Return the Bin ID for this data
    int BinID_pC_X(double xf, double pt,int pdgcode);
    
    //! Return the Bin ID for this data
    int barton_BinID_pC_X(double xf, double pt,int pdgcode);
    
    std::vector<double> pC_pi_xfmin, pC_pi_xfmax, pC_pi_ptmin, pC_pi_ptmax;
    std::vector<double> b_pC_pi_xfmin, b_pC_pi_xfmax, b_pC_pi_ptmin, b_pC_pi_ptmax;
    
  private:
    static ThinTargetBins* instance;
    
  };

  
  
};
#endif
