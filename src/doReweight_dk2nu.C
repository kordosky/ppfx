
#include "dk2nu/tree/dkmeta.h"
#include "dk2nu/tree/dk2nu.h"
#include "MakeReweight.h"

#include <string>
#include <cstddef>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include "TH1D.h"

//Some constants::

const int NbinsE   = 120;
const double emin  =   0.;
const double emax  = 120.;
const int Nnuhel   = 4;
const char* nuhel[Nnuhel] = {"numu","numubar","nue","nuebar"};

using namespace NeutrinoFluxReweight;

class nu_g4numi;
class Numi2Pdg;

int idx_hel(int pdgdcode);

/*!
 * Run the reweighting for a single file (inputFile) for
 * particular MIPP covariance matrix given in input.xml file. 
 */
void doReweight_dk2nu(const char* inputFile, const char* outputFile, const char* optionsFile){ 
  
  const char* thisDir = getenv("PPFX_DIR");
  const char* OutputDir=thisDir;
 
  std::cout<< "Instance of MakeReweight()" <<std::endl;
  MakeReweight* makerew = MakeReweight::getInstance();
  makerew->SetOptions(optionsFile); 
  
  std::cout<<"Making an output file to store histograms"<<std::endl;
  TFile* fOut = new TFile(outputFile,"recreate");
  std::cout<<"File name: "<<fOut->GetName()<<std::endl;
  
  fOut->mkdir("nom");
  for(int ii=0;ii<Nnuhel;ii++){
    fOut->mkdir(Form("%s_thintarget",nuhel[ii]));
    fOut->mkdir(Form("%s_mippnumi",nuhel[ii]));
    fOut->mkdir(Form("%s_attenuation",nuhel[ii]));
    fOut->mkdir(Form("%s_others",nuhel[ii]));
    fOut->mkdir(Form("%s_total",nuhel[ii]));
  }
  std::cout<<"Done making the output file"<<std::endl;
  
  int Nuniverses = makerew->GetNumberOfUniversesUsed();
  
  TH1D* hnom[Nnuhel];
  TH1D* hcv[Nnuhel];
  TH1D* hthin[Nnuhel][Nuniverses];
  TH1D* hmipp[Nnuhel][Nuniverses];
  TH1D* hatt[Nnuhel][Nuniverses];
  TH1D* hothers[Nnuhel][Nuniverses];
  TH1D* htotal[Nnuhel][Nuniverses];
  
  for(int ii=0;ii<Nnuhel;ii++){
    hnom[ii] = new TH1D(Form("hnom_%s",nuhel[ii]),"",NbinsE,emin,emax);
    hcv[ii]  = new TH1D(Form("hcv_%s",nuhel[ii]),"",NbinsE,emin,emax);
    for(int jj=0;jj< Nuniverses; jj++){
      hthin[ii][jj]   = new TH1D(Form("hthin_%s_%d",  nuhel[ii],jj),"",NbinsE,emin,emax);
      hmipp[ii][jj]   = new TH1D(Form("hmipp_%s_%d",  nuhel[ii],jj),"",NbinsE,emin,emax);
      hatt[ii][jj]    = new TH1D(Form("hatt_%s_%d",   nuhel[ii],jj),"",NbinsE,emin,emax);
      hothers[ii][jj] = new TH1D(Form("hothers_%s_%d",nuhel[ii],jj),"",NbinsE,emin,emax);
      htotal[ii][jj]  = new TH1D(Form("htotal_%s_%d", nuhel[ii],jj),"",NbinsE,emin,emax);
    }
  }
  
  //Loading ntuples:
  TChain* chain_evts   = new TChain("dk2nuTree");  
  TChain* chain_meta   = new TChain("dkmetaTree");  
  bsim::Dk2Nu*  dk2nu  = new bsim::Dk2Nu;  
  bsim::DkMeta* dkmeta = new bsim::DkMeta;

  std::cout<<" Adding ntuple at: "<<inputFile<<std::endl;

  std::string instr= inputFile;
  std::size_t dlmntr = instr.find_last_of("/\\");
  TString ffname= instr.substr(0,dlmntr+1)+std::string("f_")+instr.substr(dlmntr+1);
  TFile friendfile(ffname,"recreate");
  TTree *f_dk2nuTree= new TTree("dk2nuTree","dk2nuTree Friend");

  chain_evts->Add(inputFile);
  chain_evts->SetBranchAddress("dk2nu",&dk2nu);
  int nentries  = chain_evts->GetEntries(); 

  //declaring variables and branches for friend tree
  //TBranch *b_Nuniverses = chain_events->Branch("Nuniverses",&Nuniverses, "Nuniverses/I");//Does this vary across events?
  Double_t cv_wgt, uni_wgt[Nuniverses];
  TString uw_str;
  uw_str.Form("uni_wgt[%d]/D",Nuniverses);
  f_dk2nuTree->Branch("cv_wgt",&cv_wgt,"cv_wgt/D");
  f_dk2nuTree->Branch("uni_wgt",&uni_wgt,uw_str);


  chain_meta->Add(inputFile);
  chain_meta->SetBranchAddress("dkmeta",&dkmeta);
  chain_meta->GetEntry(0); //all entries are the same     
  
  std::vector<double> vwgt_mipp_pi;
  std::vector<double> vwgt_mipp_K;
  std::vector<double> vwgt_abs;
  std::vector<double> vwgt_att;
  std::vector<double> vwgt_ttpCpi;
  std::vector<double> vwgt_ttpCk;
  std::vector<double> vwgt_ttnCpi;
  std::vector<double> vwgt_ttpCnu;
  std::vector<double> vwgt_ttnua;
  std::vector<double> vwgt_ttmesinc;
  std::vector<double> vwgt_oth;
 
  std::cout<<"N of entries: "<<nentries<<std::endl;
  

  for(int ii=0;ii<nentries;ii++){  
    if(ii%1000==0)std::cout<<ii/1000<<" k evts"<<std::endl;
    vwgt_mipp_pi.clear();  
    vwgt_mipp_K.clear();  
    vwgt_abs.clear();  
    vwgt_att.clear();  
    vwgt_ttpCpi.clear();  
    vwgt_ttpCk.clear();  
    vwgt_ttnCpi.clear();  
    vwgt_ttpCnu.clear();  
    vwgt_ttmesinc.clear();
    vwgt_ttnua.clear();  
    vwgt_oth.clear(); 
     
    chain_evts->GetEntry(ii);     
    makerew->calculateWeights(dk2nu,dkmeta);
    
    double fluxWGT = ( (dk2nu->nuray)[1].wgt )*(dk2nu->decay.nimpwt)/3.1416;
    int nuidx = idx_hel(dk2nu->decay.ntype);
    double nuenergy = (dk2nu->nuray)[1].E; 
    
    if(nuidx<0){
      std::cout<<"=> Wrong neutrino file"<<std::endl;
    }

    hnom[nuidx]->Fill(nuenergy,fluxWGT);
    hcv[nuidx]->Fill(nuenergy,fluxWGT*makerew->GetCVWeight());

    vwgt_mipp_pi = makerew->GetWeights("MIPPNumiPionYields");
    vwgt_mipp_K  = makerew->GetWeights("MIPPNumiKaonYields"); 
    vwgt_abs     = makerew->GetWeights("TotalAbsorption");
    vwgt_att     = makerew->GetWeights("TargetAttenuation");
    vwgt_ttpCpi  = makerew->GetWeights("ThinTargetpCPion");
    vwgt_ttpCk   = makerew->GetWeights("ThinTargetpCKaon");
    vwgt_ttnCpi  = makerew->GetWeights("ThinTargetnCPion");
    vwgt_ttpCnu  = makerew->GetWeights("ThinTargetpCNucleon");
    vwgt_ttmesinc= makerew->GetWeights("ThinTargetMesonIncident");
    vwgt_ttnua   = makerew->GetWeights("ThinTargetnucleonA");
    vwgt_oth     = makerew->GetWeights("Other"); 

    cv_wgt=makerew->GetCVWeight();


    for(int jj=0;jj<Nuniverses;jj++){
      double wgt_thin = vwgt_ttpCpi[jj]*vwgt_ttpCk[jj]*vwgt_ttnCpi[jj]*vwgt_ttpCnu[jj]*vwgt_ttmesinc[jj]*vwgt_ttnua[jj];
      double wgt_mipp = vwgt_mipp_pi[jj]*vwgt_mipp_K[jj];
      double wgt_att = vwgt_att[jj]*vwgt_abs[jj];
      hthin[nuidx][jj]->Fill(nuenergy,fluxWGT*wgt_thin);
      hmipp[nuidx][jj]->Fill(nuenergy,fluxWGT*wgt_mipp);
      hatt[nuidx][jj]->Fill(nuenergy,fluxWGT*wgt_att);
      hothers[nuidx][jj]->Fill(nuenergy,fluxWGT*vwgt_oth[jj]);
      htotal[nuidx][jj]->Fill(nuenergy,fluxWGT*wgt_thin*wgt_mipp*wgt_att*vwgt_oth[jj]);

      uni_wgt[jj]=wgt_thin*wgt_mipp*wgt_att*vwgt_oth[jj]; //total weight of this universe
    }

    f_dk2nuTree->Fill();  //

  }
  f_dk2nuTree->Print();//
  friendfile.cd();
  f_dk2nuTree->Write();//

  //Releasing memory:
  makerew->resetInstance();  

  //  chain_evts->AddFriend("dk2nuTree","f_"+inputfile);
  //chain_evts->Draw("cv_wgt");
  std::cout<<"storing general histos"<<std::endl;
  fOut->cd();
  for(int ii=0;ii<Nnuhel;ii++){
    fOut->cd("nom");
    hnom[ii]->Write();
    hcv[ii]->Write();
    for(int jj=0;jj< Nuniverses; jj++){
      fOut->cd(Form("%s_thintarget",nuhel[ii]));  hthin[ii][jj]->Write();
      fOut->cd(Form("%s_mippnumi",nuhel[ii]));    hmipp[ii][jj]->Write();
      fOut->cd(Form("%s_attenuation",nuhel[ii])); hatt[ii][jj]->Write();
      fOut->cd(Form("%s_others",nuhel[ii]));      hothers[ii][jj]->Write();
      fOut->cd(Form("%s_total",nuhel[ii]));       htotal[ii][jj]->Write();      
    }
  }

  std::cout<<"End of run()"<<std::endl;
 

}
  
int idx_hel(int pdgcode){
  int idx = -1;
  if(pdgcode ==  14)idx = 0;
  if(pdgcode == -14)idx = 1;
  if(pdgcode ==  12)idx = 2;
  if(pdgcode == -12)idx = 3;
  return idx;
}

//////////////

#ifndef __CINT__
int main(int argc, const char* argv[]){
  
  doReweight_dk2nu(argv[1],argv[2],argv[3]);
  return 0;
}
#endif
