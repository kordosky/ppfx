
#include "InteractionChainData.h"
#include "Numi2Pdg.h"

namespace NeutrinoFluxReweight{ 
  InteractionChainData::InteractionChainData(){}
  InteractionChainData::InteractionChainData(nu_g4numi* nu, 
					     const char* tgtcfg,
					     const char* horncfg){
    
    // Fill in information about the hadron exiting the target
    // this information is needed in order to reweight yields
    // off the target (i.e., MIPP)
    double tarP[3];
    tarP[0] = nu->tpx;
    tarP[1] = nu->tpy;
    tarP[2] = nu->tpz;
    double tarV[3];
    tarV[0] = nu->tvx;
    tarV[1] = nu->tvy;
    tarV[2] = nu->tvz;

    static Numi2Pdg numi2pdg;
    tar_info = TargetData(tarP,numi2pdg.GetPdg(nu->tptype),tarV);
    
    // loop over trajectories, create InteractionData objects,
    // and add them to the interaction_chain vector    
    //(Note about units: In nudata format, the momentum unit is MeV)
    
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
    
    //
    //nothing to fill for ParticlesThroughVolumesData here. If we are using nudata format
    //the size of the vector of ParticlesThroughVolumesData objects will be zero.
    //    

    target_config=tgtcfg;
    horn_config=horncfg;
  }
  
  InteractionChainData::InteractionChainData(bsim::Dk2Nu* nu, 
					     bsim::DkMeta* meta){

    // Fill in information about the hadron exiting the target
    // this information is needed in order to reweight yields
    // off the target (i.e., MIPP)
    double tarP[3];
    tarP[0]=nu->tgtexit.tpx;
    tarP[1]=nu->tgtexit.tpy;
    tarP[2]=nu->tgtexit.tpz;
    double tarV[3];
    tarV[0]=nu->tgtexit.tvx;
    tarV[1]=nu->tgtexit.tvy;
    tarV[2]=nu->tgtexit.tvz;

    // static Numi2Pdg numi2pdg; 
    // note: in dk2nu, all pids are store in pdg code. 
    tar_info = TargetData(tarP,nu->tgtexit.tptype,tarV);

    
    // loop over trajectories, create InteractionData objects,
    // and add them to the interaction_chain vector
    // Note about units: In dk2nu format, the momentum unit is GeV

    Int_t ntraj = nu->ancestor.size();
    for(int itraj=0;itraj<(ntraj-1);itraj++){
      double incP[3];
      incP[0] = nu->ancestor[itraj+1].pprodpx;
      incP[1] = nu->ancestor[itraj+1].pprodpy;
      incP[2] = nu->ancestor[itraj+1].pprodpz;

      Int_t itraj_prod = itraj + 1;
      Int_t pdg_prod   = nu->ancestor[itraj_prod].pdg;
      std::string this_proc = std::string(nu->ancestor[itraj_prod].proc);
      
      // skip over etas and other swiftly decaying particles
      // we are interested in their daughters
      while( (pdg_prod==221) || (pdg_prod==331) || 
	   (pdg_prod==3212) || (pdg_prod==113) || (pdg_prod==223) ){
	itraj_prod++;
	pdg_prod = nu->ancestor[itraj_prod].pdg;
      }           
      double prodP[3];
      prodP[0] = nu->ancestor[itraj_prod].startpx;
      prodP[1] = nu->ancestor[itraj_prod].startpy;
      prodP[2] = nu->ancestor[itraj_prod].startpz;
      
      double vtx[3];
      vtx[0]=nu->ancestor[itraj_prod].startx;
      vtx[1]=nu->ancestor[itraj_prod].starty;
      vtx[2]=nu->ancestor[itraj_prod].startz;
      std::string this_vol=nu->ancestor[itraj].ivol;
      int pdg_inc=nu->ancestor[itraj].pdg;
      InteractionData inter(incP,pdg_inc,prodP,pdg_prod,
			    this_vol,this_proc,vtx);   
      interaction_chain.push_back(inter);
      
    }// end loop over trajectories
   
    //Filling here the ParticlesThroughVolumesData info:
    ptv_info.clear();
    //Looking IC, DPIP and DVOL:
    int pdgs[3];
    double moms[3];
    double amount_IC[3],amount_DPIP[3],amount_DVOL[3];
    for(int ii=0;ii<3;ii++){
      pdgs[ii] = 0; moms[ii] = 0; 
      amount_IC[ii] = 0; amount_DPIP[ii] = 0; amount_DVOL[ii] = 0;
    }
    
    for(int ii=0;ii<3;ii++){
      if(nu->ancestor.size()==3 && ii==2)continue;
      pdgs[ii] = nu->ancestor[nu->ancestor.size()-ii-2].pdg;
      moms[ii] = sqrt(pow(nu->ancestor[nu->ancestor.size()-ii-2].startpx,2)+
			pow(nu->ancestor[nu->ancestor.size()-ii-2].startpy,2)+
			pow(nu->ancestor[nu->ancestor.size()-ii-2].startpz,2));
      
      //Amounts:
      //control:
      if( (nu->vdbl)[ii] <0 || (nu->vdbl)[ii+3] <0 || (nu->vdbl)[ii+6] <0 || (nu->vdbl)[ii+9] <0){
	std::cout<< "ERROR FILLING AMOUNT OF MATERIAL CROSSED (In InteractionChainData) !!!" <<std::endl;
      }
      amount_IC[ii]   = (nu->vdbl)[ii] + (nu->vdbl)[ii+3];
      amount_DPIP[ii] = (nu->vdbl)[ii+6];
      amount_DVOL[ii] = (nu->vdbl)[ii+9];
    }
    
    ParticlesThroughVolumesData tmp_ptv_IC(pdgs,amount_IC,moms,"IC");
    ParticlesThroughVolumesData tmp_ptv_DPIP(pdgs,amount_DPIP,moms,"DPIP");
    ParticlesThroughVolumesData tmp_ptv_DVOL(pdgs,amount_DVOL,moms,"DVOL");
    
    ptv_info.push_back(tmp_ptv_IC);
    ptv_info.push_back(tmp_ptv_DPIP);
    ptv_info.push_back(tmp_ptv_DVOL);
    ///
    
    target_config=meta->tgtcfg;
    horn_config=meta->horncfg;
    
  }

  std::ostream& InteractionChainData::print(std::ostream& os) const{
    using namespace std;
    os<<"==== InteractionChainData ====\n"
      <<" *config* "<<target_config<<" "<<horn_config
      <<"\n *target info*\n  ";
    tar_info.print(os);
    os<<"\n *ancestors*\n";
    for(int i=0; i<interaction_chain.size(); i++){
      os<<"   ";
      interaction_chain[i].print(os);
    }
    os<<"\n *Particlethrough volumes:*\n";
    for(int i=0; i<ptv_info.size(); i++){
      os<<"   ";
      ptv_info[i].print(os);
    }
    os<<endl;
    return os;
  }

}
