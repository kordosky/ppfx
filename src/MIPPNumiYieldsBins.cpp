

#include "MIPPNumiYieldsBins.h"
#include <set>
#include <string>
#include <iostream>
#include <sstream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>


namespace NeutrinoFluxReweight{ 
  
  MIPPNumiYieldsBins* MIPPNumiYieldsBins::instance = 0;
  
  MIPPNumiYieldsBins::MIPPNumiYieldsBins(){
  }
  
  void MIPPNumiYieldsBins::readPIP_FromXML(const char* filename){
    using boost::property_tree::ptree;
    ptree top;
    
    read_xml(filename,top,2); // option 2 removes comment strings
    
    ptree& binsPIP = top.get_child("bins.MIPP_Numi_pip");
     
    ptree::iterator it = binsPIP.begin();
    int idx=0;
    double aux_pzmin,aux_pzmax,aux_ptmin,aux_ptmax;

    for(; it!=binsPIP.end(); it++){
      // it->first is the name
      // it->second is the child property tree
      std::string pz_string=it->second.get<std::string>("pzrange");
      std::string pt_string=it->second.get<std::string>("ptrange");
      
      std::stringstream ss1(pz_string);
      std::stringstream ss2(pt_string);
      ss1 >> aux_pzmin >> aux_pzmax;
      ss2 >> aux_ptmin >> aux_ptmax;
   
      pip_pzmin.push_back(aux_pzmin);
      pip_pzmax.push_back(aux_pzmax);
      pip_ptmin.push_back(aux_ptmin);
      pip_ptmax.push_back(aux_ptmax);
    }
  }

  void MIPPNumiYieldsBins::readPIM_FromXML(const char* filename){
    using boost::property_tree::ptree;
    ptree top;
    
    read_xml(filename,top,2); // option 2 removes comment strings
    
    ptree& binsPIM = top.get_child("bins.MIPP_Numi_pim");
     
    ptree::iterator it = binsPIM.begin();
    double aux_pzmin,aux_pzmax,aux_ptmin,aux_ptmax;

    for(; it!=binsPIM.end(); it++){
      // it->first is the name
      // it->second is the child property tree
      std::string pz_string=it->second.get<std::string>("pzrange");
      std::string pt_string=it->second.get<std::string>("ptrange");
      
      std::stringstream ss1(pz_string);
      std::stringstream ss2(pt_string);
      ss1 >> aux_pzmin >> aux_pzmax;
      ss2 >> aux_ptmin >> aux_ptmax;
   
      pim_pzmin.push_back(aux_pzmin);
      pim_pzmax.push_back(aux_pzmax);
      pim_ptmin.push_back(aux_ptmin);
      pim_ptmax.push_back(aux_ptmax);
    }
  }
  
  int MIPPNumiYieldsBins::BinID(double pz,double pt, int pdgcode){
    
    int ibinID = -1;
    
    int size = 0;
    if(pdgcode==211){
      size = pip_pzmin.size();
      for(int ii=0;ii<size;ii++){
	if(pz>pip_pzmin[ii] && pz<pip_pzmax[ii] && pt>pip_ptmin[ii] && pt<pip_ptmax[ii]){
	  ibinID = ii;
	}
      }
      
    }
    if(pdgcode==-211){
      size = pim_pzmin.size();
      for(int ii=0;ii<size;ii++){
	if(pz>pim_pzmin[ii] && pz<pim_pzmax[ii] && pt>pim_ptmin[ii] && pt<pim_ptmax[ii]){
	  ibinID = ii;
	}
      }
      
    }
    return ibinID;
    
  }

  MIPPNumiYieldsBins* MIPPNumiYieldsBins::getInstance(){
    if (instance == 0) instance = new MIPPNumiYieldsBins;
    return instance;
  }
  
}
