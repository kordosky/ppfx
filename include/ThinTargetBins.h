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
    void pip_data_from_xml(const char* filename);

    //! Read a data pim xml file name to parse the bins
    void pim_data_from_xml(const char* filename);

    //! Read a data kap xml file name to parse the bins
    void kap_data_from_xml(const char* filename);
    
    //! Read a data kam xml file name to parse the bins
    void kam_data_from_xml(const char* filename);
    
    //! Read a data prt xml file name to parse the bins
    void prt_data_from_xml(const char* filename);
    
    //! Read a data neu xml file name to parse the bins
    void neu_data_from_xml(const char* filename);

     //! Return the Bin ID for this data
    int BinID(double pz, double pt,int pdgcode);
    
    std::vector<double> pip_data_pzmin, pip_data_pzmax, pip_data_ptmin, pip_data_ptmax;
    std::vector<double> pim_data_pzmin, pim_data_pzmax, pim_data_ptmin, pim_data_ptmax;
    std::vector<double> kap_data_pzmin, kap_data_pzmax, kap_data_ptmin, kap_data_ptmax;
    std::vector<double> kam_data_pzmin, kam_data_pzmax, kam_data_ptmin, kam_data_ptmax;
    std::vector<double> prt_data_pzmin, prt_data_pzmax, prt_data_ptmin, prt_data_ptmax;
    std::vector<double> neu_data_pzmin, neu_data_pzmax, neu_data_ptmin, neu_data_ptmax;
    
  private:
    static ThinTargetBins* instance;
    
  };

  
  
};
#endif
