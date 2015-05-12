
#include "MIPPNumiMC.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>


namespace NeutrinoFluxReweight{ 
  
  MIPPNumiMC* MIPPNumiMC::instance = 0;
  
  MIPPNumiMC::MIPPNumiMC(){
    ranges_already_filled = false;
  }
  
  void MIPPNumiMC::pip_mc_from_xml(const char* filename){
    using boost::property_tree::ptree;
    ptree top;
    read_xml(filename,top,2); 
    std::string namebase = "MIPPNuMI_MC_pip";    
    ptree bins;
    bins = top.get_child("mcbin.MIPPNuMI_MC_pip");
    ptree::iterator it;    
    // we know that pip, pim, kap and kam have the same binning
    double cv,pzmin,pzmax,ptmin,ptmax;
    
    for(it = bins.begin(); it!=bins.end(); it++){
   
      std::string cv_string=it->second.get<std::string>("cvmc");
      std::string pz_string=it->second.get<std::string>("pzrange");
      std::string pt_string=it->second.get<std::string>("ptrange");

      std::stringstream ss1(cv_string);
      std::stringstream ss2(pz_string);
      std::stringstream ss3(pt_string);
      ss1 >> cv;
      ss2 >> pzmin >> pzmax;
      ss3 >> ptmin >> ptmax;
   
      pip_cv.push_back(cv);
      if(ranges_already_filled==false){
	v_pzmin.push_back(pzmin);
	v_pzmax.push_back(pzmax);
	v_ptmin.push_back(ptmin);
	v_ptmax.push_back(ptmax);
      }
      
    }
    ranges_already_filled=true;
    
  }

void MIPPNumiMC::pim_mc_from_xml(const char* filename){
    using boost::property_tree::ptree;
    ptree top;
    read_xml(filename,top,2); 
    std::string namebase = "MIPPNuMI_MC_pim";    
    ptree bins;
    bins = top.get_child("mcbin.MIPPNuMI_MC_pim");
    ptree::iterator it;    
    // we know that pip, pim, kap and kam have the same binning
    double cv,pzmin,pzmax,ptmin,ptmax;
    
    for(it = bins.begin(); it!=bins.end(); it++){
   
      std::string cv_string=it->second.get<std::string>("cvmc");
      std::string pz_string=it->second.get<std::string>("pzrange");
      std::string pt_string=it->second.get<std::string>("ptrange");

      std::stringstream ss1(cv_string);
      std::stringstream ss2(pz_string);
      std::stringstream ss3(pt_string);
      ss1 >> cv;
      ss2 >> pzmin >> pzmax;
      ss3 >> ptmin >> ptmax;
  
      pim_cv.push_back(cv);
      if(ranges_already_filled==false){
	v_pzmin.push_back(pzmin);
	v_pzmax.push_back(pzmax);
	v_ptmin.push_back(ptmin);
	v_ptmax.push_back(ptmax);
      }
    }
    ranges_already_filled=true;
}


void MIPPNumiMC::kap_mc_from_xml(const char* filename){
    using boost::property_tree::ptree;
    ptree top;
    read_xml(filename,top,2); 
    std::string namebase = "MIPPNuMI_MC_kap";    
    ptree bins;
    bins = top.get_child("mcbin.MIPPNuMI_MC_kap");
    ptree::iterator it;    
    // we know that pip, pim, kap and kam have the same binning
    double cv,pzmin,pzmax,ptmin,ptmax;
    
    for(it = bins.begin(); it!=bins.end(); it++){
   
      std::string cv_string=it->second.get<std::string>("cvmc");
      std::string pz_string=it->second.get<std::string>("pzrange");
      std::string pt_string=it->second.get<std::string>("ptrange");
      
      std::stringstream ss1(cv_string);
      std::stringstream ss2(pz_string);
      std::stringstream ss3(pt_string);
      ss1 >> cv;
      ss2 >> pzmin >> pzmax;
      ss3 >> ptmin >> ptmax;
   
      kap_cv.push_back(cv);
      if(ranges_already_filled==false){
	v_pzmin.push_back(pzmin);
	v_pzmax.push_back(pzmax);
	v_ptmin.push_back(ptmin);
	v_ptmax.push_back(ptmax);
      }
    }
    ranges_already_filled=true;
  }

void MIPPNumiMC::kam_mc_from_xml(const char* filename){
    using boost::property_tree::ptree;
    ptree top;
    read_xml(filename,top,2); 
    std::string namebase = "MIPPNuMI_MC_kam";    
    ptree bins;
    bins = top.get_child("mcbin.MIPPNuMI_MC_kam");
    ptree::iterator it;    
    // we know that pip, pim, kap and kam have the same binning
    double cv,pzmin,pzmax,ptmin,ptmax;
    
    for(it = bins.begin(); it!=bins.end(); it++){
   
      std::string cv_string=it->second.get<std::string>("cvmc");
      std::string pz_string=it->second.get<std::string>("pzrange");
      std::string pt_string=it->second.get<std::string>("ptrange");
      std::stringstream ss1(cv_string);
      std::stringstream ss2(pz_string);
      std::stringstream ss3(pt_string);
      ss1 >> cv;
      ss2 >> pzmin >> pzmax;
      ss3 >> ptmin >> ptmax;
   
      kam_cv.push_back(cv);
      if(ranges_already_filled==false){
	v_pzmin.push_back(pzmin);
	v_pzmax.push_back(pzmax);
	v_ptmin.push_back(ptmin);
	v_ptmax.push_back(ptmax);
      }
    }
    ranges_already_filled=true;
  }

  
  double MIPPNumiMC::getMCval(double pz,double pt, int pdgcode){
 
    double cvmc = -1;
    if(abs(pdgcode)!=211 && abs(pdgcode)!=321)return cvmc;
    int size = 0;
   
    //pip:
    if(pdgcode==211){
      size = pip_cv.size();
      for(int ii=0;ii<size;ii++){
	if(pz>v_pzmin[ii] && pz<v_pzmax[ii] && pt>v_ptmin[ii] && pt<v_ptmax[ii]){
	  cvmc = pip_cv[ii];
	}
      }      
    }

    //pim:
    if(pdgcode==-211){
       size = pim_cv.size();
      for(int ii=0;ii<size;ii++){
	if(pz>v_pzmin[ii] && pz<v_pzmax[ii] && pt>v_ptmin[ii] && pt<v_ptmax[ii]){
	  cvmc = pim_cv[ii];
	}
      }  
    }

    //kap
    if(pdgcode==321){
       size = kap_cv.size();
      for(int ii=0;ii<size;ii++){
	if(pz>v_pzmin[ii] && pz<v_pzmax[ii] && pt>v_ptmin[ii] && pt<v_ptmax[ii]){
	  cvmc = kap_cv[ii];
	}
      }  
    }

    //kam:
    if(pdgcode==-321){    
       size = kam_cv.size();
       for(int ii=0;ii<size;ii++){
	if(pz>v_pzmin[ii] && pz<v_pzmax[ii] && pt>v_ptmin[ii] && pt<v_ptmax[ii]){
	  cvmc = kam_cv[ii];
	}
      }  
    }

  return cvmc;
    
  }
  
  MIPPNumiMC* MIPPNumiMC::getInstance(){
    if (instance == 0) instance = new MIPPNumiMC;
    return instance;
  }
  
}
