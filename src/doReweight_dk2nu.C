
#include "HistoContainer.h"
#include "ExtractInfo.h"

#include "dkmeta.h"
#include "dk2nu.h"
#include "MakeReweight.h"

#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <iomanip>

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
void doReweight_dk2nu(const char* inputFile){ 
  
  const char* thisDir = getenv("PPFX_DIR");
  const char* OutputDir=thisDir;
  MakeReweight* makerew = MakeReweight::getInstance();
  makerew->SetOptions("scripts/inputs.xml"); 

  std::cout<<"Making an output file to store histograms"<<std::endl;
  TFile* fOut = new TFile(Form("%s/test.root",OutputDir),"recreate");
  std::cout<<"File name: "<<fOut->GetName()<<std::endl;
  
  HistoContainer* histos =  HistoContainer::getInstance();
  ExtractInfo*    info   =  ExtractInfo::getInstance();
  
  fOut->mkdir("nom");
  fOut->mkdir("nom_corr");
  for(int ii=0;ii<Nnuhel;ii++){
    fOut->mkdir(Form("%s",nuhel[ii]));
    fOut->mkdir(Form("%s_corr",nuhel[ii]));
  }
  std::cout<<"Done making the output file"<<std::endl;
  
  TH1D* hnom[Nnuhel];
  TH1D* hnom_corr[Nnuhel];
  for(int ii=0;ii<Nnuhel;ii++){
    hnom[ii] = new TH1D(Form("hnom_%s",nuhel[ii]),"",NbinsE,emin,emax);
    hnom_corr[ii] = new TH1D(Form("hnom_corr_%s",nuhel[ii]),"",NbinsE,emin,emax);
  }
  std::vector<TH1D*> vh_rw[Nnuhel],vh_corr_rw[Nnuhel];
  
  for(int ii=0;ii< (makerew->GetNumberOfUniversesUsed() );ii++){
    for(int jj=0;jj<Nnuhel;jj++){
      vh_rw[jj].push_back(new TH1D(Form("h_rw_%s_%d",nuhel[jj],ii),"",NbinsE,emin,emax));
      vh_corr_rw[jj].push_back(new TH1D(Form("h_rw_%s_corr_%d",nuhel[jj],ii),"",NbinsE,emin,emax));
    }
  }
  
  //Loading ntuples:
  TChain* chain_evts   = new TChain("dk2nuTree");  
  TChain* chain_meta   = new TChain("dkmetaTree");  
  bsim::Dk2Nu*  dk2nu  = new bsim::Dk2Nu;  
  bsim::DkMeta* dkmeta = new bsim::DkMeta;
  
  std::cout<<" Adding ntuple at: "<<inputFile<<std::endl;

  chain_evts->Add(inputFile);
  chain_evts->SetBranchAddress("dk2nu",&dk2nu);
  int nentries  = chain_evts->GetEntries();
  int ntrees    = chain_evts->GetNtrees();

  chain_meta->Add(inputFile);
  chain_meta->SetBranchAddress("dkmeta",&dkmeta);
  chain_meta->GetEntry(0); //all entries are the same     
  
  std::cout<<"Ntrees: "<<ntrees<<", entries: "<<nentries<<std::endl;

  // for(int ii=0;ii<nentries;ii++){  
  for(int ii=0;ii<10000;ii++){  
    //   if(ii%1000==0)std::cout<<ii/1000<<" k evts"<<std::endl;
    if(ii%10==0)std::cout<<ii<<" evts"<<std::endl;
    chain_evts->GetEntry(ii);     
    makerew->calculateWeights(dk2nu,dkmeta);
    
    double fluxWGT = ( (dk2nu->nuray)[2].wgt )*(dk2nu->decay.nimpwt)/3.1416;
    int nuidx = idx_hel(dk2nu->decay.ntype);
    double nuenergy = (dk2nu->nuray)[2].E; 
    
    if(nuidx>=0)hnom[nuidx]->Fill(nuenergy,fluxWGT);
    
    bool is_corr = false;   
    std::vector<double> wgts = makerew->GetTotalWeights();
    std::vector<double> wgts_mipp_pi   = makerew->GetWeights("MIPPNumiYields");
    std::vector<double> wgts_mipp_K    = makerew->GetWeights("MIPPNumiKaonsYields");
    std::vector<double> wgts_abs_ic    = makerew->GetWeights("AbsorptionIC");
    std::vector<double> wgts_abs_dpip  = makerew->GetWeights("AbsorptionDPIP");
    std::vector<double> wgts_abs_dvol  = makerew->GetWeights("AbsorptionDVOL");
    std::vector<double> wgts_abs_nucl  = makerew->GetWeights("NucleonAbsorptionOutOfTarget");
    std::vector<double> wgts_abs_oth   = makerew->GetWeights("OtherAbsorptionOutOfTarget");
    std::vector<double> wgts_abs_the   = makerew->GetWeights("TheoryThinTarget");
    
    
    for(int jj=0;jj<wgts.size();jj++){
      if(fabs(wgts[jj]-1.0)>1.e-15 && jj==0){is_corr=true;}
      
      std::map<std::string,double> map_info = info->GetInfo();
      std::map<std::string,double>::iterator it_info;

      double univWGT = fluxWGT*wgts[jj];
      if(nuidx>=0){
	vh_rw[nuidx][jj]->Fill(nuenergy,univWGT);
	
	if(is_corr){
	  vh_corr_rw[nuidx][jj]->Fill(nuenergy,univWGT);
	}
	
      }
      info->CleanInfo(); 
      
    }
    if(is_corr && nuidx>=0)hnom_corr[nuidx]->Fill(nuenergy,fluxWGT);
    
  }
  
  std::cout<<"storing general histos"<<std::endl;
  fOut->cd();
  for(int ii=0;ii<Nnuhel;ii++){
    
    fOut->cd("nom");
    hnom[ii]->Write();
    
    fOut->cd("nom_corr");
    hnom_corr[ii]->Write();
    
    fOut->cd(Form("%s",nuhel[ii]));
    for(int jj=0;jj< ( makerew->GetNumberOfUniversesUsed() );jj++){
      vh_rw[ii][jj]->Write();
    }

    fOut->cd(Form("%s_corr",nuhel[ii]));
    for(int jj=0;jj< ( makerew->GetNumberOfUniversesUsed() );jj++){
      vh_corr_rw[ii][jj]->Write();
    }    

  }
  std::cout<<"stored general histos!!"<<std::endl;
   
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
  
  doReweight_dk2nu(argv[1]);
  return 0;
}
#endif
