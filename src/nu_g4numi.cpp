
#include "nu_g4numi.h"

nu_g4numi::nu_g4numi(TChain* chain){

  nu_g4numi::fChain = chain;

  fChain->SetBranchAddress("Ntype", &Ntype);
  fChain->SetBranchAddress("NenergyN[11]",NenergyN);
  fChain->SetBranchAddress("NWtNear[11]", NWtNear);
  fChain->SetBranchAddress("NenergyF[2]", NenergyF);
  fChain->SetBranchAddress("NWtFar[2]",   NWtFar);
  fChain->SetBranchAddress("Nimpwt",  &Nimpwt);

  fChain->SetBranchAddress("ntrajectory", &ntrajectory);
  fChain->SetBranchAddress("overflow",    &overflow);
  fChain->SetBranchAddress("pdg[10]",     pdg);

  fChain->SetBranchAddress("pprodpx[10]", pprodpx);
  fChain->SetBranchAddress("pprodpy[10]", pprodpy);
  fChain->SetBranchAddress("pprodpz[10]", pprodpz);


  fChain->SetBranchAddress("startpx[10]", startpx);
  fChain->SetBranchAddress("startpy[10]", startpy);
  fChain->SetBranchAddress("startpz[10]", startpz);
  fChain->SetBranchAddress("stoppx[10]",  stoppx);
  fChain->SetBranchAddress("stoppy[10]",  stoppy);
  fChain->SetBranchAddress("stoppz[10]",  stoppz);

  fChain->SetBranchAddress("startx[10]", startx);
  fChain->SetBranchAddress("starty[10]", starty);
  fChain->SetBranchAddress("startz[10]", startz);
  fChain->SetBranchAddress("stopx[10]",  stopx);
  fChain->SetBranchAddress("stopy[10]",  stopy);
  fChain->SetBranchAddress("stopz[10]",  stopz);

  fChain->SetBranchAddress("proc[10]",    proc);
  fChain->SetBranchAddress("ivol[10]",    ivol);
  fChain->SetBranchAddress("fvol[10]",    fvol);

  fChain->SetBranchAddress("tpx",   &tpx);
  fChain->SetBranchAddress("tpy",   &tpy);
  fChain->SetBranchAddress("tpz",   &tpz);
  
  fChain->SetBranchAddress("tvx",   &tvx);
  fChain->SetBranchAddress("tvy",   &tvy);
  fChain->SetBranchAddress("tvz",   &tvz);
  

  fChain->SetBranchAddress("tptype",&tptype);



  fChain->SetMakeClass(1);
  
  nu_g4numi::ntrees   = fChain->GetNtrees();
  nu_g4numi::nentries = fChain->GetEntries();
  
}

void nu_g4numi::GetEntry(Int_t ientry){
  
  fChain->GetEntry(ientry);
  
}

nu_g4numi::~nu_g4numi(){
  
}
 
