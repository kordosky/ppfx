
#include "ReweightDriver.h"
#include "InteractionData.h"
#include "TargetData.h"
#include "InteractionChainData.h"
#include "ParameterTable.h"
#include "CentralValuesAndUncertainties.h"
#include "MIPPNumiYieldsBins.h"
#include "HistoContainer.h"
#include "ExtractInfo.h"

#include "nu_g4numi.h"
#include "Numi2Pdg.h"


#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h>

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


namespace flxRW = NeutrinoFluxReweight;

class nu_g4numi;
class Numi2Pdg;

int idx_hel(int pdgdcode);

/*!
 * Run the reweighting for a single file cnent number of events, for a 
 * particular MIPP covariance matrix. 
 *
 * par_option: select the covariance matrix by passing in the name of the xml file.
 * beammode: select the beam configuration, i.e., LE010z185i, this is only used to select the input file.
 * runnumber: select the run number, only used to select the input file
 * cnent: number of events we want to process from the file.
 */
void doRW(const char* par_option,const char* beammode,const char* runnumber, const char* cnent){ 

  const char* ntupleDir = "/minerva/data/flux_hadron_samples/flux/g4numi/v4_test"; 
  const char* thisDir = getenv("PPFX_DIR");
  //  const char* OutputDir = "/minerva/data/users/laliaga/FLUX_PACK/GRID";
  const char* OutputDir=thisDir;
  
  int nent = atoi(cnent);

  int irun = atoi(runnumber);
  //  TFile* fOut = new TFile(Form("%s/%s/%s/fOut_%s_%s_%04d.root",OutputDir,beammode,par_option,par_option,beammode,irun),"recreate");

  std::cout<<"Making an output file to store histograms"<<std::endl;

  TFile* fOut = new TFile(Form("test%d_%dentries_with1000univ.root",irun,nent),"recreate");
  std::cout<<"File name: "<<fOut->GetName()<<std::endl;

  flxRW::HistoContainer* histos =  flxRW::HistoContainer::getInstance();
  flxRW::ExtractInfo*    info   =  flxRW::ExtractInfo::getInstance();
  
  fOut->mkdir("nom");
  fOut->mkdir("nom_mipp");
  for(int ii=0;ii<Nnuhel;ii++){
    fOut->mkdir(Form("%s",nuhel[ii]));
    fOut->mkdir(Form("%s_mipp",nuhel[ii]));
  }
  std::cout<<"Done making the output file"<<std::endl;

  TH1D* hnom[Nnuhel];
  TH1D* hnom_mipp[Nnuhel];
  for(int ii=0;ii<Nnuhel;ii++){
    hnom[ii] = new TH1D(Form("hnom_%s",nuhel[ii]),"",NbinsE,emin,emax);
    hnom_mipp[ii] = new TH1D(Form("hnom_mipp_%s",nuhel[ii]),"",NbinsE,emin,emax);
  }
  std::vector<TH1D*> vh_rw[Nnuhel],vh_mipp_rw[Nnuhel];
  
  for(int ii=0;ii<Nuniverses;ii++){
    for(int jj=0;jj<Nnuhel;jj++){
      vh_rw[jj].push_back(new TH1D(Form("h_rw_%s_%d",nuhel[jj],ii),"",NbinsE,emin,emax));
      vh_mipp_rw[jj].push_back(new TH1D(Form("h_rw_%s_mipp_%d",nuhel[jj],ii),"",NbinsE,emin,emax));
    }
  }
  
  //Loading ntuples:
  TChain* chain_in = new TChain("nudata");
  chain_in->Add(Form("%s/%s/g4numiv4_%s_%04d_0001.root",ntupleDir,beammode,beammode,irun));
  
  nu_g4numi* nu = new nu_g4numi(chain_in);
  int nentries  = nu->nentries;
  int ntrees    = nu->ntrees;
  std::cout<<"Ntrees: "<<ntrees<<", entries: "<<nentries<<std::endl;
  
  //Central Values And Uncertainties:
  flxRW::CentralValuesAndUncertainties* cvu = flxRW::CentralValuesAndUncertainties::getInstance();;
  flxRW::MIPPNumiYieldsBins*  myb =  flxRW::MIPPNumiYieldsBins::getInstance();
                              
  cvu->readFromXML(Form("%s/uncertainties/Parameters_%s.xml",thisDir,par_option));
  myb->readPIP_FromXML(Form("%s/data/BINS/MIPPNumiData_PIP_Bins.xml",thisDir));
  myb->readPIM_FromXML(Form("%s/data/BINS/MIPPNumiData_PIM_Bins.xml",thisDir));
  
  //Classes and variables:
  flxRW::InteractionChainData inter_chain;
  Numi2Pdg* numi2pdg = new Numi2Pdg(); 
  double incP[3];
  double prodP[3];
  double tarP[3];
  
  //Reweighter drivers:
  std::vector<flxRW::ReweightDriver*> vec_rws;
  vec_rws.reserve(Nuniverses);
  
  std::cout<<"Initializing reweight drivers for "<<Nuniverses<<" universes"<<std::endl;
  flxRW::ParameterTable  table_cv;
  flxRW::ParameterTable  table_univ;
  const int base_universe=1000000;
  for(int ii=0;ii<Nuniverses;ii++){
    flxRW::ParameterTable cvPars=cvu->getCVPars();
    flxRW::ParameterTable univPars=cvu->calculateParsForUniverse(ii+base_universe);
    vec_rws.push_back(new flxRW::ReweightDriver(ii,cvPars,univPars));    
  }
  std::cout<<"Done configuring universes"<<std::endl;

  //  for(int ii=0;ii<Nuniverses;ii++){
  //    vec_rws[ii]->SetUniverseID(ii);
  //    vec_rws[ii]->Configure();
  //  }

  std::vector<double> wgts;
  for(int ii=0;ii<Nuniverses;ii++){
    wgts.push_back(1.0);
  }

  //  int red_nentries = int(nentries/1000);  
  std::cout<<"Just running "<<nent<<std::endl;
  
  // for(int ii=0;ii<red_nentries;ii++){  
  for(int ii=0;ii<nent;ii++){  
    //  if(ii%1000==0)std::cout<<ii/1000<<" k evts"<<std::endl;
    std::cout<<"ii "<<ii<<" evts"<<std::endl;
    nu->GetEntry(ii);     
    (inter_chain.interaction_chain).clear();
    
    //Hadron that exits the target:    
    tarP[0] = nu->tpx;
    tarP[1] = nu->tpy;
    tarP[2] = nu->tpz;
    
    flxRW::TargetData tar(tarP,numi2pdg->GetPdg(nu->tptype));
    inter_chain.tar_info = tar;
    
    //Loop over trajectories:
    Int_t ntraj = nu->ntrajectory;
    if(ntraj>10)ntraj = 10;
    
    for(int itraj=0;itraj<(ntraj-1);itraj++){
      
      incP[0] = nu->pprodpx[itraj+1]/1000.;
      incP[1] = nu->pprodpy[itraj+1]/1000.;
      incP[2] = nu->pprodpz[itraj+1]/1000.;
      
      Int_t itraj_prod = itraj + 1;
      Int_t pdg_prod   = nu->pdg[itraj_prod];
      std::string this_proc = std::string(nu->proc[itraj_prod]);
      while( (pdg_prod==221) || (pdg_prod==331) || 
	   (pdg_prod==3212) || (pdg_prod==113) || (pdg_prod==223) ){
	itraj_prod++;
	pdg_prod = nu->pdg[itraj_prod];
      }           
      
      prodP[0] = nu->startpx[itraj_prod]/1000.;
      prodP[1] = nu->startpy[itraj_prod]/1000.;
      prodP[2] = nu->startpz[itraj_prod]/1000.;
      
      flxRW::InteractionData inter(incP,nu->pdg[itraj],prodP,pdg_prod,std::string(nu->fvol[itraj]),this_proc);   
      (inter_chain.interaction_chain).push_back(inter);
    }  
    
    double fluxWGT = (nu->Nimpwt)*(nu->NWtNear[0]);    
    int nuidx = idx_hel(numi2pdg->GetPdg(nu->Ntype));
    if(nuidx>=0)hnom[nuidx]->Fill(nu->NenergyN[0],fluxWGT);
    bool is_mipp = false;    
    
    for(int jj=0;jj<Nuniverses;jj++){
      wgts[jj] = vec_rws[jj]->calculateWeight(inter_chain);
      if(fabs(wgts[jj]-1.0)>1.e-15 && jj==0){is_mipp=true;}
      
      std::map<std::string,double> map_info = info->GetInfo();
      std::map<std::string,double>::iterator it_info;

      double univWGT = fluxWGT*wgts[jj];
      if(nuidx>=0){
	vh_rw[nuidx][jj]->Fill(nu->NenergyN[0],univWGT);
	
	if(is_mipp){
	  vh_mipp_rw[nuidx][jj]->Fill(nu->NenergyN[0],univWGT);
	  
	  for(it_info=map_info.begin();it_info!=map_info.end();it_info++){
	    double checkwgt = fluxWGT*(it_info->second);
	    histos->plot1D(nu->NenergyN[0],it_info->first,"",NbinsE,emin,emax,checkwgt);
	  }
	  
	}
	
      }
      info->Print(std::cout);
      info->CleanInfo(); 
      
    }
    
    if(is_mipp && nuidx>=0)hnom_mipp[nuidx]->Fill(nu->NenergyN[0],fluxWGT);
    
    
  }
  
  std::cout<<"storing general histos"<<std::endl;
  fOut->cd();
  for(int ii=0;ii<Nnuhel;ii++){
    
    fOut->cd("nom");
    hnom[ii]->Write();
    
    fOut->cd("nom_mipp");
    hnom_mipp[ii]->Write();
    
    fOut->cd(Form("%s",nuhel[ii]));
    for(int jj=0;jj<Nuniverses;jj++){
      vh_rw[ii][jj]->Write();
    }

    fOut->cd(Form("%s_mipp",nuhel[ii]));
    for(int jj=0;jj<Nuniverses;jj++){
      vh_mipp_rw[ii][jj]->Write();
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

////////////////////////////////
#ifndef __CINT__
int main(int argc, const char* argv[]){
  

  doRW(argv[1],argv[2],argv[3],argv[4]);
  return 0;
}
#endif
