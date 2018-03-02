
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

    //int idx=0;
    double aux_xfmin,aux_xfmax,aux_ptmin,aux_ptmax;
    for(; it!=binsPI.end(); ++it){
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

  void ThinTargetBins::barton_pC_pi_from_xml(const char* filename){
    using boost::property_tree::ptree;
    ptree top;    
    read_xml(filename,top,2); 
    ptree& binsPI = top.get_child("bins.ThinTargetBarton_pC_pi"); 
    ptree::iterator it = binsPI.begin();

    //int idx=0;
    double aux_xfmin,aux_xfmax,aux_ptmin,aux_ptmax;
    for(; it!=binsPI.end(); ++it){
      std::string xf_string=it->second.get<std::string>("xfrange");
      std::string pt_string=it->second.get<std::string>("ptrange");
      
      std::stringstream ss1(xf_string);
      std::stringstream ss2(pt_string);
      ss1 >> aux_xfmin >> aux_xfmax;
      ss2 >> aux_ptmin >> aux_ptmax;
   
      b_pC_pi_xfmin.push_back(aux_xfmin);
      b_pC_pi_xfmax.push_back(aux_xfmax);
      b_pC_pi_ptmin.push_back(aux_ptmin);
      b_pC_pi_ptmax.push_back(aux_ptmax);
      
    }
    
  }

  void ThinTargetBins::pC_p_from_xml(const char* filename){
    using boost::property_tree::ptree;
    ptree top;    
    read_xml(filename,top,2); 
    ptree& binsP = top.get_child("bins.ThinTarget_pC_p"); 
    ptree::iterator it = binsP.begin();

    //int idx=0;
    double aux_xfmin,aux_xfmax,aux_ptmin,aux_ptmax;
    for(; it!=binsP.end(); ++it){
      std::string xf_string=it->second.get<std::string>("xfrange");
      std::string pt_string=it->second.get<std::string>("ptrange");
      
      std::stringstream ss1(xf_string);
      std::stringstream ss2(pt_string);
      ss1 >> aux_xfmin >> aux_xfmax;
      ss2 >> aux_ptmin >> aux_ptmax;
   
      pC_p_xfmin.push_back(aux_xfmin);
      pC_p_xfmax.push_back(aux_xfmax);
      pC_p_ptmin.push_back(aux_ptmin);
      pC_p_ptmax.push_back(aux_ptmax);
      
    }
    
  }

void ThinTargetBins::pC_n_from_xml(const char* filename){
    using boost::property_tree::ptree;
    ptree top;    
    read_xml(filename,top,2); 
    ptree& binsN = top.get_child("bins.ThinTarget_pC_n"); 
    ptree::iterator it = binsN.begin();

    //int idx=0;
    double aux_xfmin,aux_xfmax;
    for(; it!=binsN.end(); ++it){
      std::string xf_string=it->second.get<std::string>("xfrange");
      std::stringstream ss1(xf_string);
      ss1 >> aux_xfmin >> aux_xfmax;
         
      pC_n_xfmin.push_back(aux_xfmin);
      pC_n_xfmax.push_back(aux_xfmax);
      
    }
    
  }

 void ThinTargetBins::pC_k_from_xml(const char* filename){
    using boost::property_tree::ptree;
    ptree top;    
    read_xml(filename,top,2); 
    ptree& binsK = top.get_child("bins.ThinTargetLowxF_pC_k"); 
    ptree::iterator it = binsK.begin();
    
    //int idx=0;
    double aux_xfmin,aux_xfmax,aux_ptmin,aux_ptmax;
    for(; it!=binsK.end(); ++it){
      std::string xf_string=it->second.get<std::string>("xfrange");
      std::string pt_string=it->second.get<std::string>("ptrange");
      
      std::stringstream ss1(xf_string);
      std::stringstream ss2(pt_string);
      ss1 >> aux_xfmin >> aux_xfmax;
      ss2 >> aux_ptmin >> aux_ptmax;
   
      pC_k_xfmin.push_back(aux_xfmin);
      pC_k_xfmax.push_back(aux_xfmax);
      pC_k_ptmin.push_back(aux_ptmin);
      pC_k_ptmax.push_back(aux_ptmax);
      
    }
    
  }

  void ThinTargetBins::mipp_pC_k_pi_from_xml(const char* filename){
    using boost::property_tree::ptree;
    ptree top;    
    read_xml(filename,top,2); 
    ptree& binsK = top.get_child("bins.ThinTarget_k_pi"); 
    ptree::iterator it = binsK.begin();

    //int idx=0;
    double aux_pzmin,aux_pzmax,aux_ptmin,aux_ptmax;
    for(; it!=binsK.end(); ++it){
      std::string pz_string=it->second.get<std::string>("pzrange");
      std::string pt_string=it->second.get<std::string>("ptrange");
      
      std::stringstream ss1(pz_string);
      std::stringstream ss2(pt_string);
      ss1 >> aux_pzmin >> aux_pzmax;
      ss2 >> aux_ptmin >> aux_ptmax;
   
      mipp_pC_k_pzmin.push_back(aux_pzmin);
      mipp_pC_k_pzmax.push_back(aux_pzmax);
      mipp_pC_k_ptmin.push_back(aux_ptmin);
      mipp_pC_k_ptmax.push_back(aux_ptmax);
      
    }
    
}

 void ThinTargetBins::meson_incident_from_xml(const char* filename){
    using boost::property_tree::ptree;
    ptree top;    
    read_xml(filename,top,2); 
    ptree& bins = top.get_child("bins.ThinTarget_MesonIncident"); 
    ptree::iterator it = bins.begin();
    
    //int idx=0;
    double aux_xfmin,aux_xfmax,aux_ptmin,aux_ptmax;
    for(; it!=bins.end(); ++it){
      std::string xf_string=it->second.get<std::string>("xfrange");
      std::string pt_string=it->second.get<std::string>("ptrange");
      
      std::stringstream ss1(xf_string);
      std::stringstream ss2(pt_string);
      ss1 >> aux_xfmin >> aux_xfmax;
      ss2 >> aux_ptmin >> aux_ptmax;
      
      meson_inc_xfmin.push_back(aux_xfmin);
      meson_inc_xfmax.push_back(aux_xfmax);
      meson_inc_ptmin.push_back(aux_ptmin);
      meson_inc_ptmax.push_back(aux_ptmax);
      
    }
    
 }
  
 void ThinTargetBins::material_scaling_from_xml(const char* filename){
    using boost::property_tree::ptree;
    ptree top;    
    read_xml(filename,top,2); 
    ptree& bins = top.get_child("bins.ThinTarget_material_scaling"); 
    ptree::iterator it = bins.begin();
    
    //int idx=0;
    double aux_xfmin,aux_xfmax,aux_ptmin,aux_ptmax;
    for(; it!=bins.end(); ++it){
      std::string xf_string=it->second.get<std::string>("xfrange");
      std::string pt_string=it->second.get<std::string>("ptrange");
      
      std::stringstream ss1(xf_string);
      std::stringstream ss2(pt_string);
      ss1 >> aux_xfmin >> aux_xfmax;
      ss2 >> aux_ptmin >> aux_ptmax;
      
      mat_scal_xfmin.push_back(aux_xfmin);
      mat_scal_xfmax.push_back(aux_xfmax);
      mat_scal_ptmin.push_back(aux_ptmin);
      mat_scal_ptmax.push_back(aux_ptmax);
      
    }
    
 }

  /////////////////////////////////////////////////////////////////
  int ThinTargetBins::BinID_pC_pi(double xf, double pt,int pdgcode){
    
    int ibinID = -1;
    int size = 0;
    
    if(pdgcode == 211 || pdgcode == -211){
      size = pC_pi_xfmin.size();
      for(int ii=0;ii<size;++ii){
	if(xf>pC_pi_xfmin[ii] && xf<pC_pi_xfmax[ii] && pt>pC_pi_ptmin[ii] && pt<pC_pi_ptmax[ii]){
	  ibinID = ii;
	}
      }
    }
    
    return ibinID;
    
  }
  
  int ThinTargetBins::barton_BinID_pC_pi(double xf, double pt,int pdgcode){
    
    int ibinID = -1;
    int size = 0;
    
    if(pdgcode == 211 || pdgcode == -211){
      size = b_pC_pi_xfmin.size();
      for(int ii=0;ii<size;ii++){
	if(xf>b_pC_pi_xfmin[ii] && xf<b_pC_pi_xfmax[ii] && pt>b_pC_pi_ptmin[ii] && pt<b_pC_pi_ptmax[ii]){
	  ibinID = ii;
	}
      }
    }
    
    return ibinID;
    
  }

  int ThinTargetBins::BinID_pC_p(double xf, double pt,int pdgcode){
    
    int ibinID = -1;
    int size = 0;
    
    if(pdgcode == 2212){
      size = pC_p_xfmin.size();
      for(int ii=0;ii<size;++ii){
	if(xf>pC_p_xfmin[ii] && xf<pC_p_xfmax[ii] && pt>pC_p_ptmin[ii] && pt<pC_p_ptmax[ii]){
	  ibinID = ii;
	}
      }
    }
    
    return ibinID;
    
  }

  int ThinTargetBins::BinID_pC_n(double xf, int pdgcode){
    
    int ibinID = -1;
    int size = 0;
    if(pdgcode == 2112){
      size = pC_n_xfmin.size();
      for(int ii=0;ii<size;++ii){
	if( (xf>pC_n_xfmin[ii]) && (xf<pC_n_xfmax[ii])){
	  ibinID = ii;
	}
      }
    }	  
    
    return ibinID;
	  
  }
      
  int ThinTargetBins::BinID_pC_k(double xf, double pt,int pdgcode){
    
    int ibinID = -1;
    int size = 0;
    
    if(pdgcode == 321 || pdgcode == -321 || pdgcode == 130 || pdgcode == 310){
      size = pC_k_xfmin.size();
      for(int ii=0;ii<size;++ii){
	if(xf>pC_k_xfmin[ii] && xf<pC_k_xfmax[ii] && pt>pC_k_ptmin[ii] && pt<pC_k_ptmax[ii]){
	  ibinID = ii;
	}
      }
    }
    
    return ibinID;
    
  }
  
 int ThinTargetBins::mipp_BinID_pC_k(double pz, double pt,int pdgcode){
    
    int ibinID = -1;
    int size = 0;
    
    if(pdgcode == 321 || pdgcode == -321 || pdgcode == 130 || pdgcode == 310){
      size = mipp_pC_k_pzmin.size();
      for(int ii=0;ii<size;++ii){
	if(pz>mipp_pC_k_pzmin[ii] && pz<mipp_pC_k_pzmax[ii] && pt>mipp_pC_k_ptmin[ii] && pt<mipp_pC_k_ptmax[ii]){
	  ibinID = ii;
	}
      }
    }
    
    return ibinID;
    
  }

int ThinTargetBins::meson_inc_BinID(double xf, double pt,int pdgcode){
    
    int ibinID = -1;
    int size = 0;
    
    if(pdgcode == 211 || pdgcode == -211 || pdgcode == 321 || pdgcode == -321||  pdgcode == 130 ||  pdgcode == 310){
      size = meson_inc_xfmin.size();
      for(int ii=0;ii<size;++ii){
	if(xf>meson_inc_xfmin[ii] && xf<meson_inc_xfmax[ii] && pt>meson_inc_ptmin[ii] && pt<meson_inc_ptmax[ii]){
	  ibinID = ii;
	}
      }
    }
    
    return ibinID;
    
  }

int ThinTargetBins::material_scaling_BinID(double xf, double pt,int pdgcode){
    
    int ibinID = -1;
    int size = 0;
    
    if(pdgcode == 211 || pdgcode == -211 || pdgcode == 321 || pdgcode == -321||  pdgcode == 130 ||  pdgcode == 310){
      size = mat_scal_xfmin.size();
      for(int ii=0;ii<size;++ii){
	if((xf>mat_scal_xfmin[ii]) && (xf<mat_scal_xfmax[ii]) && (pt>mat_scal_ptmin[ii]) && (pt<mat_scal_ptmax[ii])){
	  ibinID = ii;
	}
      }
    }
    
    return ibinID;
    
  }
  
  int ThinTargetBins::GetNbins_pC_piX_NA49(){
    if(pC_pi_xfmin.size()==0)throw std::runtime_error("ThinTargetBins has not been initialized!!");
    return pC_pi_xfmin.size();
  }
  int ThinTargetBins::GetNbins_pC_piX_Barton(){
    if(b_pC_pi_xfmin.size()==0)throw std::runtime_error("ThinTargetBins has not been initialized!!");
    return b_pC_pi_xfmin.size();
  }
  int ThinTargetBins::GetNbins_pC_pX_NA49(){
    if(pC_p_xfmin.size()==0)throw std::runtime_error("ThinTargetBins has not been initialized!!");
    return pC_p_xfmin.size();
  }
  int ThinTargetBins::GetNbins_pC_nX_NA49(){
    if(pC_n_xfmin.size()==0)throw std::runtime_error("ThinTargetBins has not been initialized!!");
    return pC_n_xfmin.size();
  }
  int ThinTargetBins::GetNbins_pC_KX_NA49(){
    if(pC_k_xfmin.size()==0)throw std::runtime_error("ThinTargetBins has not been initialized!!");
    return pC_k_xfmin.size();
  }
  int ThinTargetBins::GetNbins_pC_KX_MIPP(){
    if(mipp_pC_k_pzmin.size()==0)throw std::runtime_error("ThinTargetBins has not been initialized!!");
    return mipp_pC_k_pzmin.size();
  }
  int ThinTargetBins::GetNbins_meson_incident(){
    if(meson_inc_xfmin.size()==0)throw std::runtime_error("ThinTargetBins has not been initialized!!");
    return meson_inc_xfmin.size();
  }
  int ThinTargetBins::GetNbins_material_scaling(){
    if(mat_scal_xfmin.size()==0)throw std::runtime_error("ThinTargetBins has not been initialized!!");
    return mat_scal_xfmin.size();
  }

  ThinTargetBins* ThinTargetBins::getInstance(){
    if (instance == 0) instance = new ThinTargetBins;
    return instance;
  }
  
}
