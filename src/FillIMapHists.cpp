#include "FillIMapHists.h"
#include "InteractionChainData.h"

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
    NeutrinoFluxReweight::InteractionChainData icd(dk2nu,dkmeta);
    FillOneEntry(icd,hists,opts); 
  }
}


void FillOneEntry(const NeutrinoFluxReweight::InteractionChainData& icd, HistList* hists, const FillIMapHistsOpts* opts){
  
}
