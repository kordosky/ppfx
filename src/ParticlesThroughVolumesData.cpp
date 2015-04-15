
#include "ParticlesThroughVolumesData.h"
#include <iomanip>

namespace NeutrinoFluxReweight{ 
  
  ParticlesThroughVolumesData::ParticlesThroughVolumesData(){
   
    //!default values: 
    Pdgs[0] = 0;
    Pdgs[1] = 0;
    Pdgs[2] = 0;
    
    AmountMat[0] = -1.0;
    AmountMat[1] = -1.0;
    AmountMat[2] = -1.0;

    Moms[0] = 0.0;
    Moms[1] = 0.0;
    Moms[2] = 0.0;
    
    Vol = "None";
  }
  
  ParticlesThroughVolumesData::ParticlesThroughVolumesData(int ptv_pdg[], double  ptv_amount_mat[],double ptv_mom[],std::string ptv_vol){

     //!I assumed arrays of dimmension 3:
    ParticlesThroughVolumesData::Pdgs[0] = ptv_pdg[0];
    ParticlesThroughVolumesData::Pdgs[1] = ptv_pdg[1];
    ParticlesThroughVolumesData::Pdgs[2] = ptv_pdg[2];
    
    ParticlesThroughVolumesData::AmountMat[0] = ptv_amount_mat[0];
    ParticlesThroughVolumesData::AmountMat[1] = ptv_amount_mat[1];
    ParticlesThroughVolumesData::AmountMat[2] = ptv_amount_mat[2];

    ParticlesThroughVolumesData::Moms[0] = ptv_mom[0];
    ParticlesThroughVolumesData::Moms[1] = ptv_mom[1];
    ParticlesThroughVolumesData::Moms[2] = ptv_mom[2];
    
    ParticlesThroughVolumesData::Vol = ptv_vol;
    
  }

  //----------------------------------------------------------------------
  ParticlesThroughVolumesData::~ParticlesThroughVolumesData(){
    
  }
  std::ostream& ParticlesThroughVolumesData::print(std::ostream& os) const {
 
    using namespace std;
    
    os<<"Vol: "<<Vol<<endl;
    for(int ii=0;ii<3;ii++){
      os<<"pid:"<<setw(5)<<Pdgs[ii]<<
	"|dist, mom: "<<setiosflags(ios::fixed) << setprecision(2);
      os<<AmountMat[ii]<<","<<Moms[ii];
      os<<endl;
    }
    return os;
  }
  
}
