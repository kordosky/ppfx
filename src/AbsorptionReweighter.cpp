

#include "AbsorptionReweighter.h"
#include "DataHistos.h"
#include "CentralValuesAndUncertainties.h"

#include "HistoContainer.h"
#include "ExtractInfo.h"
#include <math.h>

#include <iostream>

namespace NeutrinoFluxReweight{
  
  AbsorptionReweighter::AbsorptionReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars):iUniv(iuniv),cvPars(cv_pars),univPars(univ_pars){ 
    // do any other necessary initialization    
  }
  AbsorptionReweighter::~AbsorptionReweighter(){
    
  }
  std::vector<bool> AbsorptionReweighter::canReweight(const InteractionChainData& aa){
 
    std::vector<bool> this_nodes;
    double low_val = 1.E-20;    
    
    std::vector<ParticlesThroughVolumesData>  vec_ptv = aa.ptv_info;
    bool passIC = vec_ptv[0].DistXdens[0] >low_val || vec_ptv[1].DistXdens[0] >low_val;
    passIC = passIC && abs(vec_ptv[0].Pdgs[0])==211;
    passIC = passIC && abs(vec_ptv[0].Pdgs[0])==321;
    if(passIC)this_nodes.push_back(passIC);
    return this_nodes;

  }
  double AbsorptionReweighter::calculateWeight(const InteractionChainData& aa){
    
    std::map<std::string, double> dsig_table = univPars.table;
    std::map<std::string, double>::iterator it = dsig_table.begin();
    
    std::vector<ParticlesThroughVolumesData>  vec_ptv = aa.ptv_info;
    std::string namepar;
    
    if(abs(vec_ptv[0].Pdgs[0])==211){
      namepar = "inel_piAl_xsec";
    }
    else if(vec_ptv[0].Pdgs[0]==321 && vec_ptv[0].Moms[0]<2.0){
      namepar = "inel_kapAl_xsec_lowP";
    }
    else if(vec_ptv[0].Pdgs[0]==321 && vec_ptv[0].Moms[0]>2.0){
      namepar = "inel_kapAl_xsec_highP";
    }
    else if(vec_ptv[0].Pdgs[0]==-321 && vec_ptv[0].Moms[0]<2.0){
      namepar = "inel_kamAl_xsec_lowP";
    }
    else if(vec_ptv[0].Pdgs[0]==-321 && vec_ptv[0].Moms[0]>2.0){
      namepar = "inel_kamAl_xsec_lowP";
    }
    
    it = dsig_table.find(std::string(namepar));
    double shift = it->second;
    double NA_mb = 6.02E-4;
    double Al_A = 26.8;
    double tot_dist = vec_ptv[0].DistXdens[0] + vec_ptv[1].DistXdens[0];
 
    tot_dist /= Al_A;
    tot_dist *= NA_mb;
    tot_dist *= shift;
    
    double wgt = exp(-1.0*tot_dist);
    
    //for now, it will not be allowed to very low and very high weithgs. 
    if(wgt<0.1 || wgt>10.){
      // std::cout<<"something weird here. wgt = "<<wgt<<" "<<vec_ptv[0].Pdgs[0]<<" "<<vec_ptv[0].Moms[0]<<" "<<vec_ptv[0].DistXdens[0]<<" "<<vec_ptv[1].DistXdens[0]<<" "<<shift<<" "<<tot_dist<<std::endl;
      wgt = 1.0;
    }
    return wgt;
    
  }

}
