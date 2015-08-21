
#include "ThinTargetpCKaonReweighter.h"
#include <iostream>
#include "MakeReweight.h"
#include "ThinTargetBins.h"
#include "DataHistos.h"
#include "ThinTargetMC.h"

namespace NeutrinoFluxReweight{
  
  ThinTargetpCKaonReweighter::ThinTargetpCKaonReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars):iUniv(iuniv),cvPars(cv_pars),univPars(univ_pars){
  
    vbin_data_kap.reserve(1353);
    vbin_data_kam.reserve(1353);
    mipp_vbin_data_kap_pip.reserve(24);
    mipp_vbin_data_kam_pim.reserve(24);

    std::map<std::string, double> cv_table   = cvPars.table;
    std::map<std::string, double> univ_table = univPars.table;
       
    data_prod_xs = univ_table["prod_prtC_xsec"];
    char namepar[100];
    for(int ii=0;ii<1353;ii++){
      
      sprintf(namepar,"ThinTargetLowxF_pC_%s_sys_%d","kap",ii);
      double data_cv  = cv_table[std::string(namepar)];
      double data_sys = univ_table[std::string(namepar)];
      sprintf(namepar,"ThinTargetLowxF_pC_%s_stats_%d","kap",ii);
      double data_sta = univ_table[std::string(namepar)];
      vbin_data_kap.push_back(data_sta + data_sys - data_cv);
      
      sprintf(namepar,"ThinTargetLowxF_pC_%s_sys_%d","kam",ii);
      data_cv  = cv_table[std::string(namepar)];
      data_sys = univ_table[std::string(namepar)];
      sprintf(namepar,"ThinTargetLowxF_pC_%s_stats_%d","kam",ii);
      data_sta = univ_table[std::string(namepar)];
      vbin_data_kam.push_back(data_sta + data_sys - data_cv);
      
    }
    
   for(int ii=0;ii<24;ii++){
      
      sprintf(namepar,"ThinTarget_kap_pip_sys_%d",ii);
      double data_cv  = cv_table[std::string(namepar)];
      double data_sys = univ_table[std::string(namepar)];
      sprintf(namepar,"ThinTarget_kap_pip_stats_%d",ii);
      double data_sta = univ_table[std::string(namepar)];
      mipp_vbin_data_kap_pip.push_back(data_sta + data_sys - data_cv);

      sprintf(namepar,"ThinTarget_kam_pim_sys_%d",ii);
      data_cv  = cv_table[std::string(namepar)];
      data_sys = univ_table[std::string(namepar)];
      sprintf(namepar,"ThinTarget_kam_pim_stats_%d",ii);
      data_sta = univ_table[std::string(namepar)];
      mipp_vbin_data_kam_pim.push_back(data_sta + data_sys - data_cv);

    } 
    
  }
  
  ThinTargetpCKaonReweighter::~ThinTargetpCKaonReweighter(){
    
  }
  bool ThinTargetpCKaonReweighter::canReweight(const InteractionData& aa){
     //checking:
    if(aa.Inc_pdg != 2112)return false;
    if(aa.Inc_P < 12.0)return false;
    if(aa.Vol != "TGT1" && aa.Vol != "BudalMonitor")return false;
    if(aa.Prod_pdg != 321 && aa.Prod_pdg != -321)return false;
    
    //Looking for if there is kaon:
    ThinTargetBins*  Thinbins =  ThinTargetBins::getInstance();
    int bin      = Thinbins->BinID_pC_k(aa.xF,aa.Pt,aa.Prod_pdg);
    if(bin>=0)return true; //found NA49 kaon 
    
    //now looking for high energy kaon   
    int mipp_bin = Thinbins->mipp_BinID_pC_k(aa.Pz,aa.Pt,aa.Prod_pdg);
    if(mipp_bin<0)return false;
    //Looking for the corresponding pion:
    double inc_mom[3]  = {aa.Inc_P4[0], aa.Inc_P4[1], aa.Inc_P4[2]};
    double prod_mom[3] = {aa.Prod_P4[0],aa.Prod_P4[1],aa.Prod_P4[2]};
    double vtx_int[3]  = {aa.Vtx[0],aa.Vtx[1],aa.Vtx[2]};
    int pion_pdg = 211;
    if(aa.Prod_pdg == -321)pion_pdg = -211;
    
    MakeReweight*  makerew =  MakeReweight::getInstance();
    if(iUniv==-1)tt_pCPionRew = (makerew->cv_rw)->THINTARGET_PC_PION_Universe;
    else tt_pCPionRew = (makerew->vec_rws[iUniv])->THINTARGET_PC_PION_Universe;   
    aux_aa = new InteractionData(aa.gen, inc_mom,2212,prod_mom,pion_pdg,aa.Vol,aa.Proc,vtx_int);   
    bool there_is_pion = tt_pCPionRew->canReweight(*aux_aa);    
    
    return there_is_pion;
    
  }
  
  double ThinTargetpCKaonReweighter::calculateWeight(const InteractionData& aa){
 
    double wgt = 1.0;
    ThinTargetBins*  Thinbins =  ThinTargetBins::getInstance();

    int bin      = Thinbins->BinID_pC_k(aa.xF,aa.Pt,aa.Prod_pdg);
    int mipp_bin = Thinbins->mipp_BinID_pC_k(aa.Pz,aa.Pt,aa.Prod_pdg);
    if(bin<0 && mipp_bin<0)return wgt;  //double check

    double dataval = -1;
    
    if(bin>=0){
      if(aa.Prod_pdg == 321)dataval = vbin_data_kap[bin];
      else if(aa.Prod_pdg == -321) dataval = vbin_data_kam[bin];
      else{
	std::cout<<"Something is wrong, pdg_prod: "<< aa.Prod_pdg  <<std::endl;
	return wgt;
      }
    }
    else if(mipp_bin>=0){
      if(aa.Prod_pdg == 321)dataval = mipp_vbin_data_kap_pip[mipp_bin];
      else if(aa.Prod_pdg == -321) dataval = mipp_vbin_data_kam_pim[mipp_bin];
      else{
	std::cout<<"Something is wrong, pdg_prod: "<< aa.Prod_pdg  <<std::endl;
	return wgt;
      }
      //Looking for the corresponding pion:
      double inc_mom[3]  = {aa.Inc_P4[0], aa.Inc_P4[1], aa.Inc_P4[2]};
      double prod_mom[3] = {aa.Prod_P4[0],aa.Prod_P4[1],aa.Prod_P4[2]};
      double vtx_int[3]  = {aa.Vtx[0],aa.Vtx[1],aa.Vtx[2]};
      int pi_pdg = 211;
      if(aa.Prod_pdg == -321)pi_pdg = -211;
      
      aux_aa = new InteractionData(aa.gen, inc_mom,2212,prod_mom,pi_pdg,aa.Vol,aa.Proc,vtx_int);    
      int bin_pi = -1;
      if(aux_aa->xF<=0.5)bin_pi = Thinbins->BinID_pC_pi(aux_aa->xF,aux_aa->Pt,aux_aa->Prod_pdg);
      if(aux_aa->xF>0.5) bin_pi = Thinbins->barton_BinID_pC_pi(aux_aa->xF,aux_aa->Pt,aux_aa->Prod_pdg);
      if(bin_pi<0){
	std::cout<<"Something is wrong, no bin pion"<<std::endl;
	return wgt;
      }
      
      MakeReweight*  makerew =  MakeReweight::getInstance();
      if(iUniv==-1)tt_pCPionRew = (makerew->cv_rw)->THINTARGET_PC_PION_Universe;
      else tt_pCPionRew = (makerew->vec_rws[iUniv])->THINTARGET_PC_PION_Universe;   
      
      double dataval_pi = -1;
      if(pi_pdg== 211 && aux_aa->xF<=0.5)dataval_pi = tt_pCPionRew->vbin_data_pip[bin_pi];
      if(pi_pdg==-211 && aux_aa->xF<=0.5)dataval_pi = tt_pCPionRew->vbin_data_pim[bin_pi];
      if(pi_pdg== 211 && aux_aa->xF>0.5) dataval_pi = tt_pCPionRew->bart_vbin_data_pip[bin_pi];
      if(pi_pdg==-211 && aux_aa->xF>0.5) dataval_pi = tt_pCPionRew->bart_vbin_data_pim[bin_pi];
      if(dataval_pi<0){
	std::cout<<"Something is wrong, pion xs <0"<<std::endl;
	return wgt;
      }
      dataval *= dataval_pi;
    }
     
    //checking if this is the first interaction:
    if(aa.gen==0)dataval /= data_prod_xs;
    else if(aa.gen>0)dataval /= 1.0;
    else{
      std::cout<<"Something is wrong with gen "<<std::endl;
      return wgt;
    }
    
    double data_scale = calculateDataScale(aa.Inc_pdg,aa.Inc_P,aa.Prod_pdg,aa.xF,aa.Pt);   
    dataval *= data_scale;
    
    ThinTargetMC*  mc =  ThinTargetMC::getInstance();
    double mc_cv = mc->getMCval_pC_X(aa.Inc_P,aa.xF,aa.Pt,aa.Prod_pdg);
    mc_cv /= calculateMCProd(aa.gen,aa.Inc_P);
    if(mc_cv<1.e-12)return wgt;
    wgt = dataval/mc_cv;

    if(wgt<0){
      //std::cout<<"TTPCK check wgt(<0) "<<iUniv<<" "<<aa.Inc_P<<" "<<aa.xF<<" "<<aa.Pt<<" "<<aa.Prod_pdg<<std::endl;
      return 1.0;
    }
    
    bool isk0 = aa.Prod_pdg ==130 || aa.Prod_pdg ==310 ;
    ThinTargetpCKaonReweighter::wgt_na49 = 1.0;
    ThinTargetpCKaonReweighter::wgt_mipp = 1.0;
    ThinTargetpCKaonReweighter::wgt_k0 = 1.0;
    if(bin>=0 && !isk0)ThinTargetpCKaonReweighter::wgt_na49 = wgt;
    else if(mipp_bin>=0 && !isk0)ThinTargetpCKaonReweighter::wgt_mipp = wgt;
    
    if(isk0)ThinTargetpCKaonReweighter::wgt_k0 = wgt;
    
    return wgt;
    
  }
  
