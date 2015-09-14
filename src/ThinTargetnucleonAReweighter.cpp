
#include "ThinTargetnucleonAReweighter.h"
#include <iostream>
#include "MakeReweight.h"

namespace NeutrinoFluxReweight{
  
  ThinTargetnucleonAReweighter::ThinTargetnucleonAReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars):iUniv(iuniv),cvPars(cv_pars),univPars(univ_pars){
    
    vbin_data_pip.reserve(9);
    vbin_data_pim.reserve(9);
    vbin_data_kap.reserve(9);
    vbin_data_kam.reserve(9);
    
    std::map<std::string, double> cv_table   = cvPars.table;
    std::map<std::string, double> univ_table = univPars.table;
    char namepar[100];
    
    //4 particles
    const char* cinc[4] = {"pip","pim","kap","kam"};    
    for(int ii=0;ii<4;ii++){
      for(int jj=0;jj<9;jj++){
	sprintf(namepar,"ThinTarget_material_scaling_%s_%d",cinc[ii],jj);
	double dataval = univ_table[std::string(namepar)];
	if(ii==0)vbin_data_pip.push_back(dataval);
	if(ii==1)vbin_data_pim.push_back(dataval);
	if(ii==2)vbin_data_kap.push_back(dataval);
	if(ii==3)vbin_data_kam.push_back(dataval);
      }
    }
    
  }
  
   ThinTargetnucleonAReweighter::~ThinTargetnucleonAReweighter(){
    
  }
  bool ThinTargetnucleonAReweighter::canReweight(const InteractionData& aa){

    //checking:
    if(aa.Inc_pdg != 2212 && aa.Inc_pdg != 2112)return false;
    if(aa.Inc_P < 12.0)return false;
    if(aa.Vol == "TGT1" || aa.Vol == "BudalMonitor")return false;
    if(aa.Prod_pdg != 211 && aa.Prod_pdg != -211 && aa.Prod_pdg !=321 && aa.Prod_pdg != -321)return false;
    
    ThinTargetBins*  Thinbins =  ThinTargetBins::getInstance();
    int bin = Thinbins->material_scaling_BinID(aa.xF,aa.Pt,aa.Prod_pdg);
    if(bin<0)return false;
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

     //checking:
    if(aa.Inc_pdg != 2212 && aa.Inc_pdg != 2112)return wgt;
    if(aa.Inc_P < 12.0)return wgt;
    if(aa.Vol == "TGT1" || aa.Vol == "BudalMonitor")return wgt;
    if(aa.Prod_pdg != 211 && aa.Prod_pdg != -211 && aa.Prod_pdg !=321 && aa.Prod_pdg != -321)return wgt;
    
    ThinTargetBins*  Thinbins =  ThinTargetBins::getInstance();
    int bin = Thinbins->material_scaling_BinID(aa.xF,aa.Pt,aa.Prod_pdg);
    if(bin<0)return wgt;

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
    
    double scaling = 1.0;
    if(aa.Prod_pdg == 211)scaling = vbin_data_pip[bin];
    if(aa.Prod_pdg ==-211)scaling = vbin_data_pim[bin];
    if(aa.Prod_pdg == 321)scaling = vbin_data_kap[bin];
    if(aa.Prod_pdg ==-321)scaling = vbin_data_kam[bin];
    
    wgt *= scaling;
        
    return wgt;
    
  }
  

}
