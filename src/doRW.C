
#include "ReweightDriver.h"
#include "InteractionData.h"
#include "TargetData.h"
#include "ParticlesThroughVolumesData.h"
#include "InteractionChainData.h"
#include "ParameterTable.h"
#include "CentralValuesAndUncertainties.h"
#include "MIPPNumiYieldsBins.h"
#include "HistoContainer.h"
#include "ExtractInfo.h"

#include "nu_g4numi.h"
#include "Numi2Pdg.h"

#include "dkmeta.h"
#include "dk2nu.h"

#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <iomanip>

//Some constants::

const int Nuniverses = 100;

const int NbinsE = 120;
const double emin   =   0.;
const double emax   = 120.;

const int Nnuhel = 4;
const char* nuhel[Nnuhel] = {"numu","numubar","nue","nuebar"};

//for checking:
const int Nbins_MIPP_PIP = 124;
const int Nbins_MIPP_PIM = 119;


//namespace flxRW = NeutrinoFluxReweight;
using namespace NeutrinoFluxReweight;

class nu_g4numi;
class Numi2Pdg;

int idx_hel(int pdgdcode);
bool check_inputs(const char* input_rew,const char* input_format,const char* par_option,const char* beammode,const char* runnumber, const char* cnent);
void print_usage();

/*!
 * Run the reweighting for a single file cnent number of events, for a 
 * particular MIPP covariance matrix. 
 *
 * input_format: select between dk2nu and nudata ntuple formats
 * par_option: select the covariance matrix by passing in the name of the xml file.
 * beammode: select the beam configuration, i.e., LE010z185i, this is only used to select the input file.
 * runnumber: select the run number, only used to select the input file
 * cnent: number of events we want to process from the file.
 */
