#ifndef NUWEIGHT_H
#define NUWEIGHT_H

#include "dk2nu/tree/dk2nu.h"
#include "dk2nu/tree/dkmeta.h"

#include <TParticlePDG.h>
#include <TDatabasePDG.h>
#include <vector>
#include <iostream>
#include <cmath>

namespace NeutrinoFluxAuxiliar{
  
  /*! \class NuWeight
   *  \brief Get the weight to get the neutrino probability flux 
   * in one point.
   */
  class NuWeight{
  public:
    
    //! The constructor has to have the position of the detector. 
    NuWeight(std::vector<double>& posdet);

    //! Destructor
    ~NuWeight();
    
    //! Calculate weight based on dk2nu entry. After this, enu and wgt have meaningful values 
    void calculate_weight(bsim::Dk2Nu* nu);
    double enu;
    double wgt;
    
  private:
    TDatabasePDG* particle;
    double xdet;
    double ydet;
    double zdet;

  };
  
}

#endif 
