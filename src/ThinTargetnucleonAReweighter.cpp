
#include "ThinTargetnucleonAReweighter.h"
#include <iostream>
#include <cstdlib>
#include "MakeReweight.h"
#include "ThinTargetMC.h"

namespace NeutrinoFluxReweight{
  
  ThinTargetnucleonAReweighter::ThinTargetnucleonAReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars):iUniv(iuniv),cvPars(cv_pars),univPars(univ_pars){
    
    ThinTargetBins* Thinbins =  ThinTargetBins::getInstance();
    
    vbin_data_pip.reserve(Thinbins->GetNbins_material_scaling());
    vbin_data_pim.reserve(Thinbins->GetNbins_material_scaling());
    vbin_data_kap.reserve(Thinbins->GetNbins_material_scaling());
    vbin_data_kam.reserve(Thinbins->GetNbins_material_scaling());
    
    //Currently, We are using the same number of xF ranges for nucleon inc. and meson inc.
    vbin_prt_inc_pip.reserve(Thinbins->GetNbins_meson_incident());
    vbin_prt_inc_pim.reserve(Thinbins->GetNbins_meson_incident());
    vbin_prt_inc_kap.reserve(Thinbins->GetNbins_meson_incident());
    vbin_prt_inc_kam.reserve(Thinbins->GetNbins_meson_incident());
    vbin_prt_inc_k0.reserve(Thinbins->GetNbins_meson_incident());
    vbin_prt_inc_p.reserve(Thinbins->GetNbins_meson_incident());
    vbin_prt_inc_n.reserve(Thinbins->GetNbins_meson_incident());
    vbin_neu_inc_pip.reserve(Thinbins->GetNbins_meson_incident());
    vbin_neu_inc_pim.reserve(Thinbins->GetNbins_meson_incident());
    vbin_neu_inc_kap.reserve(Thinbins->GetNbins_meson_incident());
    vbin_neu_inc_kam.reserve(Thinbins->GetNbins_meson_incident());
    vbin_neu_inc_k0.reserve(Thinbins->GetNbins_meson_incident());
    vbin_neu_inc_p.reserve(Thinbins->GetNbins_meson_incident());
    vbin_neu_inc_n.reserve(Thinbins->GetNbins_meson_incident());

    // const boost::interprocess::flat_map<std::string, double>& cv_table   = cvPars.getMap();
    // const boost::interprocess::flat_map<std::string, double>& univ_table = univPars.getMap();
    char namepar[100];
    
    data_prod_xs = univPars.getParameterValue("prod_prtC_xsec");
    
    //4 particles
    const char* cinc[4] = {"pip","pim","kap","kam"};    
    for(int ii=0;ii<4;ii++){
      for(int jj=0;jj<Thinbins->GetNbins_material_scaling();jj++){
        sprintf(namepar,"ThinTarget_material_scaling_%s_%d",cinc[ii],jj);
        double dataval = univPars.getParameterValue(std::string(namepar));
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
        for(int kk=0;kk<Thinbins->GetNbins_meson_incident();kk++){
          sprintf(namepar,"ThinTarget_%s_incident_%s_%d",nuinc[ii],cpro[jj],kk);
          double dataval = univPars.getParameterValue(std::string(namepar));
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
    bin_prtleftover_inc = univPars.getParameterValue(std::string(namepar));
    sprintf(namepar,"ThinTarget_neuleftover_incident_%d",0);
    bin_neuleftover_inc = univPars.getParameterValue(std::string(namepar));
    
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
    std::string mode(getenv("MODE"));
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
    
    if((mode=="REF")||(mode=="OPT")){
      is_data_based = (aa.Inc_P >= 12.0) && (aa.Vol != "TargetNoSplitSegment" && aa.Vol != "TargetFinHorizontal") && 
	(aa.Prod_pdg == 211 || aa.Prod_pdg == -211 || aa.Prod_pdg ==321 || aa.Prod_pdg == -321 || aa.Prod_pdg ==310 || aa.Prod_pdg == 130) &&
	(bin>=0);
    }
    double inc_mom[3]  = {aa.Inc_P4[0], aa.Inc_P4[1], aa.Inc_P4[2]};
    double prod_mom[3] = {aa.Prod_P4[0],aa.Prod_P4[1],aa.Prod_P4[2]};
    double vtx_int[3]  = {aa.Vtx[0],aa.Vtx[1],aa.Vtx[2]};
    std::string tgtent = "TGT1";
    if((mode=="REF")||(mode=="OPT"))tgtent="TargetFinHorizontal";
    InteractionData aux_aa2(aa.gen,inc_mom,aa.Inc_pdg,prod_mom,aa.Prod_pdg,tgtent,aa.Proc,vtx_int);
    
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
	  
          if(tt_pCPionRew->canReweight(aux_aa2)){
            wgt = tt_pCPionRew->calculateWeight(aux_aa2);
            if(aux_aa2.gen == 0) wgt *= fact_gen0;
          }
          else not_handled = true;
        }      

        else if(aa.Prod_pdg == 321 || aa.Prod_pdg == -321 || aa.Prod_pdg == 130 || aa.Prod_pdg == 310){

          if(iUniv==-1)tt_pCKaonRew = (makerew->cv_rw)->THINTARGET_PC_KAON_Universe;
          else tt_pCKaonRew = (makerew->vec_rws[iUniv])->THINTARGET_PC_KAON_Universe;    
	  
          if(tt_pCKaonRew->canReweight(aux_aa2)){
            wgt = tt_pCKaonRew->calculateWeight(aux_aa2);
            if(aux_aa2.gen == 0) wgt *= fact_gen0;
          }
          else not_handled = true;
        }
        else not_handled = true;
      }
      else if(aa.Inc_pdg == 2112){
        if(aa.Prod_pdg == 211 || aa.Prod_pdg == -211){

          if(iUniv==-1)tt_nCPionRew = (makerew->cv_rw)->THINTARGET_NC_PION_Universe;
          else tt_nCPionRew = (makerew->vec_rws[iUniv])->THINTARGET_NC_PION_Universe;    

          if(tt_nCPionRew->canReweight(aux_aa2)){
            wgt = tt_nCPionRew->calculateWeight(aux_aa2);
            if(aux_aa2.gen == 0) wgt *= fact_gen0;
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
