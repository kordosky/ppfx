
#include "TargetData.h"

namespace NeutrinoFluxReweight{ 
  
  TargetData::TargetData(){
    
    Tar_pdg = 0;
    
    Px      = -1000.;
    Py      = -1000.;
    Pz      = -1000.;
    Pt      = -1000.;
    Theta   = -1000.;
    
  }
  
  TargetData::TargetData(double tarMom[],int tarPdg){

    TargetData::Tar_pdg = tarPdg;
    
    TargetData::Px  = tarMom[0];
    TargetData::Py  = tarMom[1];
    TargetData::Pz  = tarMom[2];
    TargetData::Pt  = std::sqrt(tarMom[0]*tarMom[0] + tarMom[1]*tarMom[1]);
    TargetData::Theta = std::atan(TargetData::Pt/TargetData::Pz);
  
  }

  //----------------------------------------------------------------------
  TargetData::~TargetData(){
    
  }
  
}
