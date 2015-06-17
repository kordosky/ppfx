
#include "tmp_TARew.h"
#include <iostream>
#include <locale>
#include <cstdlib>
#include <stdexcept>
#include <cstdlib>
#include "DataHistos.h"

namespace NeutrinoFluxReweight{
  
  tmp_TARew::tmp_TARew(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars)
    :iUniv(iuniv),cvPars(cv_pars),univPars(univ_pars){
    
  }
  tmp_TARew::~tmp_TARew(){
    
  }
  std::vector<bool> tmp_TARew::canReweight(const InteractionChainData& aa){
    std::vector<bool> can_rws;
    std::vector<InteractionData> vec_inter = aa.interaction_chain;
    bool is_there_pC = false;
    if(vec_inter[0].Vol == "BudalMonitor" || vec_inter[0].Vol == "TGT1")is_there_pC = true;
    can_rws.push_back(is_there_pC);
    return can_rws;
  }
  double tmp_TARew::calculateWeight(const InteractionChainData& aa){
    
    DataHistos*          dtH      =  DataHistos::getInstance();
    std::map<std::string, double> this_table = univPars.table;
    std::map<std::string, double>::iterator it;
    
    double low_number = 1.e-10;
    double att_wgt = 1.0;
    std::vector<InteractionData> vec_inter = aa.interaction_chain;
    int ninter = vec_inter.size();
    double startZ_target = -45.0; //mm
    double startZ_budal  = -61.72; 
    double avog_n = 6.02214e23; //atoms C12
    double mb_cm2       = 1.e-27;
    double rho_C12     = 8.9254e22;
    double rho_Target  = 8.79631e22;
    double mb_cm2_AND_rho_Target = 8.79631e-5;
    
    //check:
    if(vec_inter[0].Vol != "BudalMonitor" && vec_inter[0].Vol != "TGT1")return att_wgt;
    
    //Distance:
    double dist_travel = 0;
    if(vec_inter[0].Vol == "BudalMonitor") dist_travel = (vec_inter[0]).Vtx[2] - startZ_budal;
    else if(vec_inter[0].Vol == "TGT1")   dist_travel = (vec_inter[0]).Vtx[2] - startZ_target + 2.0;
    if(dist_travel < low_number)return att_wgt;

    //cross section:
    double delta_sigma = 0.0; // sigma_data - sigma_mc
    double ratio_sigma = 0.0; // sigma_data / sigma_mc
    std::string par_name = "";
    
    par_name = "prod_prtC_xsec";
    it = this_table.begin();
    it = this_table.find(par_name);
    delta_sigma  = it->second;
	
    par_name = "qe_prtC_xsec";
    it = this_table.begin();
    it = this_table.find(par_name);
    delta_sigma  += it->second; 
    
    int binprtC = (dtH->hXS_prtC)->FindBin(vec_inter[0].Inc_P);
    double mcval = (dtH->hXS_prtC)->GetBinContent(binprtC);
    if(mcval<low_number){
      std::cout<< "MC==0 in tmp_TWRew... check!"  <<std::endl;
      return att_wgt;
    }
    ratio_sigma = delta_sigma / mcval;
    delta_sigma -= mcval;
    
    att_wgt *= (ratio_sigma*(exp(-1.0*dist_travel*delta_sigma*mb_cm2_AND_rho_Target)));
    att_wgt *= 0.99;
    return att_wgt;
    
  }
  
}
