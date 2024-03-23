
#include "ThinTargetMesonIncidentReweighter.h"
#include <iostream>
#include "ThinTargetBins.h"

namespace NeutrinoFluxReweight{
  
  ThinTargetMesonIncidentReweighter::ThinTargetMesonIncidentReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars):iUniv(iuniv),cvPars(cv_pars),univPars(univ_pars){
    
    ThinTargetBins* Thinbins =  ThinTargetBins::getInstance();
    
    vbin_pip_inc_pip.reserve(Thinbins->GetNbins_meson_incident());
    vbin_pip_inc_pim.reserve(Thinbins->GetNbins_meson_incident());
    vbin_pip_inc_kap.reserve(Thinbins->GetNbins_meson_incident());
    vbin_pip_inc_kam.reserve(Thinbins->GetNbins_meson_incident());
    vbin_pip_inc_k0.reserve(Thinbins->GetNbins_meson_incident());
    vbin_pip_inc_p.reserve(Thinbins->GetNbins_meson_incident());
    vbin_pip_inc_n.reserve(Thinbins->GetNbins_meson_incident());
    vbin_pim_inc_pip.reserve(Thinbins->GetNbins_meson_incident());
    vbin_pim_inc_pim.reserve(Thinbins->GetNbins_meson_incident());
    vbin_pim_inc_kap.reserve(Thinbins->GetNbins_meson_incident());
    vbin_pim_inc_kam.reserve(Thinbins->GetNbins_meson_incident());
    vbin_pim_inc_k0.reserve(Thinbins->GetNbins_meson_incident());
    vbin_pim_inc_p.reserve(Thinbins->GetNbins_meson_incident());
    vbin_pim_inc_n.reserve(Thinbins->GetNbins_meson_incident());
    vbin_kap_inc_pip.reserve(Thinbins->GetNbins_meson_incident());
    vbin_kap_inc_pim.reserve(Thinbins->GetNbins_meson_incident());
    vbin_kap_inc_kap.reserve(Thinbins->GetNbins_meson_incident());
    vbin_kap_inc_kam.reserve(Thinbins->GetNbins_meson_incident());
    vbin_kap_inc_k0.reserve(Thinbins->GetNbins_meson_incident());
    vbin_kap_inc_p.reserve(Thinbins->GetNbins_meson_incident());
    vbin_kap_inc_n.reserve(Thinbins->GetNbins_meson_incident());
    vbin_kam_inc_pip.reserve(Thinbins->GetNbins_meson_incident());
    vbin_kam_inc_pim.reserve(Thinbins->GetNbins_meson_incident());
    vbin_kam_inc_kap.reserve(Thinbins->GetNbins_meson_incident());
    vbin_kam_inc_kam.reserve(Thinbins->GetNbins_meson_incident());
    vbin_kam_inc_k0.reserve(Thinbins->GetNbins_meson_incident());
    vbin_kam_inc_p.reserve(Thinbins->GetNbins_meson_incident());
    vbin_kam_inc_n.reserve(Thinbins->GetNbins_meson_incident());
    vbin_k0_inc_pip.reserve(Thinbins->GetNbins_meson_incident());
    vbin_k0_inc_pim.reserve(Thinbins->GetNbins_meson_incident());
    vbin_k0_inc_kap.reserve(Thinbins->GetNbins_meson_incident());
    vbin_k0_inc_kam.reserve(Thinbins->GetNbins_meson_incident());
    vbin_k0_inc_k0.reserve(Thinbins->GetNbins_meson_incident());
    vbin_k0_inc_p.reserve(Thinbins->GetNbins_meson_incident());
    vbin_k0_inc_n.reserve(Thinbins->GetNbins_meson_incident());

    // const boost::interprocess::flat_map<std::string, double>& cv_table   = cvPars.getMap();
    // const boost::interprocess::flat_map<std::string, double>& univ_table = univPars.getMap();
    char namepar[100];

    //meson left over:
    sprintf(namepar,"ThinTarget_mesonleftover_incident_%d",0);
    double dataval = univPars.getParameterValue(std::string(namepar));
    bin_mesonleftover_inc = dataval;
    
    //5 incident particles, 7 produced particles:
    const char* cinc[5] = {"pip","pim","kap","kam","k0"};
    const char* cpro[7] = {"pip","pim","kap","kam","k0","p","n"};
    
    for(int ii=0;ii<5;ii++){
      for(int jj=0;jj<7;jj++){
	for(int kk=0;kk<Thinbins->GetNbins_meson_incident();kk++){
	  
	  sprintf(namepar,"ThinTarget_%s_incident_%s_%d",cinc[ii],cpro[jj],kk);
	  dataval = univPars.getParameterValue(std::string(namepar));
	  if(ii==0 && jj==0)vbin_pip_inc_pip.push_back(dataval);
	  if(ii==0 && jj==1)vbin_pip_inc_pim.push_back(dataval);
	  if(ii==0 && jj==2)vbin_pip_inc_kap.push_back(dataval);
	  if(ii==0 && jj==3)vbin_pip_inc_kam.push_back(dataval);
	  if(ii==0 && jj==4)vbin_pip_inc_k0.push_back(dataval);
	  if(ii==0 && jj==5)vbin_pip_inc_p.push_back(dataval);
	  if(ii==0 && jj==6)vbin_pip_inc_n.push_back(dataval);
	  if(ii==1 && jj==0)vbin_pim_inc_pip.push_back(dataval);
	  if(ii==1 && jj==1)vbin_pim_inc_pim.push_back(dataval);
	  if(ii==1 && jj==2)vbin_pim_inc_kap.push_back(dataval);
	  if(ii==1 && jj==3)vbin_pim_inc_kam.push_back(dataval);
	  if(ii==1 && jj==4)vbin_pim_inc_k0.push_back(dataval);
	  if(ii==1 && jj==5)vbin_pim_inc_p.push_back(dataval);
	  if(ii==1 && jj==6)vbin_pim_inc_n.push_back(dataval);
	  if(ii==2 && jj==0)vbin_kap_inc_pip.push_back(dataval);
	  if(ii==2 && jj==1)vbin_kap_inc_pim.push_back(dataval);
	  if(ii==2 && jj==2)vbin_kap_inc_kap.push_back(dataval);
	  if(ii==2 && jj==3)vbin_kap_inc_kam.push_back(dataval);
	  if(ii==2 && jj==4)vbin_kap_inc_k0.push_back(dataval);
	  if(ii==2 && jj==5)vbin_kap_inc_p.push_back(dataval);
	  if(ii==2 && jj==6)vbin_kap_inc_n.push_back(dataval);
	  if(ii==3 && jj==0)vbin_kam_inc_pip.push_back(dataval);
	  if(ii==3 && jj==1)vbin_kam_inc_pim.push_back(dataval);
	  if(ii==3 && jj==2)vbin_kam_inc_kap.push_back(dataval);
	  if(ii==3 && jj==3)vbin_kam_inc_kam.push_back(dataval);
	  if(ii==3 && jj==4)vbin_kam_inc_k0.push_back(dataval);
	  if(ii==3 && jj==5)vbin_kam_inc_p.push_back(dataval);
	  if(ii==3 && jj==6)vbin_kam_inc_n.push_back(dataval);
	  if(ii==4 && jj==0)vbin_k0_inc_pip.push_back(dataval);
	  if(ii==4 && jj==1)vbin_k0_inc_pim.push_back(dataval);
	  if(ii==4 && jj==2)vbin_k0_inc_kap.push_back(dataval);
	  if(ii==4 && jj==3)vbin_k0_inc_kam.push_back(dataval);
	  if(ii==4 && jj==4)vbin_k0_inc_k0.push_back(dataval);
	  if(ii==4 && jj==5)vbin_k0_inc_p.push_back(dataval);
	  if(ii==4 && jj==6)vbin_k0_inc_n.push_back(dataval);
	}
      }
    }
  }
  
  ThinTargetMesonIncidentReweighter::~ThinTargetMesonIncidentReweighter(){
    
  }
  bool ThinTargetMesonIncidentReweighter::canReweight(const InteractionData& aa){
    /*    
    if(aa.Inc_pdg != 211 && aa.Inc_pdg != -211 && aa.Inc_pdg != 321 && aa.Inc_pdg != -321 && aa.Inc_pdg != 130 && aa.Inc_pdg != 310)return false;
    if(aa.Prod_pdg != 211 && aa.Prod_pdg != -211 && aa.Prod_pdg != 321 && aa.Prod_pdg != -321 && aa.Prod_pdg != 130 && aa.Prod_pdg != 310 && aa.Prod_pdg != 2212 && aa.Prod_pdg != 2112)return false;
    */
    
    // this returns false if Inelastic is not in the Process string
    // should test if the result of find == std::string::npos  (no position)
    if(aa.Proc.find("Inelastic")>100)return false;
    
    //  ThinTargetBins*  Thinbins =  ThinTargetBins::getInstance();
    //int bin      = Thinbins->meson_inc_BinID(aa.xF,aa.Pt,aa.Prod_pdg);

    // this test will be true for all mesons, including exotic ones (JLab friends), but those are not
    // produced by GEANT
    bool is_mesoninc = (aa.Inc_pdg >99 && aa.Inc_pdg < 1000) || (aa.Inc_pdg <-99 && aa.Inc_pdg > -1000);

    //    if(bin>=0 || is_mesoninc)return true;
    if(is_mesoninc)return true;
    else return false;
    
  }
  
  double ThinTargetMesonIncidentReweighter::calculateWeight(const InteractionData& aa){

    double wgt = 1.0;

    bool right_inc = aa.Inc_pdg == 211 || aa.Inc_pdg == -211 || aa.Inc_pdg == 321 || aa.Inc_pdg == -321 || aa.Inc_pdg == 130 || aa.Inc_pdg == 310;
    bool right_prod = aa.Prod_pdg == 211 || aa.Prod_pdg == -211 || aa.Prod_pdg == 321 || aa.Prod_pdg == -321 || aa.Prod_pdg == 130 || aa.Prod_pdg == 310 || aa.Prod_pdg == 2212 || aa.Prod_pdg == 2112;
    ThinTargetBins*  Thinbins =  ThinTargetBins::getInstance();
    int bin      = Thinbins->meson_inc_BinID(aa.xF,aa.Pt,aa.Prod_pdg);
    bool is_mesoninc = (aa.Inc_pdg >99 && aa.Inc_pdg < 1000) || (aa.Inc_pdg <-99 && aa.Inc_pdg > -1000);
    if(bin>=0 && right_inc && right_prod){

      // same treatment as nucleon-A reweighter for xF < 0
      double negxF_corrunc = 1.;
      if(aa.xF < 0.) negxF_corrunc = bin_mesonleftover_inc;

      if(aa.Inc_pdg == 211){
	if(aa.Prod_pdg == 211) wgt = vbin_pip_inc_pip[bin] * negxF_corrunc;
	if(aa.Prod_pdg ==-211) wgt = vbin_pip_inc_pim[bin] * negxF_corrunc;
	if(aa.Prod_pdg == 321) wgt = vbin_pip_inc_kap[bin] * negxF_corrunc;
	if(aa.Prod_pdg ==-321) wgt = vbin_pip_inc_kam[bin] * negxF_corrunc;
	if(aa.Prod_pdg ==130 || aa.Prod_pdg ==310) wgt = vbin_pip_inc_k0[bin] * negxF_corrunc;
	if(aa.Prod_pdg ==2212) wgt = vbin_pip_inc_p[bin] * negxF_corrunc;
	if(aa.Prod_pdg ==2112) wgt = vbin_pip_inc_n[bin] * negxF_corrunc;	
      }
      else if(aa.Inc_pdg ==-211){
	if(aa.Prod_pdg == 211) wgt = vbin_pim_inc_pip[bin] * negxF_corrunc;
	if(aa.Prod_pdg ==-211) wgt = vbin_pim_inc_pim[bin] * negxF_corrunc;
	if(aa.Prod_pdg == 321) wgt = vbin_pim_inc_kap[bin] * negxF_corrunc;
	if(aa.Prod_pdg ==-321) wgt = vbin_pim_inc_kam[bin] * negxF_corrunc;
	if(aa.Prod_pdg ==130 || aa.Prod_pdg ==310) wgt = vbin_pim_inc_k0[bin] * negxF_corrunc;
	if(aa.Prod_pdg ==2212) wgt = vbin_pim_inc_p[bin] * negxF_corrunc;
	if(aa.Prod_pdg ==2112) wgt = vbin_pim_inc_n[bin] * negxF_corrunc;	
      }
      else if(aa.Inc_pdg == 321){
	if(aa.Prod_pdg == 211) wgt = vbin_kap_inc_pip[bin] * negxF_corrunc;
	if(aa.Prod_pdg ==-211) wgt = vbin_kap_inc_pim[bin] * negxF_corrunc;
	if(aa.Prod_pdg == 321) wgt = vbin_kap_inc_kap[bin] * negxF_corrunc;
	if(aa.Prod_pdg ==-321) wgt = vbin_kap_inc_kam[bin] * negxF_corrunc;
	if(aa.Prod_pdg ==130 || aa.Prod_pdg ==310) wgt = vbin_kap_inc_k0[bin] * negxF_corrunc;
	if(aa.Prod_pdg ==2212) wgt = vbin_kap_inc_p[bin] * negxF_corrunc;
	if(aa.Prod_pdg ==2112) wgt = vbin_kap_inc_n[bin] * negxF_corrunc;	
      }
      else if(aa.Inc_pdg ==-321){
	if(aa.Prod_pdg == 211) wgt = vbin_kam_inc_pip[bin] * negxF_corrunc;
	if(aa.Prod_pdg ==-211) wgt = vbin_kam_inc_pim[bin] * negxF_corrunc;
	if(aa.Prod_pdg == 321) wgt = vbin_kam_inc_kap[bin] * negxF_corrunc;
	if(aa.Prod_pdg ==-321) wgt = vbin_kam_inc_kam[bin] * negxF_corrunc;
	if(aa.Prod_pdg ==130 || aa.Prod_pdg ==310) wgt = vbin_kam_inc_k0[bin] * negxF_corrunc;
	if(aa.Prod_pdg ==2212) wgt = vbin_kam_inc_p[bin] * negxF_corrunc;
	if(aa.Prod_pdg ==2112) wgt = vbin_kam_inc_n[bin] * negxF_corrunc;	
      }
      else if(aa.Inc_pdg == 130 || aa.Inc_pdg == 310){
	if(aa.Prod_pdg == 211) wgt = vbin_k0_inc_pip[bin] * negxF_corrunc;
	if(aa.Prod_pdg ==-211) wgt = vbin_k0_inc_pim[bin] * negxF_corrunc;
	if(aa.Prod_pdg == 321) wgt = vbin_k0_inc_kap[bin] * negxF_corrunc;
	if(aa.Prod_pdg ==-321) wgt = vbin_k0_inc_kam[bin] * negxF_corrunc;
	if(aa.Prod_pdg ==130 || aa.Prod_pdg ==310) wgt = vbin_k0_inc_k0[bin] * negxF_corrunc;
	if(aa.Prod_pdg ==2212) wgt = vbin_k0_inc_p[bin] * negxF_corrunc;
	if(aa.Prod_pdg ==2112) wgt = vbin_k0_inc_n[bin] * negxF_corrunc;	
      }
      else{
	std::cout<<"MESINC Something is wrong with pdg_inc: "<< aa.Inc_pdg  <<" "<<aa.Prod_pdg<<std::endl;
	return wgt;
      }
    }
    else if(is_mesoninc){
      wgt = bin_mesonleftover_inc;
    }
    
    if(wgt<0){
      //std::cout<<"TTMESONINC check wgt(<0) "<<iUniv<<" "<<aa.Inc_P<<" "<<aa.xF<<" "<<aa.Pt<<" "<<aa.Prod_pdg<<std::endl;
      // same as nucleon-A, cap it to near-0 instead of returning 1.
      return 0.0001;
    }
    if(wgt>10){
      std::cout<<"BIG WGT IN TTMESONINC "<<iUniv<<" "<<wgt<<" "<<aa.Inc_P<<" "<<aa.xF<<" "<<aa.Pt<<" "<<aa.Prod_pdg<<std::endl;
      return 1.0;
    }
    return wgt;

  }
  

}
