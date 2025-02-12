
#include "ThinTargetpipCpipBins.h"
#include <set>
#include <string>
#include <iostream>
#include <sstream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace NeutrinoFluxReweight{ 
  
  ThinTargetpipCpipBins* ThinTargetpipCpipBins::instance = 0;
  
  ThinTargetpipCpipBins::ThinTargetpipCpipBins(){

  }
  
  void ThinTargetpipCpipBins::pipC_pip_from_xml(const char* filename){
    using boost::property_tree::ptree;
    ptree top;    
    read_xml(filename,top,2); 
    ptree& binspip = top.get_child("bins.ThinTarget_pipC_pip"); 
    ptree::iterator it = binspip.begin();

//int idx=0;
    double aux_pmin,aux_pmax,aux_thetamin,aux_thetamax;
    for(; it!=binspip.end(); ++it){
      std::string p_string=it->second.get<std::string>("prange");
      std::string theta_string=it->second.get<std::string>("thetarange");

      std::stringstream ss1(p_string);
      std::stringstream ss2(theta_string);
      ss1 >> aux_pmin >> aux_pmax;
      ss2 >> aux_thetamin >> aux_thetamax;

      pipC_pip_pmin.push_back(aux_pmin);
      pipC_pip_pmax.push_back(aux_pmax);
      pipC_pip_thetamin.push_back(aux_thetamin);
      pipC_pip_thetamax.push_back(aux_thetamax);

    }

 }

  void ThinTargetpipCpipBins::pipC_pim_from_xml(const char* filename){
    using boost::property_tree::ptree;
    ptree top;
    read_xml(filename,top,2);
    ptree& binspim = top.get_child("bins.ThinTarget_pipC_pim");
    ptree::iterator it = binspim.begin();

//int idx=0;
    double aux_pmin,aux_pmax,aux_thetamin,aux_thetamax;
    for(; it!=binspim.end(); ++it){
      std::string p_string=it->second.get<std::string>("prange");
      std::string theta_string=it->second.get<std::string>("thetarange");

      std::stringstream ss1(p_string);
      std::stringstream ss2(theta_string);
      ss1 >> aux_pmin >> aux_pmax;
      ss2 >> aux_thetamin >> aux_thetamax;

      pipC_pim_pmin.push_back(aux_pmin);
      pipC_pim_pmax.push_back(aux_pmax);
      pipC_pim_thetamin.push_back(aux_thetamin);
      pipC_pim_thetamax.push_back(aux_thetamax);

    }

 }

    void ThinTargetpipCpipBins::pipC_kp_from_xml(const char* filename){
    using boost::property_tree::ptree;
    ptree top;
    read_xml(filename,top,2);
    ptree& binskp = top.get_child("bins.ThinTarget_pipC_kp");
    ptree::iterator it = binskp.begin();

//int idx=0;
    double aux_pmin,aux_pmax,aux_thetamin,aux_thetamax;
    for(; it!=binskp.end(); ++it){
      std::string p_string=it->second.get<std::string>("prange");
      std::string theta_string=it->second.get<std::string>("thetarange");

      std::stringstream ss1(p_string);
      std::stringstream ss2(theta_string);
      ss1 >> aux_pmin >> aux_pmax;
      ss2 >> aux_thetamin >> aux_thetamax;

      pipC_kp_pmin.push_back(aux_pmin);
      pipC_kp_pmax.push_back(aux_pmax);
      pipC_kp_thetamin.push_back(aux_thetamin);
      pipC_kp_thetamax.push_back(aux_thetamax);

    }

 }

      void ThinTargetpipCpipBins::pipC_km_from_xml(const char* filename){
    using boost::property_tree::ptree;
    ptree top;
    read_xml(filename,top,2);
    ptree& binskm = top.get_child("bins.ThinTarget_pipC_km");
    ptree::iterator it = binskm.begin();

//int idx=0;
    double aux_pmin,aux_pmax,aux_thetamin,aux_thetamax;
    for(; it!=binskm.end(); ++it){
      std::string p_string=it->second.get<std::string>("prange");
      std::string theta_string=it->second.get<std::string>("thetarange");

      std::stringstream ss1(p_string);
      std::stringstream ss2(theta_string);
      ss1 >> aux_pmin >> aux_pmax;
      ss2 >> aux_thetamin >> aux_thetamax;

      pipC_km_pmin.push_back(aux_pmin);
      pipC_km_pmax.push_back(aux_pmax);
      pipC_km_thetamin.push_back(aux_thetamin);
      pipC_km_thetamax.push_back(aux_thetamax);

    }

 }
        void ThinTargetpipCpipBins::pipC_p_from_xml(const char* filename){
    using boost::property_tree::ptree;
    ptree top;
    read_xml(filename,top,2);
    ptree& binsp = top.get_child("bins.ThinTarget_pipC_p");
    ptree::iterator it = binsp.begin();

//int idx=0;
    double aux_pmin,aux_pmax,aux_thetamin,aux_thetamax;
    for(; it!=binsp.end(); ++it){
      std::string p_string=it->second.get<std::string>("prange");
      std::string theta_string=it->second.get<std::string>("thetarange");

      std::stringstream ss1(p_string);
      std::stringstream ss2(theta_string);
      ss1 >> aux_pmin >> aux_pmax;
      ss2 >> aux_thetamin >> aux_thetamax;

      pipC_p_pmin.push_back(aux_pmin);
      pipC_p_pmax.push_back(aux_pmax);
      pipC_p_thetamin.push_back(aux_thetamin);
      pipC_p_thetamax.push_back(aux_thetamax);

    }

 }
	  void ThinTargetpipCpipBins::pipC_k0s_from_xml(const char* filename){
    using boost::property_tree::ptree;
    ptree top;
    read_xml(filename,top,2);
    ptree& binsk0s = top.get_child("bins.ThinTarget_pipC_k0s");
    ptree::iterator it = binsk0s.begin();

//int idx=0;
    double aux_pmin,aux_pmax,aux_thetamin,aux_thetamax;
    for(; it!=binsk0s.end(); ++it){
      std::string p_string=it->second.get<std::string>("prange");
      std::string theta_string=it->second.get<std::string>("thetarange");

      std::stringstream ss1(p_string);
      std::stringstream ss2(theta_string);
      ss1 >> aux_pmin >> aux_pmax;
      ss2 >> aux_thetamin >> aux_thetamax;

      pipC_k0s_pmin.push_back(aux_pmin);
      pipC_k0s_pmax.push_back(aux_pmax);
      pipC_k0s_thetamin.push_back(aux_thetamin);
      pipC_k0s_thetamax.push_back(aux_thetamax);

    }

 }
	    void ThinTargetpipCpipBins::pipC_lam_from_xml(const char* filename){
    using boost::property_tree::ptree;
    ptree top;
    read_xml(filename,top,2);
    ptree& binslam = top.get_child("bins.ThinTarget_pipC_lam");
    ptree::iterator it = binslam.begin();

//int idx=0;
    double aux_pmin,aux_pmax,aux_thetamin,aux_thetamax;
    for(; it!=binslam.end(); ++it){
      std::string p_string=it->second.get<std::string>("prange");
      std::string theta_string=it->second.get<std::string>("thetarange");

      std::stringstream ss1(p_string);
      std::stringstream ss2(theta_string);
      ss1 >> aux_pmin >> aux_pmax;
      ss2 >> aux_thetamin >> aux_thetamax;

      pipC_lam_pmin.push_back(aux_pmin);
      pipC_lam_pmax.push_back(aux_pmax);
      pipC_lam_thetamin.push_back(aux_thetamin);
      pipC_lam_thetamax.push_back(aux_thetamax);

    }

 }
   void ThinTargetpipCpipBins::pipC_alam_from_xml(const char* filename){
    using boost::property_tree::ptree;
    ptree top;
    read_xml(filename,top,2);
    ptree& binsalam = top.get_child("bins.ThinTarget_pipC_alam");
    ptree::iterator it = binsalam.begin();

//int idx=0;
    double aux_pmin,aux_pmax,aux_thetamin,aux_thetamax;
    for(; it!=binsalam.end(); ++it){
      std::string p_string=it->second.get<std::string>("prange");
      std::string theta_string=it->second.get<std::string>("thetarange");

      std::stringstream ss1(p_string);
      std::stringstream ss2(theta_string);
      ss1 >> aux_pmin >> aux_pmax;
      ss2 >> aux_thetamin >> aux_thetamax;

      pipC_alam_pmin.push_back(aux_pmin);
      pipC_alam_pmax.push_back(aux_pmax);
      pipC_alam_thetamin.push_back(aux_thetamin);
      pipC_alam_thetamax.push_back(aux_thetamax);

    }

 }

    


  
  
   
  /////////////////////////////////////////////////////////////////
  int ThinTargetpipCpipBins::pipC_pip_BinID(double Prod_P,double Theta, int Inc_pdg, int Prod_pdg){
    
    int ibinID = -1;
    //int size = 0; // unused
    bool israngea = (Prod_pdg == 211 && Inc_pdg == 211) ;
      if (israngea){
      for(int ii=0;ii<200;++ii){
        if(Prod_P>pipC_pip_pmin[ii] && Prod_P<pipC_pip_pmax[ii] && Theta>pipC_pip_thetamin[ii] && Theta<pipC_pip_thetamax[ii]){
          ibinID = ii;
        }
        }
       }
       
return ibinID;

}

 int ThinTargetpipCpipBins::pipC_pim_BinID(double Prod_P,double Theta, int Inc_pdg, int Prod_pdg){

    int ibinID = -1;
    //int size = 0; // unused
    bool israngea = (Prod_pdg == -211 && Inc_pdg == 211) ;
      if (israngea){
      for(int ii=0;ii<200;++ii){
        if(Prod_P>pipC_pim_pmin[ii] && Prod_P<pipC_pim_pmax[ii] && Theta>pipC_pim_thetamin[ii] && Theta<pipC_pim_thetamax[ii]){
          ibinID = ii;
        }
        }
       }
      
return ibinID;

}

 int ThinTargetpipCpipBins::pipC_kp_BinID(double Prod_P,double Theta, int Inc_pdg, int Prod_pdg){

    int ibinID = -1;
    //int size = 0; // unused

  
    bool israngea = (Prod_pdg == 321 && Inc_pdg == 211) ;
      if (israngea){
      for(int ii=0;ii<35;++ii){
        if(Prod_P>pipC_kp_pmin[ii] && Prod_P<pipC_kp_pmax[ii] && Theta>pipC_kp_thetamin[ii] && Theta<pipC_kp_thetamax[ii]){
          ibinID = ii;
        }
        }
       }
      
return ibinID;

 }

 int ThinTargetpipCpipBins::pipC_km_BinID(double Prod_P,double Theta, int Inc_pdg, int Prod_pdg){

    int ibinID = -1;
    //int size = 0; // unused

   
    bool israngea = (Prod_pdg == -321 && Inc_pdg == 211) ;
      if (israngea){
      for(int ii=0;ii<35;++ii){
    if(Prod_P>pipC_km_pmin[ii] && Prod_P<pipC_km_pmax[ii] && Theta>pipC_km_thetamin[ii] && Theta<pipC_km_thetamax[ii]){
          ibinID = ii;
        }
        }
       }
      
return ibinID;

 }
    
   int ThinTargetpipCpipBins::pipC_p_BinID(double Prod_P,double Theta, int Inc_pdg, int Prod_pdg){

    int ibinID = -1;
    //int size = 0; // unused

   
    bool israngea = (Prod_pdg == 2212 && Inc_pdg == 211) ;
      if (israngea){
      for(int ii=0;ii<72;++ii){
        if(Prod_P>pipC_p_pmin[ii] && Prod_P<pipC_p_pmax[ii] && Theta>pipC_p_thetamin[ii] && Theta<pipC_p_thetamax[ii]){
          ibinID = ii;
        }
        }
       }
      
return ibinID;

 }
  
 
 int ThinTargetpipCpipBins::pipC_k0s_BinID(double Prod_P,double Theta, int Inc_pdg, int Prod_pdg){

    int ibinID = -1;
    //int size = 0; // unused

    bool israngea = (Prod_pdg == 310 && Inc_pdg == 211) ;
      if (israngea){
      for(int ii=0;ii<34;++ii){
    if(Prod_P>pipC_k0s_pmin[ii] && Prod_P<pipC_k0s_pmax[ii] && Theta>pipC_k0s_thetamin[ii] && Theta<pipC_k0s_thetamax[ii]){
          ibinID = ii;
        }
        }
       }
      
return ibinID;

 }

 int ThinTargetpipCpipBins::pipC_lam_BinID(double Prod_P,double Theta, int Inc_pdg, int Prod_pdg){

    int ibinID = -1;
    //int size = 0; // unused

  
    bool israngea = (Prod_pdg == 3122 && Inc_pdg == 211) ;
      if (israngea){
      for(int ii=0;ii<21;++ii){
        if(Prod_P>pipC_lam_pmin[ii] && Prod_P<pipC_lam_pmax[ii] && Theta>pipC_lam_thetamin[ii] && Theta<pipC_lam_thetamax[ii]){
          ibinID = ii;
        }
        }
       }
       
return ibinID;

 }

 int ThinTargetpipCpipBins::pipC_alam_BinID(double Prod_P,double Theta, int Inc_pdg, int Prod_pdg){

    int ibinID = -1;
    //int size = 0; // unused
    bool israngea = (Prod_pdg == -3122 && Inc_pdg == 211) ;
      if (israngea){
      for(int ii=0;ii<11;++ii){
        if(Prod_P>pipC_alam_pmin[ii] && Prod_P<pipC_alam_pmax[ii] && Theta>pipC_alam_thetamin[ii] && Theta<pipC_alam_thetamax[ii]){
          ibinID = ii;
        }
        }
       }
      
return ibinID;

 }

