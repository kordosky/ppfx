#include "MakeReweight.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "PDGParticleCodes.h"

namespace NeutrinoFluxReweight{

  MakeReweight* MakeReweight::instance = 0;
  MakeReweight::MakeReweight(){
  }
 void MakeReweight::SetOptions(std::string fileIn){
    fileOptions = fileIn;
    ParseOptions();
    Initialize();
  }

  void MakeReweight::ParseOptions(){
    //Parsing the file input:
    using boost::property_tree::ptree;
    ptree top;
    read_xml(fileOptions.c_str(),top,2); // option 2 removes comment strings
    ptree& options = top.get_child("inputs.Settings");

    mippCorrOption = options.get<std::string>("MIPPCorrOption");
    Nuniverses = options.get<int>("NumberOfUniverses");
    
  }
  void MakeReweight::Initialize(){
    
    //Getting MIPP binning and the correlation parameters:
    CentralValuesAndUncertainties* cvu = CentralValuesAndUncertainties::getInstance();
    MIPPNumiYieldsBins*  myb =  MIPPNumiYieldsBins::getInstance(); 
    ThinTargetBins*  thinbin =  ThinTargetBins::getInstance(); 
    MIPPNumiMC*  mymc =  MIPPNumiMC::getInstance(); 
    const char* ppfxDir = getenv("PPFX_DIR");
       
    std::cout<<"Initializing correlation parameters"<<std::endl;
    cvu->readFromXML(Form("%s/uncertainties/Parameters_%s.xml",ppfxDir,mippCorrOption.c_str()));
    
    std::cout<<"Initializing bin data conventions"<<std::endl;
    myb->pip_data_from_xml(Form("%s/data/BINS/MIPPNumiData_PIP_Bins.xml",ppfxDir));
    myb->pim_data_from_xml(Form("%s/data/BINS/MIPPNumiData_PIM_Bins.xml",ppfxDir));
    myb->k_pi_data_from_xml(Form("%s/data/BINS/MIPPNumiData_K_PI_Bins.xml",ppfxDir));
    thinbin->pC_pi_from_xml(Form("%s/data/BINS/ThinTarget_pC_pi_Bins.xml",ppfxDir));
    thinbin->barton_pC_pi_from_xml(Form("%s/data/BINS/ThinTargetBarton_pC_pi_Bins.xml",ppfxDir));
    thinbin->pC_k_from_xml(Form("%s/data/BINS/ThinTargetLowxF_pC_k_Bins.xml",ppfxDir));
    thinbin->mipp_pC_k_pi_from_xml(Form("%s/data/BINS/ThinTarget_K_PI_Bins.xml",ppfxDir));
    thinbin->meson_incident_from_xml(Form("%s/data/BINS/ThinTarget_MesonIncident.xml",ppfxDir));
    thinbin->pC_p_from_xml(Form("%s/data/BINS/ThinTarget_pC_p_Bins.xml",ppfxDir));
    thinbin->pC_n_from_xml(Form("%s/data/BINS/ThinTarget_pC_n_Bins.xml",ppfxDir));
    thinbin->material_scaling_from_xml(Form("%s/data/BINS/ThinTarget_material_scaling_Bins.xml",ppfxDir));
    
    std::cout<<"Initializing MC values"<<std::endl;
    mymc->pip_mc_from_xml(Form("%s/data/MIPP/MIPPNuMI_MC_PIP.xml",ppfxDir));
    mymc->pim_mc_from_xml(Form("%s/data/MIPP/MIPPNuMI_MC_PIM.xml",ppfxDir));
    mymc->kap_mc_from_xml(Form("%s/data/MIPP/MIPPNuMI_MC_KAP.xml",ppfxDir));
    mymc->kam_mc_from_xml(Form("%s/data/MIPP/MIPPNuMI_MC_KAM.xml",ppfxDir));
    mymc->k0l_mc_from_xml(Form("%s/data/MIPP/MIPPNuMI_MC_K0L.xml",ppfxDir));
    mymc->k0s_mc_from_xml(Form("%s/data/MIPP/MIPPNuMI_MC_K0S.xml",ppfxDir));

    //Reweighter drivers:
    vec_rws.reserve(Nuniverses);
    std::cout<<"Initializing reweight drivers for "<<Nuniverses<<" universes"<<std::endl;
    
    const int base_universe=1000000;
    // cvPars.reserve(Nuniverses+1);
    univPars.reserve(Nuniverses+1);

    ParameterTable cvPars=cvu->getCVPars();

    std::cout<<"Loading parameters for universe: ";
    for(int ii=0;ii<Nuniverses;ii++){
      std::cout << ii << " " << std::flush;
      // cvPars.push_back(cvu->getCVPars());
      univPars.push_back(cvu->calculateParsForUniverse(ii+base_universe));
      vec_rws.push_back(new ReweightDriver(ii,cvPars,univPars[ii],fileOptions));
      vec_wgts.push_back(1.0);
    }
    std::cout << std::endl;
    
    //Central Value driver:
    //by convention, we use universe -1 to hold the cv. It is in agreement with CentralValueAndUncertainties  
    const int cv_id = -1;
    std::cout<<"Loading parameters for cv"<<std::endl;
    // cvPars.push_back(cvu->getCVPars());
    univPars.push_back(cvu->calculateParsForUniverse(cv_id));
    cv_rw = new ReweightDriver(cv_id,cvPars,univPars[Nuniverses],fileOptions); 

    cv_wgt = 1.0;

    std::cout<<"Done configuring universes"<<std::endl;

  }
  
