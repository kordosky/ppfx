#ifndef PARTICLESTHROUGHVOLUMESDATA_H
#define PARTICLESTHROUGHVOLUMESDATA_H

#include <cmath>
#include <iostream>

namespace NeutrinoFluxReweight{
  
  /*! \class ParticlesThroughVolumes
   *  \brief The information about the distance (multiplied by the density) of the particles passed by a volume.
   */
  class ParticlesThroughVolumesData{ 
 
  public:
    //! Default Constructor
    ParticlesThroughVolumesData();

    //! Constructor given the kinematic and distance traveled in the volume.
    ParticlesThroughVolumesData(int ptv_pdg[], double ptv_distXdens[],double ptv_mom[],std::string ptv_vol);
    
    virtual ~ParticlesThroughVolumesData();
    
    //! pdg codes of the particles
    int pdg[];

    //! distance multiplied by volume 
    double distXdens[];

    //! Momentum of the particles 
    double Mom[];

    //! Voulme 
    std::string vol;

    std::ostream& print(std::ostream& os) const;

  };

  
};
#endif
