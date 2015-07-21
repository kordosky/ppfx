
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
  
  void ThinTargetBins::pip_na49_from_xml(const char* filename){
    using boost::property_tree::ptree;
    ptree top;    
    read_xml(filename,top,2); 
  }

  void ThinTargetBins::pim_na49_from_xml(const char* filename){
    using boost::property_tree::ptree;
    ptree top;    
    read_xml(filename,top,2); 
  }
  void ThinTargetBins::kap_na49_from_xml(const char* filename){
    using boost::property_tree::ptree;
    ptree top;    
    read_xml(filename,top,2); 
  }

  void ThinTargetBins::kam_na49_from_xml(const char* filename){
    using boost::property_tree::ptree;
    ptree top;    
    read_xml(filename,top,2); 
  }
  void ThinTargetBins::prt_na49_from_xml(const char* filename){
    using boost::property_tree::ptree;
    ptree top;    
    read_xml(filename,top,2); 
  }

  void ThinTargetBins::neu_na49_from_xml(const char* filename){
    using boost::property_tree::ptree;
    ptree top;    
    read_xml(filename,top,2); 
  }

  int ThinTargetBins::BinID(double pz, double pt,int pdgcode){
    
    int ibinID = -1;
    int size = 0;
    
    if(pdgcode == 211){
      size = pip_na49_pzmin.size();
      for(int ii=0;ii<size;ii++){
	if(pz>pip_na49_pzmin[ii] && pz<pip_na49_pzmax[ii] && pt>pip_na49_ptmin[ii] && pt<pip_na49_ptmax[ii]){
	  ibinID = ii;
	}
      }
    }
    if(pdgcode == -211){
      size = pim_na49_pzmin.size();
      for(int ii=0;ii<size;ii++){
	if(pz>pim_na49_pzmin[ii] && pz<pim_na49_pzmax[ii] && pt>pim_na49_ptmin[ii] && pt<pim_na49_ptmax[ii]){
	  ibinID = ii;
	}
      }
    }
    if(pdgcode == 321){
      size = kap_na49_pzmin.size();
      for(int ii=0;ii<size;ii++){
	if(pz>kap_na49_pzmin[ii] && pz<kap_na49_pzmax[ii] && pt>kap_na49_ptmin[ii] && pt<kap_na49_ptmax[ii]){
	  ibinID = ii;
	}
      }
    }
    if(pdgcode == -321){
      size = kam_na49_pzmin.size();
      for(int ii=0;ii<size;ii++){
	if(pz>kam_na49_pzmin[ii] && pz<kam_na49_pzmax[ii] && pt>kam_na49_ptmin[ii] && pt<kam_na49_ptmax[ii]){
	  ibinID = ii;
	}
      }
    }
    if(pdgcode == 2212){
      size = prt_na49_pzmin.size();
      for(int ii=0;ii<size;ii++){
	if(pz>prt_na49_pzmin[ii] && pz<prt_na49_pzmax[ii] && pt>prt_na49_ptmin[ii] && pt<prt_na49_ptmax[ii]){
	  ibinID = ii;
	}
      }
    }
    if(pdgcode == 2112){
      size = neu_na49_pzmin.size();
      for(int ii=0;ii<size;ii++){
	if(pz>neu_na49_pzmin[ii] && pz<neu_na49_pzmax[ii] && pt>neu_na49_ptmin[ii] && pt<neu_na49_ptmax[ii]){
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
