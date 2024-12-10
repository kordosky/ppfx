#ifndef THINTARGETPIPCPIPMC_H
#define THINTARGETPIPCPIPMC_H

#include <utility>
#include <vector>
#include <string>
#include <iostream>
#include <iostream>
#include <sstream>

namespace NeutrinoFluxReweight{
  
  /* classThinTargetpipCpipMC brief A class to manage the MC value for  NA61 60GeV pip incident NuMI. 
   */
  class ThinTargetpipCpipMC{

  private:
   ThinTargetpipCpipMC();

  public:
    static ThinTargetpipCpipMC* getInstance();
    
    //! Read a xml file name to get the mc value for pip
    void pipC_pip_mc_from_xml(const char* filename);
    void pipC_pim_mc_from_xml(const char* filename);
    void pipC_kp_mc_from_xml(const char* filename); 
    void pipC_km_mc_from_xml(const char* filename);
    void pipC_p_mc_from_xml(const char* filename);    
    void pipC_k0s_mc_from_xml(const char* filename);
    void pipC_lam_mc_from_xml(const char* filename);
    void pipC_alam_mc_from_xml(const char* filename);
       
       //! MC value for this HP production
    double getMCval(double Prod_P,double Theta, int Inc_pdg, int Prod_pdg);

  private:    
    std::vector<double> pipC_pip_cv;
    std::vector<double> pipC_pim_cv;
    std::vector<double> pipC_kp_cv;
    std::vector<double> pipC_km_cv;
    std::vector<double> pipC_p_cv; 
    std::vector<double> pipC_k0s_cv; 
    std::vector<double> pipC_lam_cv;
    std::vector<double> pipC_alam_cv;

    
    std::vector<double> v_pmin,v_pmax,v_thetamin,v_thetamax;
    bool ranges_already_filled;
    double proton_no_interacting;

    static ThinTargetpipCpipMC* instance;
    
  };

  
  
}
#endif
