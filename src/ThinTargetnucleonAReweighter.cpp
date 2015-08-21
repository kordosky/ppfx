
#include "ThinTargetnucleonAReweighter.h"
#include <iostream>
#include "MakeReweight.h"

namespace NeutrinoFluxReweight{
  
  ThinTargetnucleonAReweighter::ThinTargetnucleonAReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars):iUniv(iuniv),cvPars(cv_pars),univPars(univ_pars){
    
  }
  
   ThinTargetnucleonAReweighter::~ThinTargetnucleonAReweighter(){
    
  }
  bool ThinTargetnucleonAReweighter::canReweight(const InteractionData& aa){

    //checking:
    if(aa.Inc_pdg != 2212 && aa.Inc_pdg != 2112)return false;
    if(aa.Inc_P < 12.0)return false;
    if(aa.Vol == "TGT1" || aa.Vol == "BudalMonitor")return false;
    if(aa.Prod_pdg != 211 && aa.Prod_pdg != -211 && aa.Prod_pdg !=321 && aa.Prod_pdg != -321)return false;
    
    double inc_mom[3]  = {aa.Inc_P4[0], aa.Inc_P4[1], aa.Inc_P4[2]};
    double prod_mom[3] = {aa.Prod_P4[0],aa.Prod_P4[1],aa.Prod_P4[2]};
    double vtx_int[3]  = {aa.Vtx[0],aa.Vtx[1],aa.Vtx[2]};
    aux_aa = new InteractionData(aa.gen,inc_mom,aa.Inc_pdg,prod_mom,aa.Prod_pdg,"TGT1",aa.Proc,vtx_int);
    
    MakeReweight*  makerew =  MakeReweight::getInstance();
    if(aa.Inc_pdg == 2212){
      if(aa.Prod_pdg == 211 || aa.Prod_pdg == -211){
	if(iUniv==-1)tt_pCPionRew = (makerew->cv_rw)->THINTARGET_PC_PION_Universe;
	else tt_pCPionRew = (makerew->vec_rws[iUniv])->THINTARGET_PC_PION_Universe; 
	return tt_pCPionRew->canReweight(*aux_aa);
      }
      else if(aa.Prod_pdg == 321 || aa.Prod_pdg == -321){
	if(iUniv==-1)tt_pCKaonRew = (makerew->cv_rw)->THINTARGET_PC_KAON_Universe;
	else tt_pCKaonRew = (makerew->vec_rws[iUniv])->THINTARGET_PC_KAON_Universe;  
	return tt_pCKaonRew->canReweight(*aux_aa);
      }
    }
    else if(aa.Inc_pdg == 2112){
      if(aa.Prod_pdg == 211 || aa.Prod_pdg == -211){
	if(iUniv==-1)tt_nCPionRew = (makerew->cv_rw)->THINTARGET_NC_PION_Universe;
	else tt_nCPionRew = (makerew->vec_rws[iUniv])->THINTARGET_NC_PION_Universe;
	return tt_nCPionRew->canReweight(*aux_aa);
      }      
    }
    
    return false;
  }
  
  double ThinTargetnucleonAReweighter::calculateWeight(const InteractionData& aa){

    double wgt = 1.0;
    MakeReweight*  makerew =  MakeReweight::getInstance();
    
    if(aa.Inc_pdg == 2212){
      if(aa.Prod_pdg == 211 || aa.Prod_pdg == -211){
	if(iUniv==-1)tt_pCPionRew = (makerew->cv_rw)->THINTARGET_PC_PION_Universe;
	else tt_pCPionRew = (makerew->vec_rws[iUniv])->THINTARGET_PC_PION_Universe;    
	wgt = tt_pCPionRew->calculateWeight(*aux_aa);
      }      
      else if(aa.Prod_pdg == 321 || aa.Prod_pdg == -321){
	if(iUniv==-1)tt_pCKaonRew = (makerew->cv_rw)->THINTARGET_PC_KAON_Universe;
	else tt_pCKaonRew = (makerew->vec_rws[iUniv])->THINTARGET_PC_KAON_Universe;    
	wgt = tt_pCKaonRew->calculateWeight(*aux_aa);
      }
    }
    else if(aa.Inc_pdg == 2112){
      if(aa.Prod_pdg == 211 || aa.Prod_pdg == -211){
	if(iUniv==-1)tt_nCPionRew = (makerew->cv_rw)->THINTARGET_NC_PION_Universe;
	else tt_nCPionRew = (makerew->vec_rws[iUniv])->THINTARGET_NC_PION_Universe;    
	wgt = tt_nCPionRew->calculateWeight(*aux_aa);
      }      
    }   
    
    if(wgt>1.0)wgt *= 1.1;
    if(wgt<1.0)wgt *= 0.9;
    
    return wgt;
    
  }
  

}
