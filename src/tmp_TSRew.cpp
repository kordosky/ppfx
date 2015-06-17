
#include "tmp_TSRew.h"
#include <iostream>
#include <locale>
#include <cstdlib>
#include <stdexcept>
#include <cstdlib>
#include "DataHistos.h"

namespace NeutrinoFluxReweight{
  
  tmp_TSRew::tmp_TSRew(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars)
    :iUniv(iuniv),cvPars(cv_pars),univPars(univ_pars){
    
  }
  tmp_TSRew::~tmp_TSRew(){
    
  }
  
  std::vector<bool> tmp_TSRew::canReweight(const InteractionChainData& aa){
    std::vector<bool> can_rws;
    std::vector<InteractionData> vec_inter = aa.interaction_chain;
    bool this_survive = false;
    double minZ = 40.0; //cm
    bool died_out  = vec_inter[0].Vol != "BudalMonitor" && vec_inter[0].Vol != "TGT1" && vec_inter[0].Vtx[2]>minZ; 
    if(died_out)this_survive = true; 
    can_rws.push_back(this_survive);
    return can_rws;
  }
  double tmp_TSRew::calculateWeight(const InteractionChainData& aa){
    
    DataHistos*          dtH      =  DataHistos::getInstance();
    std::map<std::string, double> this_table = univPars.table;
    std::map<std::string, double>::iterator it;
    
    double minZ = 40.0; //cm
    double low_number = 1.e-10;
    double sur_wgt = 1.0;
    std::vector<InteractionData> vec_inter = aa.interaction_chain;
    int ninter = vec_inter.size();
    double startZ_target = -45.0; //mm
    double startZ_budal  = -61.72; 
    double avog_n = 6.02214e23; //atoms C12
    double mb_cm2       = 1.e-27;
    double rho_C12     = 8.9254e22;
    double rho_Target  = 8.79631e22;
    double mb_cm2_AND_rho_Target = 8.79631e-5;
    double dist_travel = 96.0;
    std::string  par_name = "";
    
    bool died_out  = vec_inter[0].Vol != "BudalMonitor" && vec_inter[0].Vol != "TGT1" && vec_inter[0].Vtx[2]>minZ; 
    if(!died_out)return sur_wgt;
    double delta_sigma = 0;
    par_name = "prod_prtC_xsec";
    it = this_table.begin();
    it = this_table.find(par_name);
    delta_sigma  = it->second;
    
    par_name = "qe_prtC_xsec";
    it = this_table.begin();
    it = this_table.find(par_name);
    delta_sigma  += it->second; 
    
    int binprtC = (dtH->hXS_prtC)->FindBin(120.); //120GeV, energy of the primary proton beam.
    double mcval = (dtH->hXS_prtC)->GetBinContent(binprtC);
    if(mcval<low_number){
      std::cout<< "MC==0 in tmp_TWRew... check!"  <<std::endl;
      return sur_wgt;
    }
    
    delta_sigma -= mcval;
    sur_wgt = exp(-1.0*dist_travel*delta_sigma*mb_cm2_AND_rho_Target);
    
    return sur_wgt;
     
  }
  
}
