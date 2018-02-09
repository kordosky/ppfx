
#include "ThinTargetnCPionReweighter.h"
#include <iostream>
#include "MakeReweight.h"

namespace NeutrinoFluxReweight{
  
  ThinTargetnCPionReweighter::ThinTargetnCPionReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars):iUniv(iuniv),cvPars(cv_pars),univPars(univ_pars){
    
  }
  
   ThinTargetnCPionReweighter::~ThinTargetnCPionReweighter(){
    
  }
  bool ThinTargetnCPionReweighter::canReweight(const InteractionData& aa){

    MakeReweight*  makerew =  MakeReweight::getInstance();
    if(iUniv==-1)tt_pCPionRew = (makerew->cv_rw)->THINTARGET_PC_PION_Universe;
    else tt_pCPionRew = (makerew->vec_rws[iUniv])->THINTARGET_PC_PION_Universe;
    
    //checking:
    std::string mode(getenv("MODE"));
    if(aa.Inc_pdg != 2112)return false;
    if(aa.Inc_P < 12.0)return false;
    //volume check: 
    bool is_wrong_volume = aa.Vol != "TGT1" && aa.Vol != "BudalMonitor" && aa.Vol != "Budal_HFVS" && aa.Vol != "Budal_VFHS";
    if( (mode=="REF") || (mode=="OPT") ){
      is_wrong_volume = aa.Vol != "TargetFinHorizontal" && aa.Vol != "TargetNoSplitSegment";
    }
    if(is_wrong_volume)return false;
    //
    if(aa.Prod_pdg != 211 && aa.Prod_pdg != -211)return false;
    
    double inc_mom[3]  = {aa.Inc_P4[0], aa.Inc_P4[1], aa.Inc_P4[2]};
    double prod_mom[3] = {aa.Prod_P4[0],aa.Prod_P4[1],aa.Prod_P4[2]};
    double vtx_int[3]  = {aa.Vtx[0],aa.Vtx[1],aa.Vtx[2]};
    
    InteractionData intData(aa.gen, inc_mom,2212,prod_mom,-1*aa.Prod_pdg,aa.Vol,aa.Proc,vtx_int);
    return tt_pCPionRew->canReweight(intData);
    
  }
  
  double ThinTargetnCPionReweighter::calculateWeight(const InteractionData& aa){
    
    MakeReweight*  makerew =  MakeReweight::getInstance();
    if(iUniv==-1)tt_pCPionRew = (makerew->cv_rw)->THINTARGET_PC_PION_Universe;
    else tt_pCPionRew = (makerew->vec_rws[iUniv])->THINTARGET_PC_PION_Universe;

    double inc_mom[3]  = {aa.Inc_P4[0], aa.Inc_P4[1], aa.Inc_P4[2]};
    double prod_mom[3] = {aa.Prod_P4[0],aa.Prod_P4[1],aa.Prod_P4[2]};
    double vtx_int[3]  = {aa.Vtx[0],aa.Vtx[1],aa.Vtx[2]};
    InteractionData intData(aa.gen, inc_mom,2212,prod_mom,-1*aa.Prod_pdg,aa.Vol,aa.Proc,vtx_int);

    double wgt = tt_pCPionRew->calculateWeight(intData);

    if(wgt<0)std::cout<<"TTNCPI check wgt(<0) "<<iUniv<<" "<<aa.Inc_P<<" "<<aa.xF<<" "<<aa.Pt<<" "<<aa.Prod_pdg<<std::endl;
    return wgt;
      
  }
  

}
