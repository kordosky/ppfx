
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
  
  void ThinTargetBins::pip_data_from_xml(const char* filename){
    using boost::property_tree::ptree;
    ptree top;    
    read_xml(filename,top,2); 
  }

  void ThinTargetBins::pim_data_from_xml(const char* filename){
    using boost::property_tree::ptree;
    ptree top;    
    read_xml(filename,top,2); 
  }
  void ThinTargetBins::kap_data_from_xml(const char* filename){
    using boost::property_tree::ptree;
    ptree top;    
    read_xml(filename,top,2); 
  }

  void ThinTargetBins::kam_data_from_xml(const char* filename){
    using boost::property_tree::ptree;
    ptree top;    
    read_xml(filename,top,2); 
  }
  void ThinTargetBins::prt_data_from_xml(const char* filename){
    using boost::property_tree::ptree;
    ptree top;    
    read_xml(filename,top,2); 
  }

  void ThinTargetBins::neu_data_from_xml(const char* filename){
    using boost::property_tree::ptree;
    ptree top;    
    read_xml(filename,top,2); 
  }

  int ThinTargetBins::BinID(double pz, double pt,int pdgcode){
    
    int ibinID = -1;
    int size = 0;
    
    if(pdgcode == 211){
      size = pip_data_pzmin.size();
      for(int ii=0;ii<size;ii++){
	if(pz>pip_data_pzmin[ii] && pz<pip_data_pzmax[ii] && pt>pip_data_ptmin[ii] && pt<pip_data_ptmax[ii]){
	  ibinID = ii;
	}
      }
    }
    if(pdgcode == -211){
      size = pim_data_pzmin.size();
      for(int ii=0;ii<size;ii++){
	if(pz>pim_data_pzmin[ii] && pz<pim_data_pzmax[ii] && pt>pim_data_ptmin[ii] && pt<pim_data_ptmax[ii]){
	  ibinID = ii;
	}
      }
    }
    if(pdgcode == 321){
      size = kap_data_pzmin.size();
      for(int ii=0;ii<size;ii++){
	if(pz>kap_data_pzmin[ii] && pz<kap_data_pzmax[ii] && pt>kap_data_ptmin[ii] && pt<kap_data_ptmax[ii]){
	  ibinID = ii;
	}
      }
    }
    if(pdgcode == -321){
      size = kam_data_pzmin.size();
      for(int ii=0;ii<size;ii++){
	if(pz>kam_data_pzmin[ii] && pz<kam_data_pzmax[ii] && pt>kam_data_ptmin[ii] && pt<kam_data_ptmax[ii]){
	  ibinID = ii;
	}
      }
    }
    if(pdgcode == 2212){
      size = prt_data_pzmin.size();
      for(int ii=0;ii<size;ii++){
	if(pz>prt_data_pzmin[ii] && pz<prt_data_pzmax[ii] && pt>prt_data_ptmin[ii] && pt<prt_data_ptmax[ii]){
	  ibinID = ii;
	}
      }
    }
    if(pdgcode == 2112){
      size = neu_data_pzmin.size();
      for(int ii=0;ii<size;ii++){
	if(pz>neu_data_pzmin[ii] && pz<neu_data_pzmax[ii] && pt>neu_data_ptmin[ii] && pt<neu_data_ptmax[ii]){
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
