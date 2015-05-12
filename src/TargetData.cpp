
#include "TargetData.h"
#include <iomanip>

namespace NeutrinoFluxReweight{ 
  
  TargetData::TargetData(){
    
    Tar_pdg = 0;
    
    Px      = -1000.;
    Py      = -1000.;
    Pz      = -1000.;
    Pt      = -1000.;
    Theta   = -1000.;

    Vx      = -1000.;
    Vy      = -1000.;
    Vz      = -1000.;
    
    Idx_ancestry = -1;

  }
  
  TargetData::TargetData(double tarMom[],int tarPdg, double position[],int anc_idx){

    TargetData::Tar_pdg = tarPdg;
    
    TargetData::Px  = tarMom[0];
    TargetData::Py  = tarMom[1];
    TargetData::Pz  = tarMom[2];
    TargetData::Pt  = std::sqrt(tarMom[0]*tarMom[0] + tarMom[1]*tarMom[1]);
    TargetData::Theta = std::atan(TargetData::Pt/TargetData::Pz);

    TargetData::Vx  = position[0];
    TargetData::Vy  = position[1];
    TargetData::Vz  = position[2];
  
    Idx_ancestry = anc_idx; 
  }

  //----------------------------------------------------------------------
  TargetData::~TargetData(){
    
  }
  std::ostream& TargetData::print(std::ostream& os) const {
 
    using namespace std;
    
    os<<"pid:"<<setw(5)<<Tar_pdg
      <<"|p3:"<<setiosflags(ios::fixed) << setprecision(2);
    os<<Px<<","<<Py<<","<<Pz;
    os <<"|v3:";
    os<<Vx<<","<<Vy<<","<<Vz;
    //    os<<endl;
    return os;
  }

}
