
#include "ParticlesThroughVolumesData.h"
#include <iomanip>

namespace NeutrinoFluxReweight{ 
  
  ParticlesThroughVolumesData::ParticlesThroughVolumesData(){
   
    //!default values: 
    pdg[0] = 0;
    pdg[1] = 0;
    pdg[2] = 0;
    
    distXdens[0] = -1.0;
    distXdens[1] = -1.0;
    distXdens[2] = -1.0;

    Mom[0] = 0.0;
    Mom[1] = 0.0;
    Mom[2] = 0.0;
    
    vol = "None";
  }
  
  ParticlesThroughVolumesData::ParticlesThroughVolumesData(int ptv_pdg[], double ptv_distXdens[],double ptv_mom[],std::string ptv_vol){

     //!I assumed arrays of dimmension 3:
    ParticlesThroughVolumesData::pdg[0] = ptv_pdg[0];
    ParticlesThroughVolumesData::pdg[1] = ptv_pdg[1];
    ParticlesThroughVolumesData::pdg[2] = ptv_pdg[2];
    
    ParticlesThroughVolumesData::distXdens[0] = ptv_distXdens[0];
    ParticlesThroughVolumesData::distXdens[1] = ptv_distXdens[1];
    ParticlesThroughVolumesData::distXdens[2] = ptv_distXdens[2];

    ParticlesThroughVolumesData::Mom[0] = ptv_mom[0];
    ParticlesThroughVolumesData::Mom[1] = ptv_mom[1];
    ParticlesThroughVolumesData::Mom[2] = ptv_mom[2];
    
    ParticlesThroughVolumesData::vol = ptv_vol;
    
  }

  //----------------------------------------------------------------------
  ParticlesThroughVolumesData::~ParticlesThroughVolumesData(){
    
  }
  std::ostream& ParticlesThroughVolumesData::print(std::ostream& os) const {
 
    using namespace std;
    
    os<<"Vol: "<<vol<<endl;
    for(int ii=0;ii<3;ii++){
      os<<"pid:"<<setw(5)<<pdg[ii]<<
	"|dist, mom: "<<setiosflags(ios::fixed) << setprecision(2);
      os<<distXdens[ii]<<","<<Mom[ii];
      os<<endl;
    }
    return os;
  }
  
}
