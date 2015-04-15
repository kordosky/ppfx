

#include "AbsorptionICReweighter.h"
#include "DataHistos.h"
#include "CentralValuesAndUncertainties.h"

#include "HistoContainer.h"
#include "ExtractInfo.h"
#include <cstdlib>

#include <iostream>

namespace NeutrinoFluxReweight{
  
  AbsorptionICReweighter::AbsorptionICReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars):iUniv(iuniv),cvPars(cv_pars),univPars(univ_pars){ 
    // do any other necessary initialization    
  }
  AbsorptionICReweighter::~AbsorptionICReweighter(){
    
  }
  std::vector<bool> AbsorptionICReweighter::canReweight(const InteractionChainData& aa){
 
    std::vector<bool> this_nodes;
    int index_vol = 0;
    double low_val = 1.E-20;    
  
    std::vector<ParticlesThroughVolumesData>  vec_ptv = aa.ptv_info;

    bool passVOL = false;
    //Cheking at least one ancestor with amount of materail value:
    for(int ii=0;ii<3;ii++){
      passVOL = passVOL || (vec_ptv[index_vol].AmountMat[ii] >low_val && (abs(vec_ptv[index_vol].Pdgs[ii])==211 || abs(vec_ptv[index_vol].Pdgs[ii])==321));
    }
    if(passVOL)this_nodes.push_back(passVOL);
    return this_nodes;

  }
  double AbsorptionICReweighter::calculateWeight(const InteractionChainData& aa){
    
    std::map<std::string, double> dsig_table = univPars.table;
    std::map<std::string, double>::iterator it = dsig_table.begin();
    
    std::vector<ParticlesThroughVolumesData>  vec_ptv = aa.ptv_info;
    std::string namepar;
    double NA_mb    = 6.02E-4;
    double wgt      = 1.0;
    double tot_dist = 0.0;
    double low_val  = 1.E-20;   
    int index_vol = 0;
    
    for(int ii=0;ii<3;ii++){

      tot_dist = vec_ptv[index_vol].AmountMat[ii];
      if(tot_dist<low_val)continue;
      if(abs(vec_ptv[index_vol].Pdgs[ii])!=321 && abs(vec_ptv[index_vol].Pdgs[ii])!=211)continue;
      
      if(abs(vec_ptv[index_vol].Pdgs[ii])==211){
	namepar = "inel_piAl_xsec";
	 }
      else if(vec_ptv[index_vol].Pdgs[ii]==321 && vec_ptv[index_vol].Moms[ii]<2.0){
	namepar = "inel_kapAl_xsec_lowP";
      }
      else if(vec_ptv[index_vol].Pdgs[ii]==321 && vec_ptv[index_vol].Moms[ii]>2.0){
	namepar = "inel_kapAl_xsec_highP";
      }
      else if(vec_ptv[index_vol].Pdgs[ii]==-321 && vec_ptv[index_vol].Moms[ii]<2.0){
	namepar = "inel_kamAl_xsec_lowP";
      }
      else if(vec_ptv[index_vol].Pdgs[ii]==-321 && vec_ptv[index_vol].Moms[ii]>2.0){
	namepar = "inel_kamAl_xsec_highP";
      }

      it = dsig_table.begin();
      it = dsig_table.find(std::string(namepar));
      double shift = it->second;
    
      tot_dist *= NA_mb;
      tot_dist *= shift;
      
      wgt *= exp(-1.0*tot_dist);   
    }
    
    return wgt;
    
  }

}
