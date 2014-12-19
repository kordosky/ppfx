
#include "InteractionChainData.h"
#include "Numi2Pdg.h"

namespace NeutrinoFluxReweight{ 
  InteractionChainData::InteractionChainData(){}
  InteractionChainData::InteractionChainData(nu_g4numi* nu){
    
    // Fill in information about the hadron exiting the target
    // this information is needed in order to reweight yields
    // off the target (i.e., MIPP)
    double tarP[3];
    tarP[0] = nu->tpx;
    tarP[1] = nu->tpy;
    tarP[2] = nu->tpz;
    static Numi2Pdg numi2pdg;
    tar_info = TargetData(tarP,numi2pdg.GetPdg(nu->tptype));
    
    // loop over trajectories, create InteractionData objects,
    // and add them to the interaction_chain vector
    Int_t ntraj = nu->ntrajectory;
    if(ntraj>10)ntraj = 10;
    for(int itraj=0;itraj<(ntraj-1);itraj++){
      double incP[3];
      incP[0] = nu->pprodpx[itraj+1]/1000.;
      incP[1] = nu->pprodpy[itraj+1]/1000.;
      incP[2] = nu->pprodpz[itraj+1]/1000.;
      
      Int_t itraj_prod = itraj + 1;
      Int_t pdg_prod   = nu->pdg[itraj_prod];
      std::string this_proc = std::string(nu->proc[itraj_prod]);
      // skip over etas and other swiftly decaying particles
      // we are interested in their daughters
      while( (pdg_prod==221) || (pdg_prod==331) || 
	   (pdg_prod==3212) || (pdg_prod==113) || (pdg_prod==223) ){
	itraj_prod++;
	pdg_prod = nu->pdg[itraj_prod];
      }           
      double prodP[3];
      prodP[0] = nu->startpx[itraj_prod]/1000.;
      prodP[1] = nu->startpy[itraj_prod]/1000.;
      prodP[2] = nu->startpz[itraj_prod]/1000.;
      
      double vtx[3];
      vtx[0]=nu->startx[itraj_prod];
      vtx[1]=nu->starty[itraj_prod];
      vtx[2]=nu->startz[itraj_prod];

      InteractionData inter(incP,nu->pdg[itraj],prodP,pdg_prod,std::string(nu->fvol[itraj]),this_proc,vtx);   
      interaction_chain.push_back(inter);
    }
    

  }
  
}
