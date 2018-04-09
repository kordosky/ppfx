
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
    tar_info = TargetData(tarP,numi2pdg.GetPdg(nu->tptype),tarV,-1);
    
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
 
      //It seems that the next while is causing seg fault in gcc 6.3:
      /*  while( is_fast_decay(pdg_prod) ){
	  itraj_prod++;
	  pdg_prod = nu->pdg[itraj_prod];
	  } */   
      if( is_fast_decay(pdg_prod) ){
	for(int ifast = itraj_prod + 1; ifast < (ntraj-1); ifast++ ){
	  pdg_prod = nu->pdg[ifast];
	  itraj_prod++;
	  pdg_prod = nu->pdg[itraj_prod];
	  if( !is_fast_decay(pdg_prod) ) break;	  
	}
      }        
      double prodP[3];
      prodP[0] = nu->startpx[itraj_prod]/1000.;
      prodP[1] = nu->startpy[itraj_prod]/1000.;
      prodP[2] = nu->startpz[itraj_prod]/1000.;
      
      double vtx[3];
      vtx[0]=nu->startx[itraj_prod];
      vtx[1]=nu->starty[itraj_prod];
      vtx[2]=nu->startz[itraj_prod];

      InteractionData inter(itraj, incP,nu->pdg[itraj],prodP,pdg_prod,std::string(nu->fvol[itraj]),this_proc,vtx);   
      interaction_chain.push_back(inter);
    }
    
    //
    //nothing to fill for ParticlesThroughVolumesData here. If we are using nudata format
    //the size of the vector of ParticlesThroughVolumesData objects will be zero.
    //    

    target_config=tgtcfg;
    horn_config=horncfg;
    playlist = -1;
  }
  
  InteractionChainData::InteractionChainData(bsim::Dk2Nu* nu, 
					     bsim::DkMeta* meta){
    
    // Fill in information about the hadron exiting the target
    // this information is needed in order to reweight yields
    // off the target (i.e., MIPP)
    std::string mode(getenv("MODE"));
    //if(mode=="OPT")std::cout<<"MODE IS OPT interactionchaindata "<<std::endl;
    //else std::cout<<"MODE not recognized. InteractionChainData "<<std::endl;
    
   // std::cout<<" The environment is "<<getenv("MODE")<<std::endl;
    double tarP[3];
    tarP[0]=nu->tgtexit.tpx;
    tarP[1]=nu->tgtexit.tpy;
    tarP[2]=nu->tgtexit.tpz;
    double tarV[3];
    tarV[0]=nu->tgtexit.tvx;
    tarV[1]=nu->tgtexit.tvy;
    tarV[2]=nu->tgtexit.tvz;
    int Nskip = 0;
    //we will fill tardata after looking into the ancestry for the right index.
    
    // loop over trajectories, create InteractionData objects,
    // and add them to the interaction_chain vector
    // Note about units: In dk2nu format, the momentum unit is GeV

    Int_t ntraj = nu->ancestor.size();
    for(int itraj=0;itraj<(ntraj-1);itraj++){
      
      int pdg_inc=nu->ancestor[itraj].pdg;
      double incP[3];
      // itraj is the index of the projectile in each interaction.
      // one can find out what it did by looking at
      //       ancestor[itraj+1].proc
      // since 'proc' records the process which made the particle.
      //
      // Unfortunately, the pprod variables in g4numi v5 are needlessly complicated
      //
      // (1) If the particle at itraj *interacts* then pprod seems to hold
      // the momentum of the particle just before the interaction
      // 
      // (2) If the particle at itraj *decays* then pprod seems to hold the
      // momentum of the particle at itraj-1, just before it interacts
      //
      // (3) If the particle at itraj is the *result of a decay*, then pprod holds
      // something that looks like the momentum of the parent.
      //
      // Generally for hadron production studies, the second case isn't interesting
    //  if(nu->ancestor[itraj].pprodpz==0)std::cout<<"Found incident proton with 0 GeV Energy"<<std::endl;
      if( nu->ancestor[itraj].pprodpz != 0){
	incP[0] = nu->ancestor[itraj].pprodpx;
	incP[1] = nu->ancestor[itraj].pprodpy;
	incP[2] = nu->ancestor[itraj].pprodpz;
      
      }
      else{
	incP[0]=nu->ancestor[itraj].stoppx;
	incP[1]=nu->ancestor[itraj].stoppy;
	incP[2]=nu->ancestor[itraj].stoppz;
      }
      Int_t itraj_prod = itraj + 1;
      Int_t pdg_prod   = nu->ancestor[itraj_prod].pdg;
      std::string this_proc = std::string(nu->ancestor[itraj_prod].proc);
      
      // skip over etas and other swiftly decaying particles
      // we are interested in their daughters
      //It seems that the next while is causing seg fault in gcc 6.3:
      /*while( is_fast_decay(pdg_prod) ){
	itraj_prod++;
	Nskip++;
	pdg_prod = nu->ancestor[itraj_prod].pdg;
	}*/
      if( is_fast_decay(pdg_prod) ){
	for(int ifast = itraj_prod + 1; ifast < (ntraj-1); ifast++ ){
	  itraj_prod++;
	  Nskip++;
          pdg_prod = nu->ancestor[itraj_prod].pdg;
	  if( !is_fast_decay(pdg_prod) ) break;	  
	}
      }
      double prodP[3];
      prodP[0] = nu->ancestor[itraj_prod].startpx;
      prodP[1] = nu->ancestor[itraj_prod].startpy;
      prodP[2] = nu->ancestor[itraj_prod].startpz;
      
      double vtx[3];
      vtx[0]=nu->ancestor[itraj_prod].startx;
      vtx[1]=nu->ancestor[itraj_prod].starty;
      vtx[2]=nu->ancestor[itraj_prod].startz;
      std::string this_vol=nu->ancestor[itraj_prod].ivol;
      
      //Get Rid of Hydrogen
        if(pdg_prod == 1000010020 || pdg_inc == 1000010020){
      // std::cout<<"InteractionChainData::Unusual pdgcode found "<<pdg_prod<<std::endl; //For now just skipping these deuterons
	continue;
	}
      
      InteractionData inter(itraj, incP,pdg_inc,prodP,pdg_prod,
			    this_vol,this_proc,vtx);   
      interaction_chain.push_back(inter);

    }// end loop over trajectories
    /*&int tptype = numi2pdg.GetPdg(nu->tgtexit.tptype);
    if(mode=="NUMI")tptype = nu->tgtexit.tptype; this was added by DUNE... seems not right (Leo) */
      //  std::cout<<"The tptype::InteractionChainData "<<tptype<<" "<<nu->tgtexit.tptype<<std::endl;
    if(meta->vintnames.size()==0){
      tar_info = TargetData(tarP,nu->tgtexit.tptype,tarV,-1);
    }
    else{    
      tar_info = TargetData(tarP,nu->tgtexit.tptype,tarV,nu->vint[0]-Nskip);
    }
    
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

    //special tgt configuration for Minerva (exact longitudinal position after survey)
    //check for other experiments
    if(meta->vintnames.size()>1){
      playlist = nu->vint[1];
    }
    else{
      playlist = -1;
    }
    
  }

  std::ostream& InteractionChainData::print(std::ostream& os) const{
    using namespace std;
    os<<"==== InteractionChainData ====\n"
      <<" *config* "<<target_config<<" "<<horn_config
      <<"\n *target info*\n  ";
    tar_info.print(os);
    os<<"\n *ancestors*\n";
    for(size_t i=0; i<interaction_chain.size(); i++){
      os<<"   ";
      interaction_chain[i].print(os);
    }
    os<<"\n *Particlethrough volumes:*\n";
    for(size_t i=0; i<ptv_info.size(); i++){
      os<<"   ";
      ptv_info[i].print(os);
    }
    os<<endl;
    return os;
  }

  bool InteractionChainData::is_fast_decay(int pdg){
    
    bool fast_decay = false;
    if( (pdg==221)||(pdg==331)||(pdg==3212)||(pdg==113)||(pdg==223) )fast_decay = true;
    return fast_decay;
  }

}
