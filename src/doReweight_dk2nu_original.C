
#include "dk2nu/tree/dkmeta.h"
#include "dk2nu/tree/dk2nu.h"
#include "MakeReweight.h"
#include "NuWeight.h"

#include <string>
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
const char* nulabel[Nnuhel] = {"#nu_{#mu}","#bar#nu_{#mu}","#nu_{e}","#bar#nu_{e}"};

using namespace NeutrinoFluxReweight;

class nu_g4numi;
class Numi2Pdg;

int idx_hel(int pdgdcode);

/*!
 * Run the reweighting for a single file (inputFile) for
 * particular MIPP covariance matrix given in input.xml file. 
 */
void doReweight_dk2nu(const char* inputFile, const char* outputFile, const char* optionsFile, const char* cxxdet, const char* cyydet, const char* czzdet){ 
  
  const char* thisDir = getenv("PPFX_DIR");
  int idet = -1;
  bool doing_precalculated_pos = false;
  
  NeutrinoFluxAuxiliar::NuWeight* nuweight;			  
  if( std::string(cyydet) == "none" && std::string(czzdet) == "none" ){
    idet = atoi(cxxdet);
    doing_precalculated_pos = true;
  }
  else{
    std::vector<double> vdet;
    double xxdet = atof(cxxdet); vdet.push_back(xxdet);
    double yydet = atof(cyydet); vdet.push_back(yydet);
    double zzdet = atof(czzdet); vdet.push_back(zzdet);
    nuweight = new NeutrinoFluxAuxiliar::NuWeight(vdet);
  }
  
  std::cout<< "Instance of MakeReweight()" <<std::endl;
  MakeReweight* makerew = MakeReweight::getInstance();
  makerew->SetOptions(optionsFile); 
  
  std::cout<<"Making an output file to store histograms"<<std::endl;
  TFile* fOut = new TFile(outputFile,"recreate");
  std::cout<<"File name: "<<fOut->GetName()<<std::endl;
  
  int Nuniverses = makerew->GetNumberOfUniversesUsed();
  
  TH1D* hnom[Nnuhel];
  TH1D* hcv[Nnuhel];
  TH1D* hthin[Nnuhel][Nuniverses];
  TH1D* hmipp[Nnuhel][Nuniverses];
  TH1D* hatt[Nnuhel][Nuniverses];
  TH1D* hothers[Nnuhel][Nuniverses];
  TH1D* htotal[Nnuhel][Nuniverses];
  
  for(int ii=0;ii<Nnuhel;ii++){
    const TString xtitle = "E_{#nu} [GeV]";
    const TString ytitle = "##nu_{unoscillated} [m^{-2}]";
    hnom[ii] = new TH1D(Form("hnom_%s",nuhel[ii]), Form("Uncorrected %s flux",nulabel[ii]), NbinsE,emin,emax);
    hnom[ii]->SetXTitle(xtitle);
    hnom[ii]->SetYTitle(ytitle);

    hcv[ii]  = new TH1D(Form("hcv_%s",nuhel[ii]), Form("Fully PPFX-corrected %s flux (central value)", nulabel[ii]),NbinsE,emin,emax);
    hcv[ii]->SetXTitle(xtitle);
    hcv[ii]->SetYTitle(ytitle);


    for(int jj=0;jj< Nuniverses; jj++){
      hthin[ii][jj]   = new TH1D(Form("hthin_%s_%d",  nuhel[ii],jj),Form("%s flux corrected based on thin target data, univ. #%i", nulabel[ii], jj), NbinsE,emin,emax);
      hthin[ii][jj]->SetXTitle(xtitle);
      hthin[ii][jj]->SetYTitle(ytitle);

      hmipp[ii][jj]   = new TH1D(Form("hmipp_%s_%d",  nuhel[ii],jj),Form("%s flux corrected based on MIPP NuMI target data, univ. #%i", nulabel[ii], jj),NbinsE,emin,emax);
      hmipp[ii][jj]->SetXTitle(xtitle);
      hmipp[ii][jj]->SetYTitle(ytitle);

      hatt[ii][jj]    = new TH1D(Form("hatt_%s_%d",   nuhel[ii],jj),Form("%s flux corrected for attenuation in target, univ. #%i", nulabel[ii], jj),NbinsE,emin,emax);
      hatt[ii][jj]->SetXTitle(xtitle);
      hatt[ii][jj]->SetYTitle(ytitle);

      hothers[ii][jj] = new TH1D(Form("hothers_%s_%d",nuhel[ii],jj),Form("%s flux corrected for other effects, univ. #%i", nulabel[ii], jj),NbinsE,emin,emax);
      hothers[ii][jj]->SetXTitle(xtitle);
      hothers[ii][jj]->SetYTitle(ytitle);

      htotal[ii][jj]  = new TH1D(Form("htotal_%s_%d", nuhel[ii],jj),Form("%s flux corrected for all effects, univ. #%i", nulabel[ii], jj),NbinsE,emin,emax);
      htotal[ii][jj]->SetXTitle(xtitle);
      htotal[ii][jj]->SetYTitle(ytitle);
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

  chain_meta->Add(inputFile);
  chain_meta->SetBranchAddress("dkmeta",&dkmeta);
  chain_meta->GetEntry(0); //all entries are the same     
  
  std::string detname = "UserPosition";
  if(doing_precalculated_pos){
    detname = (dkmeta->location)[idet].name;
  }
  std::cout<<"=> Doing the analysis for: "<< detname <<std::endl; 

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
  double fluxWGT  = 0;
  double nuenergy = 0;

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
    if(doing_precalculated_pos){
      fluxWGT = ( (dk2nu->nuray)[idet].wgt )*(dk2nu->decay.nimpwt)/3.1416;
      nuenergy = (dk2nu->nuray)[idet].E; 
    }
    else{
      nuweight->calculate_weight(dk2nu);
      fluxWGT  = (nuweight->wgt)*(dk2nu->decay.nimpwt)/3.1416;
      nuenergy = nuweight->enu;
    }

    int nuidx = idx_hel(dk2nu->decay.ntype);
        
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

    
    for(int jj=0;jj<Nuniverses;jj++){
      double wgt_thin = vwgt_ttpCpi[jj]*vwgt_ttpCk[jj]*vwgt_ttnCpi[jj]*vwgt_ttpCnu[jj]*vwgt_ttmesinc[jj]*vwgt_ttnua[jj];
      double wgt_mipp = vwgt_mipp_pi[jj]*vwgt_mipp_K[jj];
      double wgt_att = vwgt_att[jj]*vwgt_abs[jj];
      hthin[nuidx][jj]->Fill(nuenergy,fluxWGT*wgt_thin);
      hmipp[nuidx][jj]->Fill(nuenergy,fluxWGT*wgt_mipp);
      hatt[nuidx][jj]->Fill(nuenergy,fluxWGT*wgt_att);
      hothers[nuidx][jj]->Fill(nuenergy,fluxWGT*vwgt_oth[jj]);
      htotal[nuidx][jj]->Fill(nuenergy,fluxWGT*wgt_thin*wgt_mipp*wgt_att*vwgt_oth[jj]);

    }
  }

  std::cout<<"storing general histos"<<std::endl;
  fOut->cd();
  
  fOut->mkdir("nom");
  for(int ii=0;ii<Nnuhel;ii++){
    fOut->mkdir(Form("%s_thintarget",nuhel[ii]));
    fOut->mkdir(Form("%s_mippnumi",nuhel[ii]));
    fOut->mkdir(Form("%s_attenuation",nuhel[ii]));
    fOut->mkdir(Form("%s_others",nuhel[ii]));
    fOut->mkdir(Form("%s_total",nuhel[ii]));
  }
  
  for(int ii=0;ii<Nnuhel;ii++){
    fOut->cd("nom");
    hnom[ii]->Write();
    hcv[ii]->Write();
    for(int jj=0;jj< Nuniverses; jj++){
      fOut->cd(Form("%s_thintarget" ,nuhel[ii]));  hthin[ii][jj]->Write();
      fOut->cd(Form("%s_mippnumi"   ,nuhel[ii]));  hmipp[ii][jj]->Write();
      fOut->cd(Form("%s_attenuation",nuhel[ii]));  hatt[ii][jj]->Write();
      fOut->cd(Form("%s_others"     ,nuhel[ii]));  hothers[ii][jj]->Write();
      fOut->cd(Form("%s_total"      ,nuhel[ii]));  htotal[ii][jj]->Write();      
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
  if(pdgcode ==  12)idx = 2;
  if(pdgcode == -12)idx = 3;
  return idx;
}
void usage(){
  std::cout<<"This script calculates the flux at one position in the NuMI beamline: "<<std::endl;
  std::cout<<"Using a precalculated detector positions:"<<std::endl;
  std::cout<<"  bin/doReweight_dk2nu [inputFile] [outputFile] [optionsFile] [idet]"<<std::endl;
  std::cout<<"Using a user input position:"<<std::endl;
  std::cout<<"  bin/doReweight_dk2nu [inputFile] [outputFile] [optionsFile] [xpos] [ypos] [zpos]"<<std::endl;
  std::cout<<"  "<<std::endl;
  std::cout<<"Inputs  "<<std::endl;
  std::cout<<"[inputFile] : g4numi ntuple in dk2nu/dkmeta format (v6 minerva branch is recommended)"<<std::endl;
  std::cout<<"[outputFile] : user definied output file name."<<std::endl;
  std::cout<<"[optionsFile] :xml file with the ppfx input parameters (look at ${PPFX_DIR}/script/input_default.xml)"<<std::endl;
  std::cout<<"[idet] : index of the precalculated detector (look at the location.name in the dkmeta tree of the g4numi ntuple)"<<std::endl;
  std::cout<<"[xpos], [ypos], [zpos] : position (cm) respect to the MC NuMI coordinate system to calculate the flux"<<std::endl;
  std::cout<<"  "<<std::endl;
}
//////////////

#ifndef __CINT__
int main(int argc, const char* argv[]){

  if(argc==5){   
    doReweight_dk2nu(argv[1],argv[2],argv[3],argv[4],"none","none");    
  }
  else if(argc==7){
    doReweight_dk2nu(argv[1],argv[2],argv[3],argv[4],argv[5],argv[6]);
  }
  else{
    usage();
    exit (1);
    }
  
  return 0;
}
#endif
