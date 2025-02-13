
#include "dk2nu/tree/dkmeta.h"
#include "dk2nu/tree/dk2nu.h"
#include "MakeReweight.h"

#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <fstream>

#include "TH1D.h"
#include "TH2D.h"

//Some constants:
const int Nhel    = 4;
const int Nhad    = 3;
const int NbinsE  = 480;
const int NbinsPt = 100;
const int NbinsxF = 200;
const double pi    = 3.14159265358979323846;
//
const char* hel[Nhel]   = {"numu","numubar","nue","nuebar"};
const double emin       =   0.;
const double emax       = 120.;
const double xfmin      =  -1.;
const double xfmax      =   1.;
const double ptmin      =   0.;
const double ptmax      =   1.;
const char* had[Nhad]   = {"pi","K","oth"};

using namespace NeutrinoFluxReweight;

void usage();
int idx_hel(int pdgdcode);
int idx_had(int pdgdcode);
//! returns string with "proper" xrootd enabled path if reading from PNFS (dCache)
std::string change_to_xrootd_path(std::string temp);


void make_basic_fluxhistograms(const char* inputFile, const char* outputFile, const char* cdet){ 
  
  //const char* thisDir = getenv("PPFX_DIR"); // unused
  int idet = atoi(cdet);
  
  std::cout<<"Making an output file to store histograms"<<std::endl;
  TFile* fOut = new TFile(outputFile,"recreate");
  std::cout<<"File name: "<<fOut->GetName()<<std::endl;
  
  TH1D* hflux[Nhel];
  TH2D* hkin_prod[Nhel][Nhad];
  for(int i=0;i<Nhel;i++){
    hflux[i] = new TH1D(Form("hflux_%s",hel[i]),"",NbinsE,emin,emax);
    for(int j=0;j<Nhad;j++){      
      hkin_prod[i][j] = new TH2D(Form("hkin_prod_%s_%s",hel[i],had[j]),"",NbinsxF,xfmin,xfmax,NbinsPt,ptmin,ptmax);
    }
  }
    
  //Loading ntuples:
  TChain* chain_evts   = new TChain("dk2nuTree");  
  TChain* chain_meta   = new TChain("dkmetaTree");  
  bsim::Dk2Nu*  dk2nu  = new bsim::Dk2Nu;  
  bsim::DkMeta* dkmeta = new bsim::DkMeta;
  


  std::ifstream* fileIn = new std::ifstream(inputFile);
  std::string line;
  int count_files = 0;
  if(fileIn->is_open()){
    while(getline(*fileIn,line)){
      if(line.find(".root")<1000){
        line = change_to_xrootd_path(line);
	chain_evts->Add(line.c_str());
	std::cout<<"File In: "<<line<<std::endl;
	count_files++;
	if(count_files==1)chain_meta->Add(line.c_str());
      }
    }
  }

  chain_evts->SetBranchAddress("dk2nu",&dk2nu);
  int nentries  = chain_evts->GetEntries();

  chain_meta->SetBranchAddress("dkmeta",&dkmeta);
  chain_meta->GetEntry(0); //all entries are the same     

  std::cout<<"=> Doing the analysis for: "<< (dkmeta->location)[idet].name <<std::endl; 

  std::cout<<"N of entries: "<<nentries<<std::endl;

  for(int ii=0;ii<nentries;ii++){  
    if(ii%100000==0)std::cout<<ii/1000<<" k evts"<<std::endl;
    chain_evts->GetEntry(ii);     

    double flxwgt = ( (dk2nu->nuray)[idet].wgt )*(dk2nu->decay.nimpwt)/pi; //	3.1416;
    double enu    = (dk2nu->nuray)[idet].E; 

    int nuidx = idx_hel(dk2nu->decay.ntype);        
    if(nuidx<0){
      std::cout<<"=> Wrong neutrino file"<<std::endl;
    }    
    hflux[nuidx]->Fill(enu,flxwgt);
    
    InteractionChainData ichaindata(dk2nu,dkmeta);
    //unsigned int nanc = (ichaindata.interaction_chain).size(); // unused
    InteractionData idata = (ichaindata.interaction_chain)[0];
    double xF  = idata.xF;
    double Pt  = idata.Pt;
    int hadidx = idx_had(idata.Prod_pdg);
    hkin_prod[nuidx][hadidx]->Fill(xF,Pt,flxwgt);

  }

  std::cout<<"storing general histos"<<std::endl;
  fOut->cd();
  for(int i=0;i<Nhel;i++){
    fOut->mkdir(hel[i]);
    fOut->cd(hel[i]);
    hflux[i]->Write();
    for(int j=0;j<Nhad;j++){  
      hkin_prod[i][j]->Write();
    }
  }
  

  std::cout<<"End of run()"<<std::endl;

} // end make_basic_flux_histograms

