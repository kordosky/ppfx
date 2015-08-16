
#include "ThinTargetMesonIncidentReweighter.h"
#include <iostream>
#include "ThinTargetBins.h"

namespace NeutrinoFluxReweight{
  
  ThinTargetMesonIncidentReweighter::ThinTargetMesonIncidentReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars):iUniv(iuniv),cvPars(cv_pars),univPars(univ_pars){
    
    vbin_data_pip_inc.reserve(4);
    vbin_data_pim_inc.reserve(4);
    vbin_data_kap_inc.reserve(4);
    vbin_data_kam_inc.reserve(4);
    vbin_data_k0_inc.reserve(4);
    
    std::map<std::string, double> cv_table   = cvPars.table;
    std::map<std::string, double> univ_table = univPars.table;
    char namepar[100];

    for(int ii=0;ii<4;ii++){
      
      sprintf(namepar,"ThinTarget_%s_incident_%d","pip",ii);
      double dataval = univ_table[std::string(namepar)];
      vbin_data_pip_inc.push_back(dataval);
      
      sprintf(namepar,"ThinTarget_%s_incident_%d","pim",ii);
      dataval = univ_table[std::string(namepar)];
      vbin_data_pim_inc.push_back(dataval);

      sprintf(namepar,"ThinTarget_%s_incident_%d","kap",ii);
      dataval = univ_table[std::string(namepar)];
      vbin_data_kap_inc.push_back(dataval);
      
      sprintf(namepar,"ThinTarget_%s_incident_%d","kam",ii);
      dataval = univ_table[std::string(namepar)];
      vbin_data_kam_inc.push_back(dataval);
      
      sprintf(namepar,"ThinTarget_%s_incident_%d","k0",ii);
      dataval = univ_table[std::string(namepar)];
      vbin_data_k0_inc.push_back(dataval);
      
    } 


  }
  
   ThinTargetMesonIncidentReweighter::~ThinTargetMesonIncidentReweighter(){
    
  }
  bool ThinTargetMesonIncidentReweighter::canReweight(const InteractionData& aa){
    
    if(aa.Inc_pdg != 211 && aa.Inc_pdg != -211 && aa.Inc_pdg != 321 && aa.Inc_pdg != -321 && aa.Inc_pdg != 130 && aa.Inc_pdg != 310)return false;
    if(aa.Proc.find("Inelastic")>100)return false;
    ThinTargetBins*  Thinbins =  ThinTargetBins::getInstance();
    int bin      = Thinbins->meson_inc_BinID(aa.xF,aa.Pt,aa.Prod_pdg);
    
    if(bin<0)return false;
    else return true;
    
  }
  
  double ThinTargetMesonIncidentReweighter::calculateWeight(const InteractionData& aa){

    double wgt = 1.0;
    ThinTargetBins*  Thinbins =  ThinTargetBins::getInstance();
    int bin      = Thinbins->meson_inc_BinID(aa.xF,aa.Pt,aa.Prod_pdg);
    if(bin>0){
      if(aa.Inc_pdg == 211)wgt = vbin_data_pip_inc[bin];
      else if(aa.Inc_pdg ==-211)wgt = vbin_data_pim_inc[bin];
      else if(aa.Inc_pdg == 321)wgt = vbin_data_kap_inc[bin];
      else if(aa.Inc_pdg ==-321)wgt = vbin_data_kam_inc[bin];
      else if(aa.Inc_pdg ==130||aa.Inc_pdg==310)wgt = vbin_data_k0_inc[bin];
      else{
	std::cout<<"Something is wrong with pdg_inc: "<< aa.Inc_pdg  <<std::endl;
	return wgt;
      }
    }
    return wgt;

  }
  

}
