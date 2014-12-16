
#include "NA49Reweighter.h"
#include "DataHistos.h"
#include <iostream>

namespace NeutrinoFluxReweight{
  
  NA49Reweighter::NA49Reweighter(){
    
  }
  
   NA49Reweighter::~NA49Reweighter(){
    
  }
  bool NA49Reweighter::canReweight(const InteractionData& aa){
    
     //Currently we are correcting interactions charged pion/kaons and protons from proton interacions.  
    bool is_inter = (aa.Inc_pdg == 2212) && 
      (abs(aa.Prod_pdg)==211 || abs(aa.Prod_pdg)==321 || aa.Prod_pdg==2212); 
 
    //The minimum incident momentum is 12 GeV:
    bool is_mom = aa.Inc_P >= 12.0;
    //    if(is_inter && is_mom)std::cout<< "TEST " <<aa.Inc_pdg<<" -> "<<aa.Prod_pdg <<std::endl;
    //NA49-like: NA49, Barton+NA49 or MIPP(thin) + NA49:
    
    bool isNA49     = false;
    bool isBart     = false;

    if(abs(aa.Prod_pdg)==211){
      isNA49 = (aa.xF>-0.1 && aa.xF<=0.5) && (aa.Pt>0.0 && aa.Pt<2.0);
      isBart = (aa.xF> 0.5 && aa.xF< 1.0) && (aa.Pt>0.3 && aa.Pt<0.5);
    }
    else if(abs(aa.Prod_pdg)==321){
      isNA49     = (aa.xF>0.0 && aa.xF<=0.2) && (aa.Pt>0.0 && aa.Pt<0.9);  
    }
    else if(aa.Prod_pdg==2212){
      isNA49 = (aa.xF>-0.8 && aa.xF<=0.95) && (aa.Pt>0.0 && aa.Pt<2.0);
    }
    
    bool is_data_covered = isNA49 || isBart;
    
    //Volume:
    bool in_Target = (aa.Vol == "TGT1") || (aa.Vol == "BudalMonitor");
    bool in_DPIP   = aa.Vol  == "DPIP";
    bool in_DVOL   = aa.Vol  == "DVOL";
    bool in_TGAR   = aa.Vol  == "TGAR";
    bool in_HORNIC = (aa.Vol == "HORN1IC") || (aa.Vol == "HORN2IC");
    
    bool is_wanted_volume = in_Target || in_DPIP || in_DVOL || in_TGAR || in_HORNIC;
    
    if(is_inter && is_mom && is_data_covered && is_wanted_volume){
      return true;
    }
    else{
      return false;
    }
    
  }
  
  double NA49Reweighter::calculateWeight(const InteractionData& thisid, ParameterTable& cv_pars, ParameterTable& univ_pars){
    DataHistos*  dtH =  DataHistos::getInstance();
    
    Parameter p1("test1",1.3);
    Parameter p2("test2",-3.5);
    Parameter p3("test3",1.5);
    cv_pars.setParameter(p1);
    cv_pars.setParameter(p2);
    cv_pars.setParameter(p3);
    return 1.0;
  }
  
  void NA49Reweighter::ConfigureThisUniverse(int iuniv){
    UnivID = iuniv;
  }
}