  std::vector<double> MakeReweight::GetTotalWeights(){
    return vec_wgts;
  }
  /////
  void MakeReweight::calculateWeights(nu_g4numi* nu, const char* tgtcfg, const char* horncfg){
    InteractionChainData inter_chain(nu,tgtcfg,horncfg);
    doTheJob(&inter_chain);
  }
  /////
  void MakeReweight::calculateWeights(bsim::Dk2Nu* nu, bsim::DkMeta* meta){
    InteractionChainData inter_chain(nu,meta);
    doTheJob(&inter_chain);
  }
  /////
  void MakeReweight::doTheJob(InteractionChainData* icd){
    
    //check if there was any QEL-like interaction
    bool reprocess_qel = false;
    std::vector<bool> qel;
    for(const InteractionData & aa : icd->interaction_chain) {
      if((aa.Inc_pdg == pdg::P || aa.Inc_pdg == pdg::N) && aa.Inc_pdg == aa.Prod_pdg && (aa.xF > 0.95 || (aa.Pt < aa.xF - 0.5))) {
        qel.push_back(true);
        reprocess_qel = true;
      }
      else {
        qel.push_back(false);
      }
    }
    InteractionChainData * icd_qel;
    if(reprocess_qel) {
      icd_qel = new InteractionChainData(*icd);
      for(int i = 0; i < icd_qel->interaction_chain.size() - 1; i++) {
        if(qel[i]){
          const double momentum_adjustment = icd_qel->interaction_chain[i].Inc_P/icd_qel->interaction_chain[i+1].Inc_P;
          icd_qel->interaction_chain[i+1].Inc_P4[0] *= momentum_adjustment; 
          icd_qel->interaction_chain[i+1].Inc_P4[1] *= momentum_adjustment; 
          icd_qel->interaction_chain[i+1].Inc_P4[2] *= momentum_adjustment; 
          
          icd_qel->interaction_chain[i+1].Inc_P     = icd_qel->interaction_chain[i].Inc_P;
          icd_qel->interaction_chain[i+1].Inc_P4[3] = icd_qel->interaction_chain[i].Inc_P4[3];
          icd_qel->interaction_chain[i+1].Ecm       = icd_qel->interaction_chain[i].Ecm;
          icd_qel->interaction_chain[i+1].Betacm    = icd_qel->interaction_chain[i].Betacm;
          icd_qel->interaction_chain[i+1].Gammacm   = icd_qel->interaction_chain[i].Gammacm;
          /**
           * The intention of the code above was to pretend the QEL
           * interaction did not occur at all, but I realize this
           * approach is oversimplified, because as I modify the
           * particle momentum, PPFX will also look for data in another
           * MC bin. A proper study would require more changes to the
           * code
           */
        }
      }
    }
    
    //universe calculation:
    map_rew_wgts.clear();
    for(int ii=0;ii<Nuniverses;ii++){
      vec_wgts[ii] = vec_rws[ii]->calculateWeight(*icd);

      map_rew_wgts["MIPPNumiPionYields"].push_back(vec_rws[ii]->mipp_pion_wgt);
      map_rew_wgts["MIPPNumiKaonYields"].push_back(vec_rws[ii]->mipp_kaon_wgt);
      
      map_rew_wgts["TargetAttenuation"].push_back(vec_rws[ii]->att_wgt);
      map_rew_wgts["AbsorptionIC"].push_back(vec_rws[ii]->abs_ic_wgt);
      map_rew_wgts["AbsorptionDPIP"].push_back(vec_rws[ii]->abs_dpip_wgt);
      map_rew_wgts["AbsorptionDVOL"].push_back(vec_rws[ii]->abs_dvol_wgt);
      map_rew_wgts["AbsorptionNucleon"].push_back(vec_rws[ii]->abs_nucleon_wgt);
      map_rew_wgts["AbsorptionOther"].push_back(vec_rws[ii]->abs_other_wgt);
      map_rew_wgts["TotalAbsorption"].push_back(vec_rws[ii]->tot_abs_wgt);
      
      map_rew_wgts["ThinTargetpCPion"].push_back(vec_rws[ii]->pC_pi_wgt);
      map_rew_wgts["ThinTargetpCKaon"].push_back(vec_rws[ii]->pC_k_wgt);
      map_rew_wgts["ThinTargetnCPion"].push_back(vec_rws[ii]->nC_pi_wgt);
      map_rew_wgts["ThinTargetpCNucleon"].push_back(vec_rws[ii]->pC_nu_wgt);
      map_rew_wgts["ThinTargetMesonIncident"].push_back(vec_rws[ii]->meson_inc_wgt);
      map_rew_wgts["ThinTargetMesonIncident_ProjectilePip"].push_back(vec_rws[ii]->meson_inc_projectile_pip_wgt);
      map_rew_wgts["ThinTargetMesonIncident_ProjectilePim"].push_back(vec_rws[ii]->meson_inc_projectile_pim_wgt);
      map_rew_wgts["ThinTargetMesonIncident_ProjectileKp"].push_back(vec_rws[ii]->meson_inc_projectile_Kp_wgt);
      map_rew_wgts["ThinTargetMesonIncident_ProjectileKm"].push_back(vec_rws[ii]->meson_inc_projectile_Km_wgt);
      map_rew_wgts["ThinTargetMesonIncident_ProjectileK0"].push_back(vec_rws[ii]->meson_inc_projectile_K0_wgt);
      map_rew_wgts["ThinTargetMesonIncident_DaughterPip"].push_back(vec_rws[ii]->meson_inc_daughter_pip_wgt);
      map_rew_wgts["ThinTargetMesonIncident_DaughterPim"].push_back(vec_rws[ii]->meson_inc_daughter_pim_wgt);
      map_rew_wgts["ThinTargetMesonIncident_DaughterKp"].push_back(vec_rws[ii]->meson_inc_daughter_Kp_wgt);
      map_rew_wgts["ThinTargetMesonIncident_DaughterKm"].push_back(vec_rws[ii]->meson_inc_daughter_Km_wgt);
      map_rew_wgts["ThinTargetMesonIncident_DaughterK0"].push_back(vec_rws[ii]->meson_inc_daughter_K0_wgt);
      map_rew_wgts["ThinTargetnucleonA"].push_back(vec_rws[ii]->nuA_wgt);
      map_rew_wgts["ThinTargetnucleonAlFe"].push_back(vec_rws[ii]->nuAlFe_wgt);
      map_rew_wgts["Other"].push_back(vec_rws[ii]->other_wgt);
      
      if(reprocess_qel) {
        //reprocess the whole chain with QEL disabled
        double new_weight = vec_rws[ii]->calculateWeight(*icd_qel);
        map_rew_wgts["ThinTargetpCQEL"].push_back(vec_wgts[ii] - new_weight + 1);
      }
      else {
        map_rew_wgts["ThinTargetpCQEL"].push_back(1);
      }
    }
    
    //cv calculation:
    cv_wgt = cv_rw->calculateWeight(*icd);
    
  }
  ////
  std::vector<double> MakeReweight::GetWeights(std::string nameReweighter){

    std::vector<double> tmp_vec;
    std::map<std::string,std::vector<double> >::iterator it = map_rew_wgts.begin();
    it = map_rew_wgts.find(nameReweighter);
    if(it!=map_rew_wgts.end())tmp_vec = it->second;
    
    return tmp_vec;
   
  }
  ////
  double MakeReweight::GetCVWeight(){   
    return cv_wgt;
  }
  ///
  int  MakeReweight::GetNumberOfUniversesUsed(){
    return Nuniverses;
  }
  MakeReweight::~MakeReweight(){
    
  }
  ///
  MakeReweight* MakeReweight::getInstance(){
    if (instance == 0) instance = new MakeReweight;
    return instance;
  }
  ////
  void MakeReweight::resetInstance()
   {
      delete instance; 
      instance = 0; 
   }
}
