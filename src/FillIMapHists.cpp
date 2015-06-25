#include "FillIMapHists.h"
#include "InteractionChainData.h"
#include "InteractionData.h"
#include "TDatabasePDG.h"
#include "TParticlePDG.h"
#include <iostream>

void FillIMapHists(TChain* tdk2nu, TChain* tdkmeta, HistList* hists, const FillIMapHistsOpts* opts){
  
  
  // setup the event loop, filling Dk2Nu and DkMeta objects
  bsim::Dk2Nu*  dk2nu  = new bsim::Dk2Nu;  
  bsim::DkMeta* dkmeta = new bsim::DkMeta;
  tdk2nu->SetBranchAddress("dk2nu",&dk2nu);
  Long64_t nentries  = tdk2nu->GetEntries();
  Long64_t ntrees    = tdk2nu->GetNtrees();
  tdkmeta->SetBranchAddress("dkmeta",&dkmeta);


  std::cout<<"FillIMapHists looping over "<<ntrees<<" trees with a total of "<<nentries<<" entries."<<std::endl;
  for(Long64_t ientry=0;ientry<nentries;ientry++){
    tdk2nu->GetEntry(ientry);
    tdkmeta->GetEntry(ientry);    
  
    FillOneEntry(dk2nu,dkmeta,hists,opts); 
  }
}


double FillOneEntry(bsim::Dk2Nu* dk2nu, bsim::DkMeta* dkmeta, HistList* hists, const FillIMapHistsOpts* opts){
  double weight=0.0;
  TDatabasePDG* pdg = TDatabasePDG::Instance();
  // check that the neutrino is of the requested type and that
  // the energy is within range
  const int nu_type=dk2nu->decay.ntype;
  // setting opts.nuid=0 will result in all neutrino species being plotted
  if( opts->nuid!=0 && nu_type!= opts->nuid) return weight;
  const double enu=dk2nu->nuray[0].E; // energy at MINOS ND
  if(enu<opts->elow || enu>opts->ehigh) return weight;
  
  const double nwtnear=dk2nu->nuray[0].wgt;
  const double nimpwt=dk2nu->decay.nimpwt;
  weight=nwtnear*nimpwt;
  
  NeutrinoFluxReweight::InteractionChainData icd(dk2nu,dkmeta);
  const int ninter=icd.interaction_chain.size();
  for(int iinter=0; iinter<ninter; iinter++){
    
    const NeutrinoFluxReweight::InteractionData& interdata
      =icd.interaction_chain[iinter];
    
    //////////////////////// TBD ////////////////////////////////////////
    // would check here if this interaction is covered by NA49, MIPP, etc
    /////////////////////////////////////////////////////////////////////

    // get an index into the large arrays listing the volume names
    // and the material of each volume.
    int mv_idx=FindIndexFromVolume(interdata.Vol);
    if(mv_idx==-1){
      std::cout<<"Skipping unknown volume "<< interdata.Vol
	       <<" at for interaction "<<iinter<<std::endl;
    }
    // fill a 2D histogram of projectile vs. material
    const string proj_name=pdg->GetParticle(interdata.Inc_pdg)->GetName();
    const string prod_name=pdg->GetParticle(interdata.Prod_pdg)->GetName();
    hists->_h_in_vs_mat->Fill(IMap::materials[mv_idx],proj_name.c_str(),weight);

  }
  

  // the end
  return weight;
}


int FindIndexFromVolume(const std::string& wanted){
  for(int i=0; i<IMap::nvol; i++){
    if(wanted== std::string(IMap::volume[i])) return i;
  }
  return -1;
}
