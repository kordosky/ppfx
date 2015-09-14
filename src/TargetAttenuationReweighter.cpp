
#include "TargetAttenuationReweighter.h"
#include <iostream>
#include <locale>
#include <cstdlib>
#include <stdexcept>
#include "AttenuationMC.h"
#include "MIPPNumiYieldsBins.h"

namespace NeutrinoFluxReweight{
  
  TargetAttenuationReweighter::TargetAttenuationReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars)
    :iUniv(iuniv),cvPars(cv_pars),univPars(univ_pars){
    
     std::map<std::string, double> this_table = univPars.table;
     prod_prtC_xsec = this_table["prod_prtC_xsec"];
     qe_prtC_xsec   = this_table["qe_prtC_xsec"];
     
  }
  TargetAttenuationReweighter::~TargetAttenuationReweighter(){
    
  }
  std::vector<bool> TargetAttenuationReweighter::canReweight(const InteractionChainData& aa){
    std::vector<bool> can_rws;
    std::vector<InteractionData> vec_inter = aa.interaction_chain;
    int ninter = vec_inter.size();

    //Looking if there is a proton-Target interaction:
    for(int ii=0;ii<ninter;ii++){
  
      //first interaction:
      if(ii==0){
	bool is_tgt_int = vec_inter[0].Vol == "BudalMonitor" || vec_inter[0].Vol == "TGT1" || vec_inter[0].Vol == "Budal_HFVS"  || vec_inter[0].Vol == "Budal_VFHS";
	if(is_tgt_int)can_rws.push_back(true);
	else if(vec_inter[0].Inc_pdg == 2212)can_rws.push_back(true);
	else can_rws.push_back(false);
      }
      //Absorption in the target:
      else{
	bool starts_tgt = vec_inter[ii-1].Vol == "BudalMonitor" || vec_inter[ii-1].Vol == "TGT1" || vec_inter[ii-1].Vol == "Budal_HFVS"  || vec_inter[ii-1].Vol == "Budal_VFHS";
	bool ends_tgt   = vec_inter[ii].Vol   == "BudalMonitor" || vec_inter[ii].Vol   == "TGT1" || vec_inter[ii-1].Vol == "Budal_HFVS"  || vec_inter[ii-1].Vol == "Budal_VFHS";
	if(starts_tgt && ends_tgt){
	  can_rws.push_back(true);
	}
	else{
	  can_rws.push_back(false);
	}
      }
    }  

    return can_rws;
  }
  double TargetAttenuationReweighter::calculateWeight(const InteractionChainData& aa){

    double wgt =1.0;

    //Some constants:
    const double graphite_density=1.78;// g/cc
    const double avog_x_mb2cm2 = 6.02214e-4; //useful constant
    const double graphite_A    = 12.01; // g/mole    

    std::vector<InteractionData> vec_inter = aa.interaction_chain;

    //Finding the Data and MC total cross sections:    
    AttenuationMC* dtH = AttenuationMC::getInstance();
    MIPPNumiYieldsBins*  MIPPbins =  MIPPNumiYieldsBins::getInstance();
   
    bool there_is_MIPP  = false;
    bool it_is_survival = false;
    
    //MIPP:
    TargetData tar = aa.tar_info;
    int binID = MIPPbins->BinID(tar.Pz,tar.Pt,tar.Tar_pdg);   
    bool is_le = isLE(aa.target_config);
    bool is_me = isLE(aa.target_config);
    if( !is_le &&  !is_me ){
      throw std::runtime_error("cannot determine if it's LE or ME beam");
    }

    TH1D* hzpos;
    if(binID>=0){
       if(tar.Tar_pdg == 211 || tar.Tar_pdg ==- 211){
	there_is_MIPP = true;
	if(tar.Tar_pdg == 211 && is_le)hzpos = dtH->hzpostgt_pip_le[binID];
	if(tar.Tar_pdg == 211 && is_me)hzpos = dtH->hzpostgt_pip_me[binID];
	if(tar.Tar_pdg ==-211 && is_le)hzpos = dtH->hzpostgt_pim_le[binID];
	if(tar.Tar_pdg ==-211 && is_me)hzpos = dtH->hzpostgt_pim_me[binID];
       }
       else if(tar.Tar_pdg == 321){
	 int aux_binID = MIPPbins->BinID(tar.Pz,tar.Pt,211);
	 if(aux_binID>=0){
	   there_is_MIPP = true;
	   //I substracted 78 because we store all histos in AttenuationMC but the kaon histograms make 
	   // sense after P>20 GeV/c and we are using bin ID convention here.
	   if(is_le)hzpos = dtH->hzpostgt_kap_le[aux_binID-78];
	   if(is_me)hzpos = dtH->hzpostgt_kap_me[aux_binID-78];
	 }
       }
       else if(tar.Tar_pdg == -321){
	 int aux_binID = MIPPbins->BinID(tar.Pz,tar.Pt,-211);
	 if(aux_binID>=0){
	   there_is_MIPP = true;
	   if(is_le)hzpos = dtH->hzpostgt_kam_le[aux_binID-78];
	   if(is_me)hzpos = dtH->hzpostgt_kam_me[aux_binID-78];
	 }
       }
       else if(tar.Tar_pdg == 130 || tar.Tar_pdg == 310){
	 int aux_binID = MIPPbins->BinID(tar.Pz,tar.Pt,211);
	 if(aux_binID>=0){
	   there_is_MIPP = true;
	   if(is_le)hzpos = dtH->hzpostgt_kap_le[aux_binID-78];
	   if(is_me)hzpos = dtH->hzpostgt_kap_me[aux_binID-78];
	 }
       }
       else{
	 std::cout<<"=> There is an in MIPPNumiYieldsBins"<<std::endl;
       }
    }
    
    //Survival:
    if(!there_is_MIPP && vec_inter[0].Vol!="BudalMonitor" && vec_inter[0].Vol!="TGT1" && vec_inter[0].Vol!="Budal_HFVS" && vec_inter[0].Vol!="Budal_VFHS"){
      it_is_survival = true;
    }
    
    double delta_sigma = 0.0; // sigma_data - sigma_mc
    double ratio_sigma = 0.0; // sigma_data / sigma_mc

    delta_sigma  = prod_prtC_xsec;
    delta_sigma  += qe_prtC_xsec; 
    
    int binprtC = (dtH->hXS_prtC)->FindBin(vec_inter[0].Inc_P);
    double mcval = (dtH->hXS_prtC)->GetBinContent(binprtC);
    if(mcval<1.e-12){
      throw std::runtime_error("MC Cross section is zero... check!");
    }
    ratio_sigma = delta_sigma / mcval;
    delta_sigma -= mcval;
    
    //Finding the target penetration for the primary proton beam:
   
    double startZ        = targetStartZ(aa.target_config) + shiftPlaylist(aa.playlist);
    double endZ = (vec_inter[0].Vtx)[2];
    double totmatZ = 0.;  //this will be the amount of material passed. 

    if( is_le){
      //check of initial z position:
      if(vec_inter[0].Vol=="BudalMonitor"){
	if(endZ<startZ || endZ>(startZ+2.0))std::cout<<"Potential error of BudalMonitor=> startZ, endZ: "<<startZ<<" "<<endZ<<std::endl;   
      }
      totmatZ = getTargetPenetrationLE(startZ,endZ,startZ);
    }
    else if( is_me ){ 
      //check of initial z position:
      if(vec_inter[0].Vol=="Budal_HFVS"){
	if(endZ<startZ || endZ>(startZ+2.4))std::cout<<"Potential error of Budal_HFVS => startZ, endZ: "<<startZ<<" "<<endZ<<std::endl;   
      }
      totmatZ = getTargetPenetrationME(startZ,endZ,startZ);
    }   
    
    totmatZ *= avog_x_mb2cm2;
    totmatZ /= graphite_A;
    totmatZ *= graphite_density;

    //Calculating the weight:
    if(there_is_MIPP){
      int nbins = hzpos->GetXaxis()->GetNbins();
      double integral_mc = 0;
      double integral_wt = 0;
      for(int ib=1;ib<=nbins;ib++){
	double cont = hzpos->GetBinContent(ib);
	double crr  = hzpos->GetXaxis()->GetBinCenter(ib);
	integral_mc += cont;	
	integral_wt += cont*ratio_sigma*exp(-1.0*crr*avog_x_mb2cm2*graphite_density*delta_sigma/graphite_A);
      }
      if(integral_wt<=0 || integral_mc<=0){
	throw std::runtime_error("yield is zero or negative... check!");
      }     
      double norm = integral_mc/integral_wt;
      wgt = norm*ratio_sigma*exp(-1.0*totmatZ*delta_sigma);
    }
    else if(it_is_survival){
      wgt = exp(-1.0*totmatZ*delta_sigma);
    }
    else{
      wgt = ratio_sigma*exp(-1.0*totmatZ*delta_sigma);
    }

    return wgt;
    
  }

  double TargetAttenuationReweighter::targetStartZ(const std::string& tgtcfg){
    //check this (Leo)
    double z0=-51.1; // position of the upstream edge of the budal monitor in 000z config
    // check to see if we are in LE config and adjust accordingly
    if( isLE(tgtcfg) ) {
      z0=-51.72; // determined by ntuple tomography
      // check to see if we are in ME config and adjust accordingly
    }else if( isME(tgtcfg) ){ 
      z0=-143.3; // determined by ntuple tomography
      // compared to LE target: recall, LE target is stuffed into the horn.
    }
    else{
      throw std::runtime_error("cannot determine if it's LE or ME beam");
    }
    // pull out all the numbers, recording them into a string
    std::string just_nums;
    for (int i=0; i<tgtcfg.length(); ++i) {
      if(isdigit(tgtcfg[i])) just_nums.push_back(tgtcfg[i]);
      if(tgtcfg[i]=='z') break;
    }
    // now convert the string to a number
    double dz=atof(just_nums.c_str());
    //
    return z0-dz;
  }
  
  bool TargetAttenuationReweighter::isLE(const std::string& tgtcfg){
    return tgtcfg.find("LE")||tgtcfg.find("le")||tgtcfg.find("Le");
  }


  bool TargetAttenuationReweighter::isME(const std::string& tgtcfg){
    return tgtcfg.find("ME")||tgtcfg.find("me")||tgtcfg.find("Me");
  }

  double TargetAttenuationReweighter::shiftPlaylist(const int ipl){
    //Minerva definies dk2nu.vint[1] as the playlist ID.
    //If this variable is not filled, then the value is filled as -1 in InteractionChainData
    //If the experiment definied dk2nu.vint[1], this function needs to be remade.
    double shift_for_playlist = 0;    
    if( ipl == -1 ){
      shift_for_playlist = 0.;
    }
    else if( ipl == 0 || ipl == 1 ){
      shift_for_playlist = 0.50;
    }
    else if( ipl == 7 || ipl == 10 || ipl == 6 ){
      shift_for_playlist = 0.82;
    }
    else if( ipl == 9 ){
      shift_for_playlist = -0.40;
    }
    else if( ipl == 13){
      shift_for_playlist =  0.83;
    }
    else if( ipl == 5 ){
      shift_for_playlist =  1.15;
    }
    else if( ipl == 2 || ipl == 3 ){
      shift_for_playlist = 0.43;
    }
    else if( ipl == 11 || ipl == 12 ){
      shift_for_playlist = 0.83;
    }
    else if( ipl == 4 ){
      shift_for_playlist = 0.43;
    }
    else if( ipl == 8 ){
      shift_for_playlist = - 0.09;
    }
    
    return shift_for_playlist;
    
  }
  
  double TargetAttenuationReweighter::getTargetPenetrationLE(double z_start, double z_end, double z0_budal)
  {
    /*!
     * Returns the amount of target material penetrated by a particle starting at
     * z_start and interacting or exiting the target at z_end.
     * the upstream edge of the budal monitor must be supplied.
     * All inputs must be in units of cm.
     */

    // fin edges determined by printing their position from inside g4numi. 
    // coordinate system will need to be translated such that the upstream
    // edge of the budal monitor is at the value returned by target
    // start z.
    // coordinate here in cm
    const int nfins=48;
    const double us_edges[nfins]={25.4484,   42.1683,   44.1983,   46.2283, 48.2583,
				  50.2883,   52.3183,   54.3483,   56.3783,
				  58.4083,   60.4383,   62.4683,   64.4983,
				  66.5283,   68.5583,   70.5883,   72.6183,
				  74.6483,   76.6783,   78.7083,   80.7383,
				  82.7683,   84.7983,   86.8283,   88.8583,
				  90.8883,   92.9183,   94.9483,   96.9783,
				  99.0083,  101.0383,  103.0683,  105.0983,
				  107.1283, 109.1583,  111.1883,  113.2183,
				  115.2483, 117.2783,  119.3083,  121.3383,
				  123.3683, 125.3983,  127.4283,  129.4583,
				  131.4883, 133.5183,  135.5483};
    
    const double budal_us_edge=us_edges[0]; // fin[0] is the budal
    const double fin_width=2.0;

    // budal_us_edge + z_trans = z0_budal
    // z_trans = z0_budal - budal_us_edge
    const double z_trans=z0_budal - budal_us_edge;
    
    // translated upstream edge of budal
    //    const double z_up=budal_us_edge+z_trans;
    
    // now, count the amount of material between z_up and z_start
    // for primary protons this will be zero, but the routine
    // should be written generally enough to handle the case
    // in which a trajectory starts in the target
    // we will end up subtracting this from the material between
    // z_up and z_end to get the material traversed.

    double mat_start=0.0;
    for(int ifin=0; ifin<nfins; ifin++){
      const double fin_us_edge=us_edges[ifin]+z_trans;
      const double fin_ds_edge=fin_us_edge+fin_width;
      if(z_start<=fin_us_edge) break; // no more material to add up
      else if(z_start<fin_ds_edge){ // z_start in this fin
	mat_start+=(z_start-fin_us_edge);
	break;
      }
      else{ // z_start after this fin (z_start>fin_ds_edge)
	mat_start+=fin_width;
      }
    }
    
    // now do the same thing for z_end
    double mat_end=0.0;
    for(int ifin=0; ifin<nfins; ifin++){
      const double fin_us_edge=us_edges[ifin]+z_trans;
      const double fin_ds_edge=fin_us_edge+fin_width;
      if(z_end<=fin_us_edge) break; // no more material to add up
      else if(z_end<fin_ds_edge){ // z_end in this fin
	mat_end+=(z_end-fin_us_edge);
	break;
      }
      else{ // z_end after this fin (z_end>fin_ds_edge)
	mat_end+=fin_width;
      }
    }
    double mat_traversed=mat_end-mat_start;
    return mat_traversed;
    
  }



  double TargetAttenuationReweighter::getTargetPenetrationME(double z_start, double z_end, double z0_budal)
  {  

        /*!
     * Returns the amount of target material penetrated by a particle starting at
     * z_start and interacting or exiting the target at z_end.
     * the upstream edge of the budal monitor must be supplied.
     * All inputs must be in units of cm.
     */

    // fin edges determined by printing their position from inside g4numi. 
    // coordinate system will need to be translated such that the upstream
    // edge of the budal monitor is at the value returned by target
    // start z.
    // coordinate here in cm
    const int nfins=50;
    const double us_edges[nfins]={ 19.285, 22.185, 25.035, 27.485, 
				   29.935, 32.385, 34.835, 37.285, 
				   39.735, 42.185, 44.635, 47.085,
				   49.535, 51.985, 54.435, 56.885,
				   59.335, 61.785, 64.235, 66.685,
				   69.135, 71.585, 74.035, 76.485,
				   78.935, 81.385, 83.835, 86.285,
				   88.735, 91.185, 93.635, 96.085,
				   98.535, 100.985, 103.435, 105.885,
				   108.335, 110.785, 113.235, 115.685,
				   118.135, 120.585, 123.035, 125.485,
				   127.935, 130.385, 132.835, 135.285,
				   137.735, 140.185};
    
    const double ds_edges[nfins]={21.685, 24.585, 27.435, 29.885,
				  32.335, 34.785, 37.235, 39.685,
				  42.135, 44.585, 47.035, 49.485,
				  51.935, 54.385, 56.835, 59.285,
				  61.735, 64.185, 66.635, 69.085,
				  71.535, 73.985, 76.435, 78.885,
				  81.335, 83.785, 86.235, 88.685,
				  91.135, 93.585, 96.035, 98.485,
				  100.935, 103.385, 105.835, 108.285,
				  110.735, 113.185, 115.635, 118.085,
				  120.535, 122.985, 125.435, 127.885,
				  130.335, 132.785, 135.235, 137.685,
				  140.135, 142.585};
    
    // fin[0] is the first budal (HFVS) (horizontal fin, vertical scan)
    // fin[1] is the second budal (VFHS)
    const double budal_us_edge=us_edges[0]; 

    // budal_us_edge + z_trans = z0_budal
    // z_trans = z0_budal - budal_us_edge
    const double z_trans=z0_budal - budal_us_edge;
    
    // translated upstream edge of budal
    //    const double z_up=budal_us_edge+z_trans;
    
    // now, count the amount of material between z_up and z_start
    // for primary protons this will be zero, but the routine
    // should be written generally enough to handle the case
    // in which a trajectory starts in the target
    // we will end up subtracting this from the material between
    // z_up and z_end to get the material traversed.
    double mat_start=0.0;
    for(int ifin=0; ifin<nfins; ifin++){
      const double fin_us_edge=us_edges[ifin]+z_trans;
      const double fin_ds_edge=ds_edges[ifin]+z_trans;
      if(z_start<=fin_us_edge) break; // no more material to add up
      else if(z_start<fin_ds_edge){ // z_start in this fin
	mat_start+=(z_start-fin_us_edge);
	break;
      }
      else{ // z_start after this fin (z_start>fin_ds_edge)
	mat_start+=(fin_ds_edge-fin_us_edge);
      }
    }
    
    // now do the same thing for z_end
    double mat_end=0.0;
    for(int ifin=0; ifin<nfins; ifin++){
      const double fin_us_edge=us_edges[ifin]+z_trans;
      const double fin_ds_edge=ds_edges[ifin]+z_trans;
      if(z_end<=fin_us_edge) break; // no more material to add up
      else if(z_end<fin_ds_edge){ // z_end in this fin
	mat_end+=(z_end-fin_us_edge);
	break;
      }
      else{ // z_end after this fin (z_end>fin_ds_edge)
	mat_end+=(fin_ds_edge-fin_us_edge);
      }
    }
    double mat_traversed=mat_end-mat_start;
    return mat_traversed;

  }
  
}
