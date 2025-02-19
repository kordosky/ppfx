#include "dk2nu/tree/dkmeta.h"
#include "dk2nu/tree/dk2nu.h"
#include "MakeReweight.h"
#include "NuWeight.h"
#include "InteractionChainData.h"
#include "InteractionData.h"
#include "ParticlesThroughVolumesData.h"
#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <fstream>

using namespace NeutrinoFluxReweight;

void trial_ivol(const char* inputFile,const char* outputFile){

//start with the reweighter...not sure if we need it or not

  //MakeReweight* makerew = MakeReweight::getInstance(); // unused
  
  TChain* chain_evts   = new TChain("dk2nuTree");  
  TChain* chain_meta   = new TChain("dkmetaTree");  
  bsim::Dk2Nu*  dk2nu  = new bsim::Dk2Nu;  
  bsim::DkMeta* dkmeta = new bsim::DkMeta;
  
  
 std::ifstream ifs(inputFile);
  //  ifs.open(inputFile);
  std::string line;
  int counter = 0;
  while (ifs.good()) {
    getline(ifs,line);
    if(line.find(".root")>10000)continue;
    chain_evts->Add(line.c_str());   
    if(counter==0)chain_meta->Add(line.c_str());
    std::cout<<"Entering: "<<line<<std::endl;
    counter++;
  }
  ifs.close();  

  chain_evts->SetBranchAddress("dk2nu",&dk2nu);
  //int nentries  = chain_evts->GetEntries(); // unused

  chain_meta->SetBranchAddress("dkmeta",&dkmeta);
  chain_meta->GetEntry(0); //all entries are the same    
  
  //int ntot = chain_evts->GetEntries(); // unused
  
  for(int i =0;i<1000;i++){
   chain_evts->GetEntry(i);
   NeutrinoFluxReweight::InteractionChainData icd(dk2nu,dkmeta);  
  
   //const int ninter = icd.interaction_chain.size(); // unused
   
   //const int ptv_size = icd.ptv_info.size(); // unused
   
   //std::cout<<"intersize "<<ninter<<" ptv size "<<ptv_size<<std::endl;
  
  //get the interaction volume info....
  std::cout<<"IC:Entry "<<i<<" PARENT "<<icd.ptv_info[0].Pdgs[0]<<" GRANDPARENT"<<icd.ptv_info[0].Pdgs[1]<<" great GRANT PARENT "<<icd.ptv_info[0].Pdgs[2]<<std::endl;
  std::cout<<"DPIP:Entry "<<i<<" PARENT "<<icd.ptv_info[1].Pdgs[0]<<" GRANDPARENT"<<icd.ptv_info[1].Pdgs[1]<<" great GRANT PARENT "<<icd.ptv_info[1].Pdgs[2]<<std::endl;
  std::cout<<"DVOL:Entry "<<i<<" PARENT "<<icd.ptv_info[2].Pdgs[0]<<" GRANDPARENT"<<icd.ptv_info[2].Pdgs[1]<<" great GRANT PARENT "<<icd.ptv_info[2].Pdgs[2]<<std::endl; 
  std::cout<<"VOL INFO "<<icd.ptv_info[0].Vol<<" "<<icd.ptv_info[1].Vol<<" "<<icd.ptv_info[2].Vol<<std::endl; 
 }  

}

#ifndef __CINT__
int main(int argc,char* argv[]){
   trial_ivol(argv[1],argv[2]);

}
#endif
