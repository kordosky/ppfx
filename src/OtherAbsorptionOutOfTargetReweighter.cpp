

#include "OtherAbsorptionOutOfTargetReweighter.h"
#include "CentralValuesAndUncertainties.h"

#include <cstdlib>

#include <iostream>

namespace NeutrinoFluxReweight{
  
  OtherAbsorptionOutOfTargetReweighter::OtherAbsorptionOutOfTargetReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars):cvPars(cv_pars),univPars(univ_pars),iUniv(iuniv){ 
    
    // const boost::interprocess::flat_map<std::string, double>& dsig_table = univPars.getMap();
    inel_kapAl_xsec_lowP  = univPars.getParameterValue("inel_kapAl_xsec_lowP");
    inel_kapAl_xsec_highP = univPars.getParameterValue("inel_kapAl_xsec_highP");
  }
  OtherAbsorptionOutOfTargetReweighter::~OtherAbsorptionOutOfTargetReweighter(){
    
  }
  std::vector<bool> OtherAbsorptionOutOfTargetReweighter::canReweight(const InteractionChainData& aa){
 
    std::vector<bool> this_nodes;
    //int index_vol = 0;
    double low_val = 1.E-20;    
  
    std::vector<ParticlesThroughVolumesData>  vec_ptv = aa.ptv_info;

    bool passVOL = false;
    //Cheking at least one ancestor with amount of materail value:
    
    for(int index_vol=0;index_vol<3;index_vol++){
      for(int ii=0;ii<3;ii++){
	passVOL = passVOL || (vec_ptv[index_vol].AmountMat[ii] >low_val && (abs(vec_ptv[index_vol].Pdgs[ii])!=211 && abs(vec_ptv[index_vol].Pdgs[ii])!=321 && vec_ptv[index_vol].Pdgs[ii]!=2212 && vec_ptv[index_vol].Pdgs[ii]!=2112 && abs(vec_ptv[index_vol].Pdgs[ii])>99));
      }
    }
    this_nodes.push_back(passVOL);
    return this_nodes;

  }
  double OtherAbsorptionOutOfTargetReweighter::calculateWeight(const InteractionChainData& aa){
    
    std::vector<ParticlesThroughVolumesData>  vec_ptv = aa.ptv_info;

    double shift_lowP = inel_kapAl_xsec_lowP;
    double shift_highP = inel_kapAl_xsec_highP;
    
    double NA_mb    = 6.02E-4;
    double wgt      = 1.0;
    double tot_dist = 0.0;
    double low_val  = 1.E-20;   
    
    for(int index_vol=0;index_vol<3;index_vol++){
      for(int ii=0;ii<3;ii++){
	
	tot_dist = vec_ptv[index_vol].AmountMat[ii];
	if(tot_dist<low_val)continue;
	if(abs(vec_ptv[index_vol].Pdgs[ii])==321 || abs(vec_ptv[index_vol].Pdgs[ii])==211 || vec_ptv[index_vol].Pdgs[ii]==2212 || vec_ptv[index_vol].Pdgs[ii]==2112 || abs(vec_ptv[index_vol].Pdgs[ii])<100)continue;
	
	tot_dist *= NA_mb;
	if(vec_ptv[index_vol].Moms[ii]<2.0){
	  tot_dist *= shift_lowP;
	}
	else if(vec_ptv[index_vol].Moms[ii]>=2.0){
	  tot_dist *= shift_highP;
	}
	wgt *= exp(-1.0*tot_dist);   
      }
    }
    return wgt;
    
  }

}
