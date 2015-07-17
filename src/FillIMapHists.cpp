#include "FillIMapHists.h"
#include "InteractionChainData.h"
#include "InteractionData.h"
#include "CentralValuesAndUncertainties.h"
#include "MIPPNumiYieldsBins.h"
#include "MIPPNumiMC.h"
#include "TDatabasePDG.h"
#include "TParticlePDG.h"
#include <iostream>
#include <math.h>

double FillIMapHists(TChain* tdk2nu, TChain* tdkmeta, HistList* hists, const FillIMapHistsOpts* opts){
  using namespace NeutrinoFluxReweight;
  
  // setup the event loop, filling Dk2Nu and DkMeta objects
  bsim::Dk2Nu*  dk2nu  = new bsim::Dk2Nu;  
  bsim::DkMeta* dkmeta = new bsim::DkMeta;
  tdk2nu->SetBranchAddress("dk2nu",&dk2nu);
  Long64_t nentries  = tdk2nu->GetEntries();
  Long64_t ntrees    = tdk2nu->GetNtrees();
  tdkmeta->SetBranchAddress("dkmeta",&dkmeta);

  ////////////////////// initializing reweighters ///////////////////////////
  //
  // here, we need to initialize the reweighers
  // and pass them into FillOneEntry
  // so it can use them to determine if a particular interaction
  // or chain of interactions can be reweighted

  ///// Confused? 
  ///// All this business with xml files, parameter tables
  ///// and the like is simply needed to ensure correct operation
  ///// of the reweight drivers, specifically the MIPP ones,
  ///// which call MIPPNumiYieldsBins::getInstance(); and
  ///// expect the result to be initialized.
  CentralValuesAndUncertainties* cvu = CentralValuesAndUncertainties::getInstance();;
  MIPPNumiYieldsBins*  myb =  MIPPNumiYieldsBins::getInstance(); 
  MIPPNumiMC*  mymc =  MIPPNumiMC::getInstance(); 
  const char* ppfxDir = getenv("PPFX_DIR");
  cvu->readFromXML(Form("%s/uncertainties/Parameters_uhighBYDET.xml",ppfxDir));
  myb->pip_data_from_xml(Form("%s/data/BINS/MIPPNumiData_PIP_Bins.xml",ppfxDir));
  myb->pim_data_from_xml(Form("%s/data/BINS/MIPPNumiData_PIM_Bins.xml",ppfxDir));
  myb->k_pi_data_from_xml(Form("%s/data/BINS/MIPPNumiData_K_PI_Bins.xml",ppfxDir));
  mymc->pip_mc_from_xml(Form("%s/data/MIPP/MIPPNuMI_MC_PIP.xml",ppfxDir));
  mymc->pim_mc_from_xml(Form("%s/data/MIPP/MIPPNuMI_MC_PIM.xml",ppfxDir));
  mymc->kap_mc_from_xml(Form("%s/data/MIPP/MIPPNuMI_MC_KAP.xml",ppfxDir));
  mymc->kam_mc_from_xml(Form("%s/data/MIPP/MIPPNuMI_MC_KAM.xml",ppfxDir));
  
  ParameterTable cvPars=cvu->getCVPars();
  const int iUniv=99;
  ParameterTable univPars=cvu->calculateParsForUniverse(iUniv);

  ////// now make the reweighters after doing all that stuff above 
  FillIMapHistsReweighters reweighters;
  reweighters.NumiPions = new MIPPNumiYieldsReweighter(iUniv,cvPars,univPars);
  reweighters.NumiKaons = new MIPPNumiKaonsYieldsReweighter(iUniv,cvPars,univPars);
  reweighters.NA49 = new NA49Reweighter(iUniv,cvPars,univPars);    
  reweighters.ThinKaons = new MIPPThinTargetReweighter(iUniv,cvPars,univPars);
  //////////////////// done initializing reweighers ////////////////////////


  std::cout<<"FillIMapHists looping over "<<ntrees<<" trees with a total of "<<nentries<<" entries."<<std::endl;
  double total_weight=0.0;
  for(Long64_t ientry=0;ientry<nentries;ientry++){
    tdk2nu->GetEntry(ientry);
    tdkmeta->GetEntry(ientry);    
  
    total_weight+=FillOneEntry(dk2nu,dkmeta,hists,opts,&reweighters); 
  }
  return total_weight;
}

