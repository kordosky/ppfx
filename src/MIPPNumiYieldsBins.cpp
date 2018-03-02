
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
  
  void MIPPNumiYieldsBins::pip_data_from_xml(const char* filename){
    using boost::property_tree::ptree;
    ptree top;
    
    read_xml(filename,top,2); // option 2 removes comment strings
    
    ptree& binsPIP = top.get_child("bins.MIPP_Numi_pip");
     
    ptree::iterator it = binsPIP.begin();
    //int idx=0;
    double aux_pzmin,aux_pzmax,aux_ptmin,aux_ptmax;

    for(; it!=binsPIP.end(); ++it){
      // it->first is the name
      // it->second is the child property tree
      std::string pz_string=it->second.get<std::string>("pzrange");
      std::string pt_string=it->second.get<std::string>("ptrange");
      
      std::stringstream ss1(pz_string);
      std::stringstream ss2(pt_string);
      ss1 >> aux_pzmin >> aux_pzmax;
      ss2 >> aux_ptmin >> aux_ptmax;
   
      pip_data_pzmin.push_back(aux_pzmin);
      pip_data_pzmax.push_back(aux_pzmax);
      pip_data_ptmin.push_back(aux_ptmin);
      pip_data_ptmax.push_back(aux_ptmax);
    }
  }

  void MIPPNumiYieldsBins::pim_data_from_xml(const char* filename){
    using boost::property_tree::ptree;
    ptree top;
    
    read_xml(filename,top,2); // option 2 removes comment strings
    
    ptree& binsPIM = top.get_child("bins.MIPP_Numi_pim");
     
    ptree::iterator it = binsPIM.begin();
    double aux_pzmin,aux_pzmax,aux_ptmin,aux_ptmax;

    for(; it!=binsPIM.end(); ++it){
      // it->first is the name
      // it->second is the child property tree
      std::string pz_string=it->second.get<std::string>("pzrange");
      std::string pt_string=it->second.get<std::string>("ptrange");
      
      std::stringstream ss1(pz_string);
      std::stringstream ss2(pt_string);
      ss1 >> aux_pzmin >> aux_pzmax;
      ss2 >> aux_ptmin >> aux_ptmax;
   
      pim_data_pzmin.push_back(aux_pzmin);
      pim_data_pzmax.push_back(aux_pzmax);
      pim_data_ptmin.push_back(aux_ptmin);
      pim_data_ptmax.push_back(aux_ptmax);
    }
  }
  
  void MIPPNumiYieldsBins::k_pi_data_from_xml(const char* filename){
    using boost::property_tree::ptree;
    ptree top;
    
    read_xml(filename,top,2); 
    
    ptree& binsK_PI = top.get_child("bins.MIPP_Numi_k_pi");
     
    ptree::iterator it = binsK_PI.begin();
    //int idx=0;
    double aux_pzmin,aux_pzmax,aux_ptmin,aux_ptmax;

    for(; it!=binsK_PI.end(); ++it){
      std::string pz_string=it->second.get<std::string>("pzrange");
      std::string pt_string=it->second.get<std::string>("ptrange");
      
      std::stringstream ss1(pz_string);
      std::stringstream ss2(pt_string);
      ss1 >> aux_pzmin >> aux_pzmax;
      ss2 >> aux_ptmin >> aux_ptmax;
   
      k_pi_data_pzmin.push_back(aux_pzmin);
      k_pi_data_pzmax.push_back(aux_pzmax);
      k_pi_data_ptmin.push_back(aux_ptmin);
      k_pi_data_ptmax.push_back(aux_ptmax);
    }
  }

  int MIPPNumiYieldsBins::BinID(double pz,double pt, int pdgcode){

    int ibinID = -1;
    
    int size = 0;
    if(pdgcode==211){
      size = pip_data_pzmin.size();
      for(int ii=0;ii<size;++ii){
	if(pz>pip_data_pzmin[ii] && pz<pip_data_pzmax[ii] && pt>pip_data_ptmin[ii] && pt<pip_data_ptmax[ii]){
	  ibinID = ii;
	}
      }
      
    }
    if(pdgcode==-211){
      size = pim_data_pzmin.size();
      for(int ii=0;ii<size;++ii){
	if(pz>pim_data_pzmin[ii] && pz<pim_data_pzmax[ii] && pt>pim_data_ptmin[ii] && pt<pim_data_ptmax[ii]){
	  ibinID = ii;
	}
      }
      
    }
    if(pdgcode==321 || pdgcode==-321 || pdgcode==130 || pdgcode==310){
      size = k_pi_data_pzmin.size();
      for(int ii=0;ii<size;++ii){
	if(pz>k_pi_data_pzmin[ii] && pz<k_pi_data_pzmax[ii] && pt>k_pi_data_ptmin[ii] && pt<k_pi_data_ptmax[ii]){
	  ibinID = ii;
	}
      }
    }
    return ibinID;
    
  }

  int MIPPNumiYieldsBins::GetNbins_pip_MIPPNuMI(){
    if(pip_data_pzmin.size()==0)throw std::runtime_error("MIPPNumiYieldsBins has not been initialized!!");
    return pip_data_pzmin.size();
  }
  int MIPPNumiYieldsBins::GetNbins_pim_MIPPNuMI(){
    if(pip_data_pzmin.size()==0)throw std::runtime_error("MIPPNumiYieldsBins has not been initialized!!");
    return pim_data_pzmin.size();
  }
  int MIPPNumiYieldsBins::GetNbins_K_MIPPNuMI(){
    if(k_pi_data_pzmin.size()==0)throw std::runtime_error("MIPPNumiYieldsBins has not been initialized!!");
    return k_pi_data_pzmin.size();
  }
  
  MIPPNumiYieldsBins* MIPPNumiYieldsBins::getInstance(){
    if (instance == 0) instance = new MIPPNumiYieldsBins;
    return instance;
  }
  
}
