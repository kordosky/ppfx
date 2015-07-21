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
    
    //! Read a NA49 pip xml file name to parse the bins
    void pip_na49_from_xml(const char* filename);

    //! Read a NA49 pim xml file name to parse the bins
    void pim_na49_from_xml(const char* filename);

    //! Read a NA49 kap xml file name to parse the bins
    void kap_na49_from_xml(const char* filename);
    
    //! Read a NA49 kam xml file name to parse the bins
    void kam_na49_from_xml(const char* filename);
    
    //! Read a NA49 prt xml file name to parse the bins
    void prt_na49_from_xml(const char* filename);
    
    //! Read a NA49 neu xml file name to parse the bins
    void neu_na49_from_xml(const char* filename);

     //! Return the Bin ID for this data
    int BinID(double pz, double pt,int pdgcode);
    
    std::vector<double> pip_na49_pzmin, pip_na49_pzmax, pip_na49_ptmin, pip_na49_ptmax;
    std::vector<double> pim_na49_pzmin, pim_na49_pzmax, pim_na49_ptmin, pim_na49_ptmax;
    std::vector<double> kap_na49_pzmin, kap_na49_pzmax, kap_na49_ptmin, kap_na49_ptmax;
    std::vector<double> kam_na49_pzmin, kam_na49_pzmax, kam_na49_ptmin, kam_na49_ptmax;
    std::vector<double> prt_na49_pzmin, prt_na49_pzmax, prt_na49_ptmin, prt_na49_ptmax;
    std::vector<double> neu_na49_pzmin, neu_na49_pzmax, neu_na49_ptmin, neu_na49_ptmax;
    
  private:
    static ThinTargetBins* instance;
    
  };

  
  
};
#endif