//#define DEBUG

double FillOneEntry(bsim::Dk2Nu* dk2nu, bsim::DkMeta* dkmeta, HistList* hists, const FillIMapHistsOpts* opts, FillIMapHistsReweighters* reweighters){
  double weight=0.0;
  TDatabasePDG* pdg = TDatabasePDG::Instance();
  // check that the neutrino is of the requested type and that
  // the energy is within range
  const int nu_type=dk2nu->decay.ntype;
  const int nuray_idx=1; // this corresponds to the location of minerva
  const double enu=dk2nu->nuray[nuray_idx].E; // energy at MINOS ND

#ifdef DEBUG
  std::cout<<"FillOneEntry() for nu_type= "<<nu_type
	   <<" and energy "<<enu<<std::endl;
#endif
  // setting opts.nuid=0 will result in all neutrino species being plotted
  if( (opts->nuid!=0 && nu_type!= opts->nuid) 
      || (enu<opts->elow || enu>opts->ehigh) ){
#ifdef DEBUG
    std::cout<<"Fails cut on nu_type or energy"<<std::endl;
#endif 
    return 0;
  }
  const double nwtnear=dk2nu->nuray[nuray_idx].wgt;
  const double nimpwt=dk2nu->decay.nimpwt;
  weight=nwtnear*nimpwt;


  /*
  if(isnan(weight)){
    std::cout<<"Encountered a NaN weight, dk2nu follows"<<std::endl;
    std::cout<<(*dk2nu)<<std::endl;    
  }
  */
  NeutrinoFluxReweight::InteractionChainData icd(dk2nu,dkmeta);
  const int ninter=icd.interaction_chain.size();
  std::vector<bool> numi_pion_nodes=reweighters->NumiPions->canReweight(icd);
  std::vector<bool> numi_kaon_nodes=reweighters->NumiKaons->canReweight(icd);

#ifdef DEBUG
  std::cout<<"Passes energy cut and has a weight of "<<weight
	   <<" with "<<ninter<<" entries in ancestry chain"<<std::endl;
#endif 

  int ninter_all=0; // a variable to count all non-Decay interactions
  int ninter_cuts=0;// ... and only those passing the MIPP/NA49/etc cuts
  

  for(int iinter=0; iinter<ninter; iinter++){
    
    const NeutrinoFluxReweight::InteractionData& interdata
      =icd.interaction_chain[iinter];
#ifdef DEBUG
    std::cout<<"Processing interaction "<<iinter<<endl;
    interdata.print(std::cout);
#endif 
    // check to see if this entry is a decay
    if(interdata.Proc=="Decay"){
#ifdef DEBUG
      std::cout<<"   This is a decay, skip it"<<std::endl;
#endif
      continue; // if so, don't histogram
    }
    ninter_all++;
    /////////////////////////////////////////////////////////////////////
    // check here if this interaction is covered by NA49, MIPP, etc
    /////////////////////////////////////////////////////////////////////
    
    if(opts->cut_mipp && numi_pion_nodes[iinter]) continue;
    if(opts->cut_mipp && numi_kaon_nodes[iinter]) continue;
    // at the time of this writing, the NA49 reweigher handles 
    // pion, proton and kaon production by protons
    bool covered_by_na49 = reweighters->NA49->canReweight(interdata);
    if(opts->cut_na49 && covered_by_na49) continue;
    bool covered_by_thin_kaons = reweighters->ThinKaons->canReweight(interdata);
    if(opts->cut_na49 && covered_by_thin_kaons) continue;
    ninter_cuts++;
    // get an index into the large arrays listing the volume names
    // and the material of each volume.
    int mv_idx=FindIndexFromVolume(interdata.Vol);
    if(mv_idx==-1){
      std::cout<<"Skipping unknown volume "<< interdata.Vol
	       <<" for interaction "<<iinter<<std::endl;
    }
    // fill a 2D histogram of projectile vs. material
    const string proj_name=pdg->GetParticle(interdata.Inc_pdg)->GetName();
    const string prod_name=pdg->GetParticle(interdata.Prod_pdg)->GetName();
    hists->_h_in_vs_mat->Fill(IMap::materials[mv_idx],proj_name.c_str(),weight);

    // figure out if the produced particle is one that we want
    // to record in histograms
    // The list of such particles is in IMap::popparticle
    // the strange name is apparently a contraction: "popular particles"
    const int prod_pop_idx=FindIndexFromParticleName(prod_name);
    const int proj_pop_idx=FindIndexFromParticleName(proj_name);
#ifdef DEBUG
    std::cout<<"   Projectile: "<<proj_name<<" with popidx "<<proj_pop_idx<<std::endl;
    std::cout<<"   Produced  : "<<prod_name<<" with popidx "<<prod_pop_idx<<std::endl;
#endif
    

    // look at things from the produced particles standpoint
    if(prod_pop_idx!=-1){ // for each of the commonly produced particles.

      // histogram kinetic energy, 3-momentum, and xF,pT
      const double produced_KE=interdata.Prod_P4[3]-interdata.Prod_Mass;
      hists->_hkepop_tot[prod_pop_idx]->Fill(produced_KE,weight);
      hists->_htmpop_tot[prod_pop_idx]->Fill(interdata.Prod_P,weight);
      hists->_hxfpt_tot[prod_pop_idx]->Fill(interdata.xF,interdata.Pt,weight);

      // histogram the material that the interaction occured in
      // along with the projectile that made the particle in question
      hists->_hmatbkw[prod_pop_idx]->Fill(IMap::materials[mv_idx],proj_name.c_str(),weight);
      
      // now, dig deeper
      if(proj_pop_idx!=-1){ // for each of the common *projectiles*
	// histogram kinetic energy, 3-momentum, and xF,pT
	// of the produced particle
	hists->_hkepop[prod_pop_idx][proj_pop_idx]->Fill(produced_KE,weight);
	hists->_htmpop[prod_pop_idx][proj_pop_idx]->Fill(interdata.Prod_P,weight);
	hists->_hxfpt[prod_pop_idx][proj_pop_idx]->Fill(interdata.xF,interdata.Pt,weight);
      }
    }
    
    // now look at things from the projectile's standpoint
    if(proj_pop_idx!=-1){ // for each of the common projectiles
      // histogram the kinetic energy of the projectile
      const double projectile_KE=interdata.Inc_P4[3]-interdata.Inc_Mass;
      hists->_henergytotal[proj_pop_idx]->Fill(projectile_KE,weight);
      // histogram the volume/material and the produced particle
      hists->_hmat[proj_pop_idx]->Fill(IMap::materials[mv_idx],prod_name.c_str(),weight);
      hists->_hvol[proj_pop_idx]->Fill(IMap::volume[mv_idx],prod_name.c_str(),weight);
      // histogram the energy of the projectile for each volume
      // This may be overkill!
      if(mv_idx!=-1) hists->_henergyvolume[mv_idx][proj_pop_idx]->Fill(projectile_KE,weight);
      if(projectile_KE>118 and proj_pop_idx==1){
	std::cout<<"Oh noes!"<<std::endl;
      }
    }

  }

  // now fill the # of interactions vs enu
  hists->_h_nint_vs_enu->Fill(enu,ninter_all,weight);
  hists->_h_nint_vs_enu_cuts->Fill(enu,ninter_cuts,weight);
  
  // the end
  return weight;
}


int FindIndexFromVolume(const std::string& wanted){
  for(int i=0; i<IMap::nvol; i++){
    if(wanted== std::string(IMap::volume[i])) return i;
  }
  return -1;
}

int FindIndexFromParticleName(const std::string& wanted){
  for(int i=0; i<IMap::npop; i++){
    if(wanted== std::string(IMap::popparticle[i])) return i;
  }
  return -1;
}

