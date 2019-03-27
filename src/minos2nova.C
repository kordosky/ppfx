
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
const int Ndet  = 3;
const int Nhel  = 2;

const int Nbins        = 101;
const int Nbins_plus_1 = 102;
const double binw      = 0.2; //GeV
const double emin      =   0.;
const double emax      = 120.;
const char* hel[Nhel]  = {"numu","numubar"};
const char* det[Ndet]  = {"MINOS_ND","NOvA_ND","NOvA_FD"};
//check in dkmetaTree if these indices are right for the g4numi ntuples being used.  
const int idet[Ndet]   = {2,3,9}; 

using namespace NeutrinoFluxReweight;

class nu_g4numi;
class Numi2Pdg;

int idx_hel(int pdgdcode);

/*!
 * Run to get the MINOS to NOvA ND/FD transformation matrices. The input is file with a list of ntuples, 
 * the name of the root output file and the option file. In this script, we are storing only 
 * the central value. I recommend to change the number of universes to be used 
 * to 1 or 2: <NumberOfUniverses>100</NumberOfUniverses> in the scripts/inputs_default.xml
 */
void minos2nova(const char* inputFiles, const char* outputFile, const char* optionsFile){ 
  
  //variable binning to catch MINOS and NOvA peaks
  double binning[Nbins_plus_1];
  for(int i=0;i<Nbins;i++)binning[i] = double(i)*binw;
  binning[Nbins] = emax;

  std::cout<< "Instance of MakeReweight()" <<std::endl;
  MakeReweight* makerew = MakeReweight::getInstance();
  makerew->SetOptions(optionsFile); 
  
  std::cout<<"Making an output file to store histograms"<<std::endl;
  TFile* fOut = new TFile(outputFile,"recreate");
  std::cout<<"File name: "<<fOut->GetName()<<std::endl;
  
  int Nuniv = makerew->GetNumberOfUniversesUsed();
  std::cout<<"Nuniverses "<<Nuniv<<std::endl;  
  
  //Histograms:
  TH1D* hflux[Nhel][Ndet];
  TH2D* hwgts[Nhel][Ndet-1];
  TH2D* htrans[Nhel][Ndet-1];
  for(int i=0;i<Nhel;i++){
    for(int j=0;j<Ndet;j++){
      hflux[i][j] = new TH1D(Form("hflux_pppfxcv_%s_%s",det[j],hel[i]),"",Nbins,binning); //this is to check the tranformation
      if(j!=0){
	hwgts[i][j-1]  = new TH2D(Form("hwgts_ppfxcv_%s_vs_%s_%s"  ,det[0],det[j],hel[i]),"",Nbins,binning,Nbins,binning);
	htrans[i][j-1] = new TH2D(Form("htrans_ppfxcv_%s_to_%s_%s" ,det[0],det[j],hel[i]),"",Nbins,binning,Nbins,binning);
      }
    }
  }
    
  //Loading ntuples:
  TChain* chain_evts   = new TChain("dk2nuTree");  
  TChain* chain_meta   = new TChain("dkmetaTree");  
  bsim::Dk2Nu*  dk2nu  = new bsim::Dk2Nu;  
  bsim::DkMeta* dkmeta = new bsim::DkMeta;
  
  std::cout<<" Adding ntuple from: "<<inputFiles<<std::endl;
  
  std::ifstream* fileIn = new std::ifstream(inputFiles);
  std::string line;
  int count_files = 0;
  if(fileIn->is_open()){
    while(getline(*fileIn,line)){
      if(line.find(".root")<2000){
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
 
  std::cout<<"N of entries: "<<nentries<<std::endl;
  
  double flxwgt[Ndet] = {0,0,0};
  double enu[Ndet] = {0,0,0};
  
  for(int ii=0;ii<nentries;ii++){  

    if(ii%100000==0)std::cout<<ii/1000<<" k evts"<<std::endl;
    chain_evts->GetEntry(ii); 
    int nuidx = idx_hel(dk2nu->decay.ntype);   
    if(nuidx<0)continue;
    
    makerew->calculateWeights(dk2nu,dkmeta);    
    double cvwgt = makerew->GetCVWeight();    
    for(int kk=0;kk<Ndet;kk++){
      flxwgt[kk] = ( (dk2nu->nuray)[idet[kk]].wgt )*(dk2nu->decay.nimpwt)/3.1416;
      enu[kk]    =   (dk2nu->nuray)[idet[kk]].E;
      if(flxwgt[kk]<0 || enu[kk]<0)std::cout<<"=> Wrong enu, wgt values"<<std::endl; 
      
      hflux[nuidx][kk]->Fill(enu[kk],flxwgt[kk]*cvwgt);
      if(kk!=0){
	hwgts[nuidx][kk-1]->Fill(enu[0],enu[kk],flxwgt[kk]*cvwgt);
      }
    }//end of detectors
  } //end of entries
  
  //Calculating the MINOS to NOvA matrix:
  for(int i=0;i<Nhel;i++){
    for(int j=1;j<Ndet;j++){
      for(int p=1;p<=Nbins;p++){
	for(int k=1;k<=Nbins;k++){
	  double MINOS_cont = hflux[i][0]->GetBinContent(k);	  
	  double wgts_cont  = hwgts[i][j-1]->GetBinContent(k,p);	  
	  if(MINOS_cont>0)htrans[i][j-1]->SetBinContent(k,p,wgts_cont/MINOS_cont);
	  else std::cout<<"Wrong values: MINOS_cont, wgts_cont: "<<MINOS_cont<<", "<<wgts_cont<<" and type: "
			<<hel[i]<<", det: "<<det[j]<<", binY: "<<p<<", binX: "<<k<<std::endl;
	}
      }//end of bins
    }//end of det
  }//end of hel

  std::cout<<"storing general histos"<<std::endl;
  fOut->cd();
  for(int i=0;i<Nhel;i++){
    htrans[i][1]->Write();
    htrans[i][2]->Write();
    fOut->mkdir(Form("h%s",hel[i]));
    fOut->cd(Form("h%s",hel[i]));
    for(int j=0;j<Ndet;j++){
      hflux[i][j]->Write();	
      if(j!=0){
	hwgts[i][j-1]->Write();
      }
    }
  }
  
  //Releasing memory:
  makerew->resetInstance();
  
  std::cout<<"End of run()"<<std::endl;

}
  
int idx_hel(int pdgcode){
  int idx = -1;
  if(pdgcode ==  14)idx = 0;
  if(pdgcode == -14)idx = 1;
  return idx;
}

//////////////

#ifndef __CINT__
int main(int argc, const char* argv[]){
  
  minos2nova(argv[1],argv[2],argv[3]);
  return 0;
}
#endif
