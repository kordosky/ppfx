
#include <iostream>
#include <cstdlib>
#include "ThinTargetpipCpipMC.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace NeutrinoFluxReweight{ 
  
  ThinTargetpipCpipMC* ThinTargetpipCpipMC::instance = 0;
  
  ThinTargetpipCpipMC::ThinTargetpipCpipMC(){
  ranges_already_filled = false;

    //FRaction of protons not interacting in the target or Budal Monitor for
    //LE NuMI mode using FTFP.
//    proton_no_interacting = 0.13288294;
  }
  
  void ThinTargetpipCpipMC::pipC_pip_mc_from_xml(const char* filename){
    using boost::property_tree::ptree;
    ptree top;
    read_xml(filename,top,2); 
    ptree bins;
    bins = top.get_child("mcbins.pipC_pip_mc");
    ptree::iterator it;    
    // we know that pip, pim have the same binning
    double cv,pmin,pmax,thetamin,thetamax;
    
    for(it = bins.begin(); it!=bins.end(); ++it){
   
      std::string cv_string=it->second.get<std::string>("cvmc");
      std::string p_string=it->second.get<std::string>("prange");
      std::string theta_string=it->second.get<std::string>("thetarange");

      std::stringstream ss1(cv_string);
      std::stringstream ss2(p_string);
      std::stringstream ss3(theta_string);
      ss1 >> cv;
      ss2 >> pmin >> pmax;
      ss3 >> thetamin >> thetamax;
   
      pipC_pip_cv.push_back(cv);
      if(ranges_already_filled==false){
	v_pmin.push_back(pmin);
	v_pmax.push_back(pmax);
	v_thetamin.push_back(thetamin);
	v_thetamax.push_back(thetamax);
      }
      
    }
    ranges_already_filled=true;
    }

 void ThinTargetpipCpipMC::pipC_pim_mc_from_xml(const char* filename){
    using boost::property_tree::ptree;
    ptree top;
    read_xml(filename,top,2);
    ptree bins_pim;
    bins_pim = top.get_child("mcbins.pipC_pim_mc");
    ptree::iterator it;
    // we know that pip, pim have the same binning
    double cv,pmin,pmax,thetamin,thetamax;

    for(it = bins_pim.begin(); it!=bins_pim.end(); ++it){

      std::string cv_string=it->second.get<std::string>("cvmc");
      std::string p_string=it->second.get<std::string>("prange");
      std::string theta_string=it->second.get<std::string>("thetarange");

      std::stringstream ss1(cv_string);
      std::stringstream ss2(p_string);
      std::stringstream ss3(theta_string);
      ss1 >> cv;
      ss2 >> pmin >> pmax;
      ss3 >> thetamin >> thetamax;

      pipC_pim_cv.push_back(cv);
      if(ranges_already_filled==false){
        v_pmin.push_back(pmin);
        v_pmax.push_back(pmax);
        v_thetamin.push_back(thetamin);
        v_thetamax.push_back(thetamax);
      }

    }
    ranges_already_filled=true;
    }

 void ThinTargetpipCpipMC::pipC_kp_mc_from_xml(const char* filename){
    using boost::property_tree::ptree;
    ptree top;
    read_xml(filename,top,2);
    ptree bins_kp;
    bins_kp = top.get_child("mcbins.pipC_kp_mc");
    ptree::iterator it;
    // we know that pip, pim have the same binning
    double cv,pmin,pmax,thetamin,thetamax;

    for(it = bins_kp.begin(); it!=bins_kp.end(); ++it){

      std::string cv_string=it->second.get<std::string>("cvmc");
      std::string p_string=it->second.get<std::string>("prange");
      std::string theta_string=it->second.get<std::string>("thetarange");

      std::stringstream ss1(cv_string);
      std::stringstream ss2(p_string);
      std::stringstream ss3(theta_string);
      ss1 >> cv;
      ss2 >> pmin >> pmax;
      ss3 >> thetamin >> thetamax;

      pipC_kp_cv.push_back(cv);
      if(ranges_already_filled==false){
        v_pmin.push_back(pmin);
        v_pmax.push_back(pmax);
        v_thetamin.push_back(thetamin);
        v_thetamax.push_back(thetamax);
      }

    }
    ranges_already_filled=true;
    }

  void ThinTargetpipCpipMC::pipC_km_mc_from_xml(const char* filename){
    using boost::property_tree::ptree;
    ptree top;
    read_xml(filename,top,2);
    ptree bins_km;
    bins_km = top.get_child("mcbins.pipC_km_mc");
    ptree::iterator it;
    // we know that pip, pim have the same binning
    double cv,pmin,pmax,thetamin,thetamax;

    for(it = bins_km.begin(); it!=bins_km.end(); ++it){

      std::string cv_string=it->second.get<std::string>("cvmc");
      std::string p_string=it->second.get<std::string>("prange");
      std::string theta_string=it->second.get<std::string>("thetarange");

      std::stringstream ss1(cv_string);
      std::stringstream ss2(p_string);
      std::stringstream ss3(theta_string);
      ss1 >> cv;
      ss2 >> pmin >> pmax;
      ss3 >> thetamin >> thetamax;

      pipC_km_cv.push_back(cv);
      if(ranges_already_filled==false){
        v_pmin.push_back(pmin);
        v_pmax.push_back(pmax);
        v_thetamin.push_back(thetamin);
        v_thetamax.push_back(thetamax);
      }

    }
    ranges_already_filled=true;
    }



  void ThinTargetpipCpipMC::pipC_p_mc_from_xml(const char* filename){
    using boost::property_tree::ptree;
    ptree top;
    read_xml(filename,top,2);
    ptree bins_p;
    bins_p = top.get_child("mcbins.pipC_p_mc");
    ptree::iterator it;
    // we know that pip, pim have the same binning
    double cv,pmin,pmax,thetamin,thetamax;

    for(it = bins_p.begin(); it!=bins_p.end(); ++it){

      std::string cv_string=it->second.get<std::string>("cvmc");
      std::string p_string=it->second.get<std::string>("prange");
      std::string theta_string=it->second.get<std::string>("thetarange");

      std::stringstream ss1(cv_string);
      std::stringstream ss2(p_string);
      std::stringstream ss3(theta_string);
      ss1 >> cv;
      ss2 >> pmin >> pmax;
      ss3 >> thetamin >> thetamax;

      pipC_p_cv.push_back(cv);
      if(ranges_already_filled==false){
        v_pmin.push_back(pmin);
        v_pmax.push_back(pmax);
        v_thetamin.push_back(thetamin);
        v_thetamax.push_back(thetamax);
      }

    }
    ranges_already_filled=true;
    }


  void ThinTargetpipCpipMC::pipC_k0s_mc_from_xml(const char* filename){
    using boost::property_tree::ptree;
    ptree top;
    read_xml(filename,top,2);
    ptree bins_k0s;
    bins_k0s = top.get_child("mcbins.pipC_k0s_mc");
    ptree::iterator it;
    // we know that pip, pim have the same binning
    double cv,pmin,pmax,thetamin,thetamax;

    for(it = bins_k0s.begin(); it!=bins_k0s.end(); ++it){

      std::string cv_string=it->second.get<std::string>("cvmc");
      std::string p_string=it->second.get<std::string>("prange");
      std::string theta_string=it->second.get<std::string>("thetarange");

      std::stringstream ss1(cv_string);
      std::stringstream ss2(p_string);
      std::stringstream ss3(theta_string);
      ss1 >> cv;
      ss2 >> pmin >> pmax;
      ss3 >> thetamin >> thetamax;

      pipC_k0s_cv.push_back(cv);
      if(ranges_already_filled==false){
        v_pmin.push_back(pmin);
        v_pmax.push_back(pmax);
        v_thetamin.push_back(thetamin);
        v_thetamax.push_back(thetamax);
      }

    }
    ranges_already_filled=true;
    }


  void ThinTargetpipCpipMC::pipC_lam_mc_from_xml(const char* filename){
    using boost::property_tree::ptree;
    ptree top;
    read_xml(filename,top,2);
    ptree bins_lam;
    bins_lam = top.get_child("mcbins.pipC_lam_mc");
    ptree::iterator it;
    // we know that pip, pim have the same binning
    double cv,pmin,pmax,thetamin,thetamax;

    for(it = bins_lam.begin(); it!=bins_lam.end(); ++it){

      std::string cv_string=it->second.get<std::string>("cvmc");
      std::string p_string=it->second.get<std::string>("prange");
      std::string theta_string=it->second.get<std::string>("thetarange");

      std::stringstream ss1(cv_string);
      std::stringstream ss2(p_string);
      std::stringstream ss3(theta_string);
      ss1 >> cv;
      ss2 >> pmin >> pmax;
      ss3 >> thetamin >> thetamax;

      pipC_lam_cv.push_back(cv);
      if(ranges_already_filled==false){
        v_pmin.push_back(pmin);
        v_pmax.push_back(pmax);
        v_thetamin.push_back(thetamin);
        v_thetamax.push_back(thetamax);
      }

    }
    ranges_already_filled=true;
    }



  void ThinTargetpipCpipMC::pipC_alam_mc_from_xml(const char* filename){
    using boost::property_tree::ptree;
    ptree top;
    read_xml(filename,top,2);
    ptree bins_alam;
    bins_alam = top.get_child("mcbins.pipC_alam_mc");
    ptree::iterator it;
    // we know that pip, pim have the same binning
    double cv,pmin,pmax,thetamin,thetamax;

    for(it = bins_alam.begin(); it!=bins_alam.end(); ++it){

      std::string cv_string=it->second.get<std::string>("cvmc");
      std::string p_string=it->second.get<std::string>("prange");
      std::string theta_string=it->second.get<std::string>("thetarange");

      std::stringstream ss1(cv_string);
      std::stringstream ss2(p_string);
      std::stringstream ss3(theta_string);
      ss1 >> cv;
      ss2 >> pmin >> pmax;
      ss3 >> thetamin >> thetamax;

      pipC_alam_cv.push_back(cv);
      if(ranges_already_filled==false){
        v_pmin.push_back(pmin);
        v_pmax.push_back(pmax);
        v_thetamin.push_back(thetamin);
        v_thetamax.push_back(thetamax);
      }

    }
    ranges_already_filled=true;
    }
 
 
 
 double ThinTargetpipCpipMC::getMCval(double Prod_P,double Theta, int Inc_pdg, int Prod_pdg){
 
    double cvmc = -1;
    if( Inc_pdg!=211 ) return cvmc;
    int size = 0;
   
    //pip:
    if(Prod_pdg==211){
      size = pipC_pip_cv.size();
      for(int ii=0;ii<size;++ii){
	if(Prod_P>v_pmin[ii] && Prod_P<v_pmax[ii] && Theta>v_thetamin[ii] && Theta<v_thetamax[ii]){
	  cvmc = pipC_pip_cv[ii];
	}
      }      
    }

 if(Prod_pdg==-211){
      size = pipC_pim_cv.size();
      for(int ii=0;ii<size;++ii){
        if(Prod_P>v_pmin[ii] && Prod_P<v_pmax[ii] && Theta>v_thetamin[ii] && Theta<v_thetamax[ii]){
          cvmc = pipC_pim_cv[ii];
        }
      }
    }

 if(Prod_pdg==321){
      size = pipC_kp_cv.size();
      for(int ii=0;ii<size;++ii){
        if(Prod_P>v_pmin[ii] && Prod_P<v_pmax[ii] && Theta>v_thetamin[ii] && Theta<v_thetamax[ii]){
          cvmc = pipC_kp_cv[ii];
        }
      }
    }

  if(Prod_pdg==-321){
      size = pipC_km_cv.size();
      for(int ii=0;ii<size;++ii){
        if(Prod_P>v_pmin[ii] && Prod_P<v_pmax[ii] && Theta>v_thetamin[ii] && Theta<v_thetamax[ii]){
          cvmc = pipC_km_cv[ii];
        }
      }
    }

   if(Prod_pdg==2212){
      size = pipC_p_cv.size();
      for(int ii=0;ii<size;++ii){
        if(Prod_P>v_pmin[ii] && Prod_P<v_pmax[ii] && Theta>v_thetamin[ii] && Theta<v_thetamax[ii]){
          cvmc = pipC_p_cv[ii];
        }
      }
    }


    if(Prod_pdg==310){
      size = pipC_k0s_cv.size();
      for(int ii=0;ii<size;++ii){
        if(Prod_P>v_pmin[ii] && Prod_P<v_pmax[ii] && Theta>v_thetamin[ii] && Theta<v_thetamax[ii]){
          cvmc = pipC_k0s_cv[ii];
        }
      }
    }

     if(Prod_pdg==3122){
      size = pipC_lam_cv.size();
      for(int ii=0;ii<size;++ii){
        if(Prod_P>v_pmin[ii] && Prod_P<v_pmax[ii] && Theta>v_thetamin[ii] && Theta<v_thetamax[ii]){
          cvmc = pipC_lam_cv[ii];
        }
      }
    }
      if(Prod_pdg==-3122){
      size = pipC_alam_cv.size();
      for(int ii=0;ii<size;++ii){
        if(Prod_P>v_pmin[ii] && Prod_P<v_pmax[ii] && Theta>v_thetamin[ii] && Theta<v_thetamax[ii]){
          cvmc = pipC_alam_cv[ii];
        }
      }
    }

    
   
   return cvmc;
    
  }
  
  ThinTargetpipCpipMC* ThinTargetpipCpipMC::getInstance(){
    if (instance == 0) instance = new ThinTargetpipCpipMC;
    return instance;
  }
  
}