double ThinTargetpCKaonReweighter::calculateMCProd(int genid, double inc_mom){
    double xx[13] ={12,20,31,40,50,60,70,80,90,100,110,120,158};
    double yy[13] ={153386793./197812683.,160302538./197811564.,164508480./197831250.,166391359./197784915.,
		    167860919./197822312.,168882647./197807739.,169681805./197803099.,170311264./197811098.,
		    170860912./197822002.,171309291./197834756.,171651963./197811822.,171991260./197823012.,
		  172902228./197804669.};
    
    int idx_lowp = -1;
    int idx_hip  = -1;
    for(int i=0;i<12;i++){
      if(inc_mom>=double(xx[i]) && inc_mom<double(xx[i+1])){
	idx_lowp=i;
	idx_hip =i+1;
      }
    }
    double frac_low = yy[idx_lowp];
    double frac_hi  = yy[idx_hip];
    double frac_m   =  frac_low + (inc_mom-double(xx[idx_lowp]))*(frac_hi-frac_low)/(double(xx[idx_hip])-double(xx[idx_lowp]));
    
    if(genid==0)return frac_m*243.2435;
    else if(genid>0)return frac_m;
    else{
      std::cout<<"Something is wrong with gen "<<std::endl;
      return 1.0;
    }
    
  }
  double ThinTargetpCKaonReweighter::calculateDataScale(int inc_pdg, double inc_mom, int prod_pdg,double xf, double pt){
    double scaling_violation = 1.0;
    DataHistos*  dtH =  DataHistos::getInstance();
    //temporary:
    const int Nscl = 11;
    const int moms[Nscl] = {12,20,31,40,50,60,70,80,100,120,158};
    
    int idx_part = -1;
    if(prod_pdg == 321)idx_part = 2;
    if(prod_pdg ==-321)idx_part = 3;
    if(idx_part<0){
      std::cout<<"Error in the prod particle"<<std::endl;
      return 1.0;
    }
    
    int binid = dtH->hNA49Scl[idx_part][Nscl-1]->FindBin(xf,pt);
    double scl_ref158 = dtH->hNA49Scl[idx_part][Nscl-1]->GetBinContent(binid);    
    
    int idx_lowp = -1;
    int idx_hip  = -1;
    for(int i=0;i<Nscl-1;i++){
      if(inc_mom>=double(moms[i]) && inc_mom<double(moms[i+1])){
	idx_lowp=i;
	idx_hip =i+1;
      }
    }
    if(idx_lowp<0 || idx_hip<0){
      std::cout<<"Error calculating the scaling"<<std::endl;
      return 1.0;
    }
    double scl_low = dtH->hNA49Scl[idx_part][idx_lowp]->GetBinContent(binid);
    double scl_hi  = dtH->hNA49Scl[idx_part][idx_hip]->GetBinContent(binid);
    double scl_m   =  scl_low + (inc_mom-double(moms[idx_lowp]))*(scl_hi-scl_low)/(double(moms[idx_hip])-double(moms[idx_lowp]));
    if(scl_ref158<1.e-10){
      // std::cout<<"ref158 zero!!! "<<scl_ref158<<std::endl;
      return 1.0;
    }
    scaling_violation = scl_m/scl_ref158;
    return scaling_violation;
  }

}
