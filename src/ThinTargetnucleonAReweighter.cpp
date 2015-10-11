
#include "ThinTargetnucleonAReweighter.h"
#include <iostream>
#include "MakeReweight.h"
#include "ThinTargetMC.h"

namespace NeutrinoFluxReweight{
  
  ThinTargetnucleonAReweighter::ThinTargetnucleonAReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars):iUniv(iuniv),cvPars(cv_pars),univPars(univ_pars){
    
    vbin_data_pip.reserve(9);
    vbin_data_pim.reserve(9);
    vbin_data_kap.reserve(9);
    vbin_data_kam.reserve(9);
    
    vbin_prt_inc_pip.reserve(4);
    vbin_prt_inc_pim.reserve(4);
    vbin_prt_inc_kap.reserve(4);
    vbin_prt_inc_kam.reserve(4);
    vbin_prt_inc_k0.reserve(4);
    vbin_prt_inc_p.reserve(4);
    vbin_prt_inc_n.reserve(4);
    vbin_neu_inc_pip.reserve(4);
    vbin_neu_inc_pim.reserve(4);
    vbin_neu_inc_kap.reserve(4);
    vbin_neu_inc_kam.reserve(4);
    vbin_neu_inc_k0.reserve(4);
    vbin_neu_inc_p.reserve(4);
    vbin_neu_inc_n.reserve(4);

    std::map<std::string, double> cv_table   = cvPars.table;
    std::map<std::string, double> univ_table = univPars.table;
    char namepar[100];
    
    data_prod_xs = univ_table["prod_prtC_xsec"];
    
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

    //for all nucleons incident not covered by any thin target reweighters    
    //2 incident nucleons, 7 produced particles:
    const char* nuinc[2] = {"prt","neu"};
    const char* cpro[7] = {"pip","pim","kap","kam","k0","n","p"};
    
    for(int ii=0;ii<2;ii++){
      for(int jj=0;jj<7;jj++){
	for(int kk=0;kk<4;kk++){
	  sprintf(namepar,"ThinTarget_%s_incident_%s_%d",nuinc[ii],cpro[jj],kk);
	  double dataval = univ_table[std::string(namepar)];
	  if(ii==0 && jj==0)vbin_prt_inc_pip.push_back(dataval);
	  if(ii==0 && jj==1)vbin_prt_inc_pim.push_back(dataval);
	  if(ii==0 && jj==2)vbin_prt_inc_kap.push_back(dataval);
	  if(ii==0 && jj==3)vbin_prt_inc_kam.push_back(dataval);
	  if(ii==0 && jj==4)vbin_prt_inc_k0.push_back(dataval);
	  if(ii==0 && jj==5)vbin_prt_inc_n.push_back(dataval);
	  if(ii==0 && jj==6)vbin_prt_inc_p.push_back(dataval);
	  if(ii==1 && jj==0)vbin_neu_inc_pip.push_back(dataval);
	  if(ii==1 && jj==1)vbin_neu_inc_pim.push_back(dataval);
	  if(ii==1 && jj==2)vbin_neu_inc_kap.push_back(dataval);
	  if(ii==1 && jj==3)vbin_neu_inc_kam.push_back(dataval);
	  if(ii==1 && jj==4)vbin_neu_inc_k0.push_back(dataval);
	  if(ii==1 && jj==5)vbin_neu_inc_n.push_back(dataval);
	  if(ii==1 && jj==6)vbin_neu_inc_p.push_back(dataval);
	}
      }
    }	  
    //left over:
    sprintf(namepar,"ThinTarget_prtleftover_incident_%d",0);
    bin_prtleftover_inc = univ_table[std::string(namepar)];
    sprintf(namepar,"ThinTarget_neuleftover_incident_%d",0);
    bin_neuleftover_inc = univ_table[std::string(namepar)];
    
  }
  
   ThinTargetnucleonAReweighter::~ThinTargetnucleonAReweighter(){
    
  }
  bool ThinTargetnucleonAReweighter::canReweight(const InteractionData& aa){

    //checking:
    if(aa.Inc_pdg != 2212 && aa.Inc_pdg != 2112)return false;
    //  if(aa.Inc_P < 12.0)return false;
    // if(aa.Vol == "TGT1" || aa.Vol == "BudalMonitor")return false;
    //if(aa.Prod_pdg != 211 && aa.Prod_pdg != -211 && aa.Prod_pdg !=321 && aa.Prod_pdg != -321 && aa.Prod_pdg !=310 && aa.Prod_pdg != 130)return false;
    
    // ThinTargetBins*  Thinbins =  ThinTargetBins::getInstance();
    //int bin = Thinbins->material_scaling_BinID(aa.xF,aa.Pt,aa.Prod_pdg);
    // if(bin<0)return false;
    double inc_mom[3]  = {aa.Inc_P4[0], aa.Inc_P4[1], aa.Inc_P4[2]};
    double prod_mom[3] = {aa.Prod_P4[0],aa.Prod_P4[1],aa.Prod_P4[2]};
    double vtx_int[3]  = {aa.Vtx[0],aa.Vtx[1],aa.Vtx[2]};
    aux_aa = new InteractionData(aa.gen,inc_mom,aa.Inc_pdg,prod_mom,aa.Prod_pdg,"TGT1",aa.Proc,vtx_int);
    
    /*
    MakeReweight*  makerew =  MakeReweight::getInstance();
    if(aa.Inc_pdg == 2212){
      if(aa.Prod_pdg == 211 || aa.Prod_pdg == -211){
	if(iUniv==-1)tt_pCPionRew = (makerew->cv_rw)->THINTARGET_PC_PION_Universe;
	else tt_pCPionRew = (makerew->vec_rws[iUniv])->THINTARGET_PC_PION_Universe; 
	return tt_pCPionRew->canReweight(*aux_aa);
      }
      else if(aa.Prod_pdg == 321 || aa.Prod_pdg == -321 || aa.Prod_pdg == 310 || aa.Prod_pdg == 130){
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
    */

    return true;
  }
  
  double ThinTargetnucleonAReweighter::calculateWeight(const InteractionData& aa){

    double wgt = 1.0;

     //checking:
    if(aa.Inc_pdg != 2212 && aa.Inc_pdg != 2112)return wgt;
    /*
    if(aa.Inc_P < 12.0)return wgt;
    if(aa.Vol == "TGT1" || aa.Vol == "BudalMonitor")return wgt;
    if(aa.Prod_pdg != 211 && aa.Prod_pdg != -211 && aa.Prod_pdg !=321 && aa.Prod_pdg != -321 && aa.Prod_pdg !=310 && aa.Prod_pdg != 130)return wgt;
    */
    ThinTargetBins*  Thinbins =  ThinTargetBins::getInstance();
    int bin = Thinbins->material_scaling_BinID(aa.xF,aa.Pt,aa.Prod_pdg);
    
    bool is_data_based = (aa.Inc_P >= 12.0) && (aa.Vol != "TGT1" && aa.Vol != "BudalMonitor" && aa.Vol != "Budal_HFVS" && aa.Vol != "Budal_VFHS") && 
      (aa.Prod_pdg == 211 || aa.Prod_pdg == -211 || aa.Prod_pdg ==321 || aa.Prod_pdg == -321 || aa.Prod_pdg ==310 || aa.Prod_pdg == 130) &&
      (bin>=0);
    
    bool not_handled = false;
    if(is_data_based){
      
      ThinTargetMC*  mc =  ThinTargetMC::getInstance(); 
      double mc_prod   = mc->getMCxs_pC_piK(0,aa.Inc_P);
      double fact_gen0 = data_prod_xs/mc_prod;
      MakeReweight*  makerew =  MakeReweight::getInstance();

      if(aa.Inc_pdg == 2212){

	if(aa.Prod_pdg == 211 || aa.Prod_pdg == -211){

	  if(iUniv==-1)tt_pCPionRew = (makerew->cv_rw)->THINTARGET_PC_PION_Universe;
	  else tt_pCPionRew = (makerew->vec_rws[iUniv])->THINTARGET_PC_PION_Universe;    
	  
	  if(tt_pCPionRew->canReweight(*aux_aa)){
	    wgt = tt_pCPionRew->calculateWeight(*aux_aa);
	    if(aux_aa->gen == 0) wgt *= fact_gen0;
	  }
	  else not_handled = true;
	}      

	else if(aa.Prod_pdg == 321 || aa.Prod_pdg == -321 || aa.Prod_pdg == 130 || aa.Prod_pdg == 310){

	  if(iUniv==-1)tt_pCKaonRew = (makerew->cv_rw)->THINTARGET_PC_KAON_Universe;
	  else tt_pCKaonRew = (makerew->vec_rws[iUniv])->THINTARGET_PC_KAON_Universe;    
	  
	  if(tt_pCKaonRew->canReweight(*aux_aa)){
	    wgt = tt_pCKaonRew->calculateWeight(*aux_aa);
	    if(aux_aa->gen == 0) wgt *= fact_gen0;
	  }
	  else not_handled = true;
	}
	else not_handled = true;
      }
      else if(aa.Inc_pdg == 2112){
	if(aa.Prod_pdg == 211 || aa.Prod_pdg == -211){

	  if(iUniv==-1)tt_nCPionRew = (makerew->cv_rw)->THINTARGET_NC_PION_Universe;
	  else tt_nCPionRew = (makerew->vec_rws[iUniv])->THINTARGET_NC_PION_Universe;    

	  if(tt_nCPionRew->canReweight(*aux_aa)){
	    wgt = tt_nCPionRew->calculateWeight(*aux_aa);
	    if(aux_aa->gen == 0) wgt *= fact_gen0;
	  }      
	  else not_handled = true;
	}
	else not_handled = true;
      }
      else not_handled = true;
      
      double scaling = 1.0;
      if(aa.Prod_pdg == 211)scaling = vbin_data_pip[bin];
      if(aa.Prod_pdg ==-211)scaling = vbin_data_pim[bin];
      if(aa.Prod_pdg == 321)scaling = vbin_data_kap[bin];
      if(aa.Prod_pdg ==-321)scaling = vbin_data_kam[bin];
      if(aa.Prod_pdg == 310 || aa.Prod_pdg == 130)scaling = vbin_data_kap[bin];    
      wgt *= scaling;
      if(!not_handled)return wgt;
    } 
 
    //trick... using a function for meson incident... same binning.
    int binnu      = Thinbins->meson_inc_BinID(aa.xF,aa.Pt,211);
    if(binnu<0)return 1.0;

    if(aa.Inc_pdg ==2212){
      if(aa.Prod_pdg == 211) wgt = vbin_prt_inc_pip[binnu];
      else if(aa.Prod_pdg ==-211) wgt = vbin_prt_inc_pim[binnu];
      else if(aa.Prod_pdg == 321) wgt = vbin_prt_inc_kap[binnu];
      else if(aa.Prod_pdg ==-321) wgt = vbin_prt_inc_kam[binnu];
      else if(aa.Prod_pdg ==130 || aa.Prod_pdg ==310) wgt = vbin_prt_inc_k0[binnu];
      else if(aa.Prod_pdg ==2212) wgt = vbin_prt_inc_p[binnu];
      else if(aa.Prod_pdg ==2112) wgt = vbin_prt_inc_n[binnu];	
      else wgt = bin_prtleftover_inc;	
     }
    else if(aa.Inc_pdg ==2112){
      if(aa.Prod_pdg == 211) wgt = vbin_neu_inc_pip[binnu];
      else if(aa.Prod_pdg ==-211) wgt = vbin_neu_inc_pim[binnu];
      else if(aa.Prod_pdg == 321) wgt = vbin_neu_inc_kap[binnu];
      else if(aa.Prod_pdg ==-321) wgt = vbin_neu_inc_kam[binnu];
      else if(aa.Prod_pdg ==130 || aa.Prod_pdg ==310) wgt = vbin_neu_inc_k0[binnu];
      else if(aa.Prod_pdg ==2212) wgt = vbin_neu_inc_p[binnu];
      else if(aa.Prod_pdg ==2112) wgt = vbin_neu_inc_n[binnu];
      else wgt = bin_neuleftover_inc;	
     }						
  
    return wgt;
    
  }
  

}
