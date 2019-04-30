#include "NuWeight.h"

const double rdet = 100.; //in cm

namespace NeutrinoFluxAuxiliar{

  NuWeight::NuWeight(std::vector<double>& posdet){
    
    particle = TDatabasePDG::Instance();
    NuWeight::enu  = -1.0;
    NuWeight::wgt  = -1.0;
    xdet = posdet[0];
    ydet = posdet[1];    
    zdet = posdet[2];
  }
  void NuWeight::calculate_weight(bsim::Dk2Nu* nu){
     //assumes units are GeV and cm

    int likely_ion = 100000000;
    double mpar=-1.;
    int size = (nu->ancestor).size();
    int pdg = nu->ancestor[size-2].pdg;

    if(pdg>likely_ion || pdg<-1*likely_ion){
      std::cout<< "Not handling ions yet, pdg: "<<pdg<<std::endl;
      exit (1);
    }
    else{
      mpar = particle->GetParticle(pdg)->Mass();
    }
    
    double ppar  = sqrt( pow(nu->decay.pdpx,2) + pow(nu->decay.pdpy,2) + pow(nu->decay.pdpz,2) );
    double epar  = sqrt(pow(ppar,2)+pow(mpar,2));
    double gamma = epar / mpar;
    double beta = sqrt(pow(gamma,2)-1)/gamma;

    double rr = sqrt(pow(xdet-nu->decay.vx,2)+pow(ydet-nu->decay.vy,2)+pow(zdet-nu->decay.vz,2));
    double cos_theta = ( (nu->decay.pdpx)*(xdet-nu->decay.vx) + (nu->decay.pdpy)*(ydet-nu->decay.vy) + (nu->decay.pdpz)*(zdet-nu->decay.vz) )/ (ppar*rr);
 
    if(cos_theta > 1 || cos_theta < -1){
      std::cout<< "Cosine of neutrino not allowed: "<<cos_theta<<std::endl;
      exit (1);
    }
    double MM      = 1.0/(gamma*(1.0-beta*cos_theta));
    double angdet  = (pow(rdet,2) /pow(rr,2)/ 4.); 
    NuWeight::enu  =  MM*(nu->decay.necm);
    NuWeight::wgt  = angdet * pow(MM,2);
 
    //done for all except polarized muon
    // in which case need to modify weight
    if (pdg == 13 || pdg == -13){
      
      //boost new neutrino to mu decay cm
      double vbeta[3];
    vbeta[0] = nu->decay.pdpx / epar;
    vbeta[1] = nu->decay.pdpy / epar;
    vbeta[2] = nu->decay.pdpz / epar;
    
    double p_nu[3]; //nu momentum    
    p_nu[0] = (xdet- nu->decay.vx) * (NuWeight::enu) / rr;
    p_nu[1] = (ydet- nu->decay.vy) * (NuWeight::enu) / rr;
    p_nu[2] = (zdet- nu->decay.vz) * (NuWeight::enu) / rr;

    double partial = gamma*(vbeta[0]*p_nu[0]+vbeta[1]*p_nu[1]+vbeta[2]*p_nu[2]);
    partial = (NuWeight::enu)-partial / (gamma+1.);
    
    double p_dcm_nu[4];
    for (int i=0;i<3;i++) p_dcm_nu[i]=p_nu[i]-vbeta[i]*gamma*partial;
    p_dcm_nu[3]=0.;
    for (int i=0;i<3;i++) p_dcm_nu[3]+=p_dcm_nu[i]*p_dcm_nu[i];
    p_dcm_nu[3]=sqrt(p_dcm_nu[3]);
    
    //boost parent of mu to mu production cm
    gamma   = nu->decay.ppenergy / mpar;
    vbeta[0] = nu->decay.ppdxdz * nu->decay.pppz / nu->decay.ppenergy;
    vbeta[1] = nu->decay.ppdydz * nu->decay.pppz / nu->decay.ppenergy;
    vbeta[2] =                    nu->decay.pppz / nu->decay.ppenergy;
    partial = gamma*(vbeta[0]*nu->decay.muparpx+vbeta[1]*nu->decay.muparpy+vbeta[2]*nu->decay.muparpz);
    partial = nu->decay.mupare - partial / (gamma+1.);
  
    double p_pcm_mp[4];
    p_pcm_mp[0]=nu->decay.muparpx-vbeta[0]*gamma*partial;
    p_pcm_mp[1]=nu->decay.muparpy-vbeta[1]*gamma*partial;
    p_pcm_mp[2]=nu->decay.muparpz-vbeta[2]*gamma*partial;
    p_pcm_mp[3]=0.;
    for (int i=0;i<3;i++) p_pcm_mp[3]+=p_pcm_mp[i]*p_pcm_mp[i];
    p_pcm_mp[3]=sqrt(p_pcm_mp[3]);
    
    double wt_ratio = 1.;
    //have to check p_pcm_mp
    //it can be 0 if mupar..=0. (I guess muons created in target??)
    if (p_pcm_mp[3] != 0. ) {
      //calc new decay angle w.r.t. (anti)spin direction
      double costh = (p_dcm_nu[0]*p_pcm_mp[0]+ 
		      p_dcm_nu[1]*p_pcm_mp[1]+ 
		      p_dcm_nu[2]*p_pcm_mp[2])/(p_dcm_nu[3]*p_pcm_mp[3]);
      
      if (costh>1.) costh = 1.;
      else if (costh<-1.) costh = -1.;
      
      //calc relative weight due to angle difference
      if(nu->decay.ntype == 12 || nu->decay.ntype == -12){
	wt_ratio = 1.-costh;
      }
      else if(nu->decay.ntype == 14 || nu->decay.ntype == -14){
	double mumass = particle->GetParticle(13)->Mass(); 
	double xnu = 2.* nu->decay.necm / mumass;
	wt_ratio = ( (3.-2.*xnu) - (1.-2.*xnu)*costh ) / (3.-2.*xnu);
      } else {
	std::cout << "NuWeight:: Bad neutrino type"<<std::endl;
      }
    }
    NuWeight::wgt *= wt_ratio;
    }

  }
  //////
  NuWeight::~NuWeight(){ 
  }
    
};