std::string change_to_xrootd_path(std::string temp){
//  pnfs access via root streaming is now not OK. Must switch to xrootd. Dec 2021 -Pierce Weatherly
//  std::string input_flux_dir_fe =  "/pnfs/dune/"+disk+"/users/"+input_user+"/fluxfiles/g4lbne/"+version+"/"+physics_list+"/"+macro+"/"+current+"/flux/";
//  std::string rootxdstr = "root://fndca1.fnal.gov:1094/pnfs/fnal.gov/usr/dune/";
//  std::string input_flux_dir =  rootxdstr+disk+"/users/"+input_user+"/fluxfiles/g4lbne/"+version+"/"+physics_list+"/"+macro+"/"+current+"/flux/";
//  std::cout<<"\n\t Using input directory: "<<input_flux_dir<<std::endl;
//  if(on_grid) {
//    input_flux_dir = getenv("_CONDOR_SCRATCH_DIR");
//    input_flux_dir += "/";
//    std::cout<<"Running on grid, so getting flux files from local disk: "<<input_flux_dir<<std::endl;
//  }

  // pnfs - dCache access via XROOTD logic
  bool enable_xrootd = true;
  std::string temp_inputFile = temp;
  std::string rootxdstr = "root://fndca1.fnal.gov:1094/pnfs/fnal.gov/usr/";
  std::size_t index = temp_inputFile.find("/pnfs/");
  if( index == std::string::npos || index != 0) enable_xrootd = false; // cannot find /pnfs/
  else if(index == 0) enable_xrootd = true; // if the path starts with /pnfs/ , then go ahead and use xrootd.
//  if(enable_xrootd && temp_inputFile.find(grid_dir) != string::npos) enable_xrootd = false;  // if on-grid, leave alone
//  if(enable_xrootd && temp_inputFile.find("/data/") != string::npos) enable_xrootd = false;  // if grabing from dune/data or whatever, leave along; make sure not in pnfs
 // if(enable_xrootd && temp_inputFile.find("root:/") != string::npos) enable_xrootd = false;  // if it looks like it is xrootd already
  if(enable_xrootd){
        temp_inputFile.replace(0,6,rootxdstr);
        printf("\n Replacing string for inputFIle %s\n\t with XROOTD path: %s\n", temp.c_str(), temp_inputFile.c_str() );
  }else printf("\n Input File not on /pnfs/ (dCache); File path: %s\n", temp_inputFile.c_str() );
  return temp_inputFile;
}




int idx_hel(int pdgcode){
  int idx = -1;
  if(pdgcode ==  14)idx = 0;
  if(pdgcode == -14)idx = 1;
  if(pdgcode ==  12)idx = 2;
  if(pdgcode == -12)idx = 3;
  return idx;
}

int idx_had(int pdgcode){
  int idx = -1;
  if(abs(pdgcode) == 211)   idx = 0;
  else if(abs(pdgcode)==321)idx = 1;
  else idx = 2;
  return idx;
}

void usage(){
  
  std::cout<<"\"make_basic_fluxhistograms\" create simple flux histograms"<<std::endl;
  std::cout<<"================================================================"<<std::endl;
  std::cout<<"make_basic_fluxhistograms [inputFile] [outputFile] [cdet]"<<std::endl;
  std::cout<<"================================================================"<<std::endl;
  std::cout<<"inputFile  : list of dk2nu flux files."<<std::endl;
  std::cout<<"outputFile : root file to store histograms."<<std::endl;
  std::cout<<"cdet       : index of the detector (see dkmeta)."<<std::endl;
  
}
//////////////

#ifndef __CINT__
int main(int argc, const char* argv[]){

  if(argc==4){   
    make_basic_fluxhistograms(argv[1],argv[2],argv[3]);
  }
  else{
    usage();
    exit (1);
  }
  
  return 0;
}
#endif
