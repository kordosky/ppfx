#ifndef THINTARGETPIPCPIPBINS_H
#define THINTARGETPIPCPIPBINS_H

#include <utility>
#include <map>
#include <vector>

namespace NeutrinoFluxReweight{
  
  

  class ThinTargetpipCpipBins{


  private:    
     ThinTargetpipCpipBins();


  public:
        static ThinTargetpipCpipBins* getInstance();
     
    //! Read pipC -> pip 
    void pipC_pip_from_xml(const char* filename);
 
  // Read pipC->pim
   void pipC_pim_from_xml(const char* filename);
   void pipC_kp_from_xml(const char* filename); 
   void pipC_km_from_xml(const char* filename);
   void pipC_p_from_xml(const char* filename);
   void pipC_k0s_from_xml(const char* filename);
   void pipC_lam_from_xml(const char* filename);
   void pipC_alam_from_xml(const char* filename);


   //! Get the binid of the NA6160GeV incident bin
     int pipC_pip_BinID(double Prod_P, double Theta, int Inc_pdg, int Prod_pgd);
     int pipC_pim_BinID(double Prod_P, double Theta, int Inc_pdg, int Prod_pgd);
     int pipC_kp_BinID(double Prod_P, double Theta, int Inc_pdg, int Prod_pgd);
     int pipC_km_BinID(double Prod_P, double Theta, int Inc_pdg, int Prod_pgd);
     int pipC_p_BinID(double Prod_P, double Theta, int Inc_pdg, int Prod_pgd);
     int pipC_k0s_BinID(double Prod_P, double Theta, int Inc_pdg, int Prod_pgd);
     int pipC_lam_BinID(double Prod_P, double Theta, int Inc_pdg, int Prod_pgd);
     int pipC_alam_BinID(double Prod_P, double Theta, int Inc_pdg, int Prod_pgd);
   
  //vectors to store min, max NA6160GeV
    std::vector<double> pipC_pip_pmin, pipC_pip_pmax, pipC_pip_thetamin, pipC_pip_thetamax;
    std::vector<double> pipC_pim_pmin, pipC_pim_pmax, pipC_pim_thetamin, pipC_pim_thetamax;
    std::vector<double> pipC_kp_pmin, pipC_kp_pmax, pipC_kp_thetamin, pipC_kp_thetamax;
    std::vector<double> pipC_km_pmin, pipC_km_pmax, pipC_km_thetamin, pipC_km_thetamax;
    std::vector<double> pipC_p_pmin, pipC_p_pmax, pipC_p_thetamin, pipC_p_thetamax;
    std::vector<double> pipC_k0s_pmin, pipC_k0s_pmax, pipC_k0s_thetamin, pipC_k0s_thetamax;
    std::vector<double> pipC_lam_pmin, pipC_lam_pmax, pipC_lam_thetamin, pipC_lam_thetamax;
    std::vector<double> pipC_alam_pmin, pipC_alam_pmax, pipC_alam_thetamin, pipC_alam_thetamax;
  
  
  
  
    //Number of bins:
 
    int GetNbins_pipC_pip(); 
    int GetNbins_pipC_pim();
    int GetNbins_pipC_kp();
    int GetNbins_pipC_km();
    int GetNbins_pipC_p();
    int GetNbins_pipC_k0s();
    int GetNbins_pipC_lam();
    int GetNbins_pipC_alam();
  private:
         static ThinTargetpipCpipBins* instance;
    
    
  };

  
  
}
#endif
