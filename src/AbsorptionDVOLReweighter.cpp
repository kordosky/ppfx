

#include "AbsorptionDVOLReweighter.h"
#include "CentralValuesAndUncertainties.h"

#include <cstdlib>

#include <iostream>

namespace NeutrinoFluxReweight{
  
  AbsorptionDVOLReweighter::AbsorptionDVOLReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars):cvPars(cv_pars),univPars(univ_pars),iUniv(iuniv){ 
    
    // const boost::interprocess::flat_map<std::string, double>& dsig_table = univPars.getMap();
    inel_piAl_xsec = univPars.getParameterValue("inel_piAl_xsec");
    inel_kapAl_xsec_lowP  = univPars.getParameterValue("inel_kapAl_xsec_lowP");
    inel_kapAl_xsec_highP = univPars.getParameterValue("inel_kapAl_xsec_highP");
    inel_kamAl_xsec_lowP  = univPars.getParameterValue("inel_kamAl_xsec_lowP");
    inel_kamAl_xsec_highP = univPars.getParameterValue("inel_kamAl_xsec_highP");
    
  }
  AbsorptionDVOLReweighter::~AbsorptionDVOLReweighter(){
    
  }
  std::vector<bool> AbsorptionDVOLReweighter::canReweight(const InteractionChainData& aa){
 
    std::vector<bool> this_nodes;
    int index_vol = 2;
    double low_val = 1.E-20;    
  
    std::vector<ParticlesThroughVolumesData>  vec_ptv = aa.ptv_info;

    bool passVOL = false;
    //Cheking at least one ancestor with amount of materail value:
    for(int ii=0;ii<3;ii++){
      passVOL = passVOL || (vec_ptv[index_vol].AmountMat[ii] >low_val && (abs(vec_ptv[index_vol].Pdgs[ii])==211 || abs(vec_ptv[index_vol].Pdgs[ii])==321));
    }
    this_nodes.push_back(passVOL);
    return this_nodes;

  }
  double AbsorptionDVOLReweighter::calculateWeight(const InteractionChainData& aa){
    
    std::vector<ParticlesThroughVolumesData>  vec_ptv = aa.ptv_info;
    std::string namepar;
    double NA_mb    = 6.02E-4;
    double wgt      = 1.0;
    double tot_dist = 0.0;
    double low_val  = 1.E-20;   
    int index_vol   = 2;
    
    for(int ii=0;ii<3;ii++){
      float shift = 0.0;
      tot_dist = vec_ptv[index_vol].AmountMat[ii];
      if(tot_dist<low_val)continue;
      if(abs(vec_ptv[index_vol].Pdgs[ii])!=321 && abs(vec_ptv[index_vol].Pdgs[ii])!=211)continue;
      
      if(abs(vec_ptv[index_vol].Pdgs[ii])== 211)shift = inel_piAl_xsec;
      else if(vec_ptv[index_vol].Pdgs[ii]== 321 && vec_ptv[index_vol].Moms[ii]<2.0)shift = inel_kapAl_xsec_lowP;
      else if(vec_ptv[index_vol].Pdgs[ii]== 321 && vec_ptv[index_vol].Moms[ii]>2.0)shift = inel_kapAl_xsec_highP;
      else if(vec_ptv[index_vol].Pdgs[ii]==-321 && vec_ptv[index_vol].Moms[ii]<2.0)shift = inel_kamAl_xsec_lowP;
      else if(vec_ptv[index_vol].Pdgs[ii]==-321 && vec_ptv[index_vol].Moms[ii]>2.0)shift = inel_kamAl_xsec_highP;
    
      tot_dist *= NA_mb;
      tot_dist *= shift;
      
      wgt *= exp(-1.0*tot_dist);   
    }
    
    return wgt;
    
  }

}
