#ifndef MIPPNUMIYIELDSBINS_H
#define MIPPNUMIYIELDSBINS_H

#include <utility>
#include <map>
#include <vector>

namespace NeutrinoFluxReweight{
  
  /*! \class MIPPNumiYieldsBins
   *  \brief A class to manage the bin definitions for MIPP Numi Yields
   */
  class MIPPNumiYieldsBins{

  private:
    MIPPNumiYieldsBins();

  public: 
    static MIPPNumiYieldsBins* getInstance();
    
    //! Read a xml pip file name to parse the bins
    void pip_data_from_xml(const char* filename);

    //! Read a xml pim file name to parse the bins
    void pim_data_from_xml(const char* filename);

    //! Read a xml kaons over pions file name to parse the bins
    void k_pi_data_from_xml(const char* filename);

     //! Return the Bin ID for this data
    int BinID(double pz,double pt, int pdgcode);
    
    std::vector<double> pip_data_pzmin, pim_data_pzmin, k_pi_data_pzmin;
    std::vector<double> pip_data_pzmax, pim_data_pzmax, k_pi_data_pzmax;
    std::vector<double> pip_data_ptmin, pim_data_ptmin, k_pi_data_ptmin;
    std::vector<double> pip_data_ptmax, pim_data_ptmax, k_pi_data_ptmax;
        
    //Number of bins:
    int GetNbins_pip_MIPPNuMI();
    int GetNbins_pim_MIPPNuMI();
    int GetNbins_K_MIPPNuMI();
    
    private:
    static MIPPNumiYieldsBins* instance;

  };

  
  
};
#endif