void doRW(const char* input_rew,const char* input_format,const char* par_option,const char* beammode,const char* runnumber, const char* cnent){ 
  
  if(!check_inputs(input_rew,input_format,par_option,beammode,runnumber,cnent)){
    exit(1);
  }

  const char* ntupleDir = "/minerva/data/flux_hadron_samples/flux/g4numi/v4_test"; 
  //this is temporary because these are the ntuples with the new variables: 
  const char* tmp_dk2nuDir = "/minerva/data/users/laliaga/flux/TESTVOL/MOD"; 
  const char* thisDir = getenv("PPFX_DIR");
  //  const char* OutputDir = "/minerva/data/users/laliaga/FLUX_PACK/GRID";
  const char* OutputDir=thisDir;
  
  int nent = atoi(cnent);
  int irun = atoi(runnumber);
  bool is_dk2nu = strcmp(input_format,"dk2nu")==0;
  //  TFile* fOut = new TFile(Form("%s/%s/%s/fOut_%s_%s_%04d.root",OutputDir,beammode,par_option,par_option,beammode,irun),"recreate");

  std::cout<<"Making an output file to store histograms"<<std::endl;

  TFile* fOut = new TFile(Form("test_%s_%s_%s_%s_%s.root",input_format,par_option,beammode,runnumber,cnent),"recreate");
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
  
  for(int ii=0;ii<Nuniverses;ii++){
    for(int jj=0;jj<Nnuhel;jj++){
      vh_rw[jj].push_back(new TH1D(Form("h_rw_%s_%d",nuhel[jj],ii),"",NbinsE,emin,emax));
      vh_corr_rw[jj].push_back(new TH1D(Form("h_rw_%s_corr_%d",nuhel[jj],ii),"",NbinsE,emin,emax));
    }
  }
  
  //Loading ntuples:
  TChain* chain_evts;
  TChain* chain_meta;
  nu_g4numi* nu;
  bsim::Dk2Nu*  dk2nu;
  bsim::DkMeta*  dkmeta;
  int nentries  = 0;
  int ntrees    = 0;

  if(!is_dk2nu){
    chain_evts = new TChain("nudata");
    chain_evts->Add(Form("%s/%s/g4numiv4_%s_%04d_0001.root",ntupleDir,beammode,beammode,irun));
    nu = new nu_g4numi(chain_evts);
    std::cout<<" Adding ntuple at: "<< Form("%s/%s/g4numiv4_%s_%04d_0001.root",ntupleDir,beammode,beammode,irun)<<std::endl;
    nentries  = nu->nentries;
    ntrees    = nu->ntrees;
  }
  else{
    chain_evts = new TChain("dk2nuTree");  
    std::cout<<" Adding ntuple at: "<<Form("%s/g4numiv5_minerva1_%s_%d_0005.root",tmp_dk2nuDir,beammode,irun) <<std::endl;
    chain_evts->Add(Form("%s/g4numiv5_minerva1_%s_%d_0005.root",tmp_dk2nuDir,beammode,irun));
    dk2nu  = new bsim::Dk2Nu;   
    chain_evts->SetBranchAddress("dk2nu",&dk2nu);
    nentries  = chain_evts->GetEntries();
    ntrees    = chain_evts->GetNtrees();

    chain_meta = new TChain("dkmetaTree");  
    chain_meta->Add(Form("%s/g4numiv5_minerva1_%s_%d_0005.root",tmp_dk2nuDir,beammode,irun));
    dkmeta  = new bsim::DkMeta;
    chain_meta->SetBranchAddress("dkmeta",&dkmeta);
    chain_meta->GetEntry(0); //all entries are the same     
  }

  std::cout<<"Ntrees: "<<ntrees<<", entries: "<<nentries<<std::endl;

  
  //Central Values And Uncertainties:
  CentralValuesAndUncertainties* cvu = CentralValuesAndUncertainties::getInstance();;
  MIPPNumiYieldsBins*  myb =  MIPPNumiYieldsBins::getInstance();
                              
  cvu->readFromXML(Form("%s/uncertainties/Parameters_%s.xml",thisDir,par_option));
  myb->pip_data_from_xml(Form("%s/data/BINS/MIPPNumiData_PIP_Bins.xml",thisDir));
  myb->pim_data_from_xml(Form("%s/data/BINS/MIPPNumiData_PIM_Bins.xml",thisDir));
  
  //Classes and variables:
  Numi2Pdg* numi2pdg = new Numi2Pdg(); 

  //Reweighter drivers:
  std::vector<ReweightDriver*> vec_rws;
  vec_rws.reserve(Nuniverses);
  
  std::cout<<"Initializing reweight drivers for "<<Nuniverses<<" universes"<<std::endl;
  ParameterTable  table_cv;
  ParameterTable  table_univ;
  const int base_universe=1000000;
  for(int ii=0;ii<Nuniverses;ii++){
    ParameterTable cvPars=cvu->getCVPars();
    ParameterTable univPars=cvu->calculateParsForUniverse(ii+base_universe);
    vec_rws.push_back(new ReweightDriver(ii,cvPars,univPars,std::string(input_rew)));    
  }
  std::cout<<"Done configuring universes"<<std::endl;
  
  std::vector<double> wgts;
  for(int ii=0;ii<Nuniverses;ii++){
    wgts.push_back(1.0);
  }
  
  if(nent==-1)nent=nentries;
  std::cout<<"Running over "<<nent<<" entries"<<std::endl;
  
  // for(int ii=0;ii<red_nentries;ii++){  
  for(int ii=0;ii<nent;ii++){  
     if(ii%1000==0)std::cout<<ii/1000<<" k evts"<<std::endl;
    //  std::cout<<"On entry "<<ii<<std::endl;
    if(!is_dk2nu)nu->GetEntry(ii);     
    else{
      chain_evts->GetEntry(ii);     
    }
    // create an interaction chain from the data record
    InteractionChainData* inter_chain;
    if(!is_dk2nu)inter_chain = new InteractionChainData(nu,"le010z","185i");
    else inter_chain = new InteractionChainData(dk2nu,dkmeta);
    
    //inter_chain->print(std::cout);
        
    // histogram the flux, without any reweighting
    double fluxWGT = 1.0;
    if(!is_dk2nu)fluxWGT = (nu->Nimpwt)*(nu->NWtNear[0])/3.1416;    
    else fluxWGT = ( (dk2nu->nuray)[2].wgt )*(dk2nu->decay.nimpwt)/3.1416;
    
    int nuidx = -1;
    double nuenergy = -1.0;
    if(!is_dk2nu){
      nuidx = idx_hel(numi2pdg->GetPdg(nu->Ntype));
      nuenergy = nu->NenergyN[0];
    }
    else{
      nuidx = idx_hel(dk2nu->decay.ntype);
      nuenergy = (dk2nu->nuray)[2].E; 
    }
    
    if(nuidx>=0)hnom[nuidx]->Fill(nuenergy,fluxWGT);
    
    
    bool is_corr = false;        
    for(int jj=0;jj<Nuniverses;jj++){
      wgts[jj] = vec_rws[jj]->calculateWeight(*inter_chain);
      if(fabs(wgts[jj]-1.0)>1.e-15 && jj==0){is_corr=true;}
      
      std::map<std::string,double> map_info = info->GetInfo();
      std::map<std::string,double>::iterator it_info;

      double univWGT = fluxWGT*wgts[jj];
      if(nuidx>=0){
	vh_rw[nuidx][jj]->Fill(nuenergy,univWGT);
	
	if(is_corr){
	  vh_corr_rw[nuidx][jj]->Fill(nuenergy,univWGT);
	  
	  for(it_info=map_info.begin();it_info!=map_info.end();it_info++){
	    double checkwgt = fluxWGT*(it_info->second);
	    histos->plot1D(nuenergy,it_info->first,"",NbinsE,emin,emax,checkwgt);
	  }
	  
	}
	
      }
      //      info->Print(std::cout);
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
    for(int jj=0;jj<Nuniverses;jj++){
      vh_rw[ii][jj]->Write();
    }

    fOut->cd(Form("%s_corr",nuhel[ii]));
    for(int jj=0;jj<Nuniverses;jj++){
      vh_corr_rw[ii][jj]->Write();
    }    

  }
  std::cout<<"stored general histos!!"<<std::endl;
  
  std::map<std::string,TH1D*> mapHIST = histos->Get1DHistos();
  std::map<std::string,TH1D*>::iterator hit = mapHIST.begin();


  std::string WGTMippTagUniv = "wgt_MIPP_NuMI";
  std::string UNIV = "univ";
  
  fOut->mkdir(WGTMippTagUniv.c_str());
  fOut->mkdir(UNIV.c_str());
  /*  
  TDirectory* dir_univ = fOut->mkdir(UNIV.c_str());
  dir_univ->cd();
   for(int ii=0;ii<124;ii++){
     dir_univ->mkdir(Form("BIN_MIPP_PIP_%d",ii));
   }
   for(int ii=0;ii<119;ii++){
     dir_univ->mkdir(Form("BIN_MIPP_PIM_%d",ii));
   }
  */

  //   std::cout<<"writing general histos!!"<<std::endl;
   for(;hit!=mapHIST.end();hit++){
     //std::cout<<"storing: "<<(hit->first)<<std::endl;
     if( (hit->first).find(WGTMippTagUniv) == 0 ){
       fOut->cd(WGTMippTagUniv.c_str());
       (hit->second)->Write();
     }
     if( (hit->first).find(UNIV) == 0 ){
       fOut->cd(UNIV.c_str());
       (hit->second)->Write();

       /*
       int npos_ = (hit->first).rfind("_");
       std::string mipp_bin = (hit->first).substr(npos_+1,(hit->first).size());
       char cbin[3];sprintf(cbin,"%s",mipp_bin.c_str());
       if((hit->first).find("pip")<50){
	 dir_univ->cd(Form("BIN_MIPP_PIP_%s",cbin));	
	 (hit->second)->Write();
       }
       if((hit->first).find("pim")<50){
	 dir_univ->cd(Form("BIN_MIPP_PIM_%s",cbin));	
	 (hit->second)->Write();
       }
       */
       
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

void print_usage(){
  std::cout<<"bin/doRW [input_flags] [input_format] [par_option] [beammode] [runnumber] [cnent]" <<std::endl;
  std::cout<<"===================================================================" <<std::endl;
  std::cout<<"input_flags : a xml file to know which reweither turn on"<<std::endl;
  std::cout<<"input_format: dk2nu or nudata"<<std::endl;
  std::cout<<"par_option  : noCorr, low, ... look at the uncertainty directory"<<std::endl;
  std::cout<<"beammode    : le010z185i or le010z-185i"<<std::endl;
  std::cout<<"runnumber   : [0-499]"<<std::endl;
  std::cout<<"cnent       : [>=0]"<<std::endl;  
}

bool check_inputs(const char* input_rew,const char* input_format,const char* par_option,const char* beammode,const char* runnumber, const char* cnent){

  //Input reweighter flags:
  bool good_flags = false;
  std::string sinput = std::string(input_rew);
  if(sinput.find(".xml")<200)good_flags = true;
  
  //input_format:
  bool good_format = false;
  if(strcmp(input_format,"dk2nu")==0 || strcmp(input_format,"nudata")==0)good_format = true;

  //par_option:
  bool good_par = false;
  if(strcmp(par_option,"noCorr")==0 || strcmp(par_option,"low")==0 )        good_par = true;
  if(strcmp(par_option,"medium")==0 || strcmp(par_option,"mediumBYDET")==0) good_par = true;
  if(strcmp(par_option,"high")==0   || strcmp(par_option,"highBYDET")==0)   good_par = true;
  if(strcmp(par_option,"uhigh")==0  || strcmp(par_option,"uhighBYDET")==0)  good_par = true;
  
  bool good_bmode = false;
  if(strcmp(beammode,"le010z185i")==0 || strcmp(beammode,"le010z-185i")==0)good_bmode = true;
  
  bool good_runN = false;
  if(atoi(runnumber)>=0 && atoi(runnumber)<500)good_runN = true;
  
  bool good_nent = false;
  if(atoi(cnent)>=-1)good_nent = true;
  
  bool good_args = good_flags && good_format && good_par && good_bmode && good_runN && good_nent;
  if(!good_args){
    std::cout<<"No good arguments"<<std::endl;
    print_usage();
  }
  return good_args;
  
  
}

////////////////////////////////
#ifndef __CINT__
int main(int argc, const char* argv[]){
  
  if(argc!=7){
    std::cout<<"We need "<<argc-1<<" arguments to execute this script. Check the help menu:"<<std::endl;
    print_usage();
    exit (1);
  }
  doRW(argv[1],argv[2],argv[3],argv[4],argv[5],argv[6]);
  return 0;
}
#endif
