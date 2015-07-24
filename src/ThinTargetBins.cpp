
#include "ThinTargetBins.h"
#include <set>
#include <string>
#include <iostream>
#include <sstream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace NeutrinoFluxReweight{ 
  
  ThinTargetBins* ThinTargetBins::instance = 0;
  
  ThinTargetBins::ThinTargetBins(){

  }
  
  void ThinTargetBins::pC_pi_from_xml(const char* filename){
    using boost::property_tree::ptree;
    ptree top;    
    read_xml(filename,top,2); 
    ptree& binsPI = top.get_child("bins.ThinTarget_pC_pi"); 
    ptree::iterator it = binsPI.begin();

    int idx=0;
    double aux_xfmin,aux_xfmax,aux_ptmin,aux_ptmax;
    for(; it!=binsPI.end(); it++){
      std::string xf_string=it->second.get<std::string>("xfrange");
      std::string pt_string=it->second.get<std::string>("ptrange");
      
      std::stringstream ss1(xf_string);
      std::stringstream ss2(pt_string);
      ss1 >> aux_xfmin >> aux_xfmax;
      ss2 >> aux_ptmin >> aux_ptmax;
   
      pC_pi_xfmin.push_back(aux_xfmin);
      pC_pi_xfmax.push_back(aux_xfmax);
      pC_pi_ptmin.push_back(aux_ptmin);
      pC_pi_ptmax.push_back(aux_ptmax);
      
    }
    
  }

  int ThinTargetBins::BinID_pC_X(double xf, double pt,int pdgcode){
    
    int ibinID = -1;
    int size = 0;
    
    if(pdgcode == 211 || pdgcode == -211){
      size = pC_pi_xfmin.size();
      for(int ii=0;ii<size;ii++){
	if(xf>pC_pi_xfmin[ii] && xf<pC_pi_xfmax[ii] && pt>pC_pi_ptmin[ii] && pt<pC_pi_ptmax[ii]){
	  ibinID = ii;
	}
      }
    }
    
    return ibinID;
    
  }
  
  ThinTargetBins* ThinTargetBins::getInstance(){
    if (instance == 0) instance = new ThinTargetBins;
    return instance;
  }
  
}