int ThinTargetpipCpipBins::GetNbins_pipC_pip(){
    if(pipC_pip_pmin.size()==0)throw std::runtime_error("ThinTargetPIPBins has not been initialized!!");
    return pipC_pip_pmin.size();
  }
 
  int ThinTargetpipCpipBins::GetNbins_pipC_pim(){
    if(pipC_pim_pmin.size()==0)throw std::runtime_error("ThinTargetPIMBins has not been initialized!!");
    return pipC_pim_pmin.size();
  }


 int ThinTargetpipCpipBins::GetNbins_pipC_kp(){
    if(pipC_kp_pmin.size()==0)throw std::runtime_error("ThinTargetKPBins has not been initialized!!");
    return pipC_kp_pmin.size();
  }

 int ThinTargetpipCpipBins::GetNbins_pipC_km(){
    if(pipC_km_pmin.size()==0)throw std::runtime_error("ThinTargetPIMBins has not been initialized!!");
    return pipC_km_pmin.size();
  }

 int ThinTargetpipCpipBins::GetNbins_pipC_p(){
    if(pipC_p_pmin.size()==0)throw std::runtime_error("ThinTargetPIMBins has not been initialized!!");
    return pipC_p_pmin.size();
  }

 int ThinTargetpipCpipBins::GetNbins_pipC_k0s(){
    if(pipC_k0s_pmin.size()==0)throw std::runtime_error("ThinTargetPIMBins has not been initialized!!");
    return pipC_k0s_pmin.size();
  }
 int ThinTargetpipCpipBins::GetNbins_pipC_lam(){
    if(pipC_lam_pmin.size()==0)throw std::runtime_error("ThinTargetPIMBins has not been initialized!!");
    return pipC_lam_pmin.size();
  }
 int ThinTargetpipCpipBins::GetNbins_pipC_alam(){
    if(pipC_alam_pmin.size()==0)throw std::runtime_error("ThinTargetPIMBins has not been initialized!!");
    return pipC_alam_pmin.size();
  }



  ThinTargetpipCpipBins* ThinTargetpipCpipBins::getInstance(){
    if (instance == 0) instance = new ThinTargetpipCpipBins;
    return instance;
  }
  
}


