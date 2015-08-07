
#include "ThinTargetnCPionReweighter.h"
#include <iostream>

namespace NeutrinoFluxReweight{
  
  ThinTargetnCPionReweighter::ThinTargetnCPionReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars):iUniv(iuniv),cvPars(cv_pars),univPars(univ_pars){

    tt_pCPionRew = new ThinTargetpCPionReweighter(iUniv,cvPars,univPars);

  }
  
   ThinTargetnCPionReweighter::~ThinTargetnCPionReweighter(){
    
  }
  bool ThinTargetnCPionReweighter::canReweight(const InteractionData& aa){

    //fast checking:
    if(aa.Inc_pdg == 2112 && (aa.Prod_pdg == 211 || aa.Prod_pdg == -211)){
      double inc_mom[3]  = {aa.Inc_P4[0], aa.Inc_P4[1], aa.Inc_P4[2]};
      double prod_mom[3] = {aa.Prod_P4[0],aa.Prod_P4[1],aa.Prod_P4[2]};
      double vtx_int[3]  = {aa.Vtx[0],aa.Vtx[1],aa.Vtx[2]};
	
      aux_aa = new InteractionData(inc_mom,2212,prod_mom,-1*aa.Prod_pdg,aa.Vol,aa.Proc,vtx_int);
      return tt_pCPionRew->canReweight(*aux_aa);
    }
    else{
      return false;
    }
  }
  
  double ThinTargetnCPionReweighter::calculateWeight(const InteractionData& thisid){
    
    double wgt = tt_pCPionRew->calculateWeight(*aux_aa);
    
    return wgt;
      
  }
  

}
