#ifndef TARGETDATA_H
#define TARGETDATA_H

#include <cmath>
#include <iostream>

namespace NeutrinoFluxReweight{
  
  /*! \class TargetData
   *  \brief The information about the hadron that exits the target
 */
  class TargetData{ 
 
  public:
    //! Default Constructor
    TargetData();

    //! Constructor given kinematic of the hadron.
    TargetData(double tarMom[],int tarPdg, double position[]);

    virtual ~TargetData();
    
    //! pdg code of the particle 
    int Tar_pdg;

    //! Longitudinal momentum (GeV/c) of the particle 
    double Pz;

    //! P_{x} (GeV/c) of the particle 
    double Px;

    //! P_{y} (GeV/c) of the particle 
    double Py;

    //! Angle (rad) of the particle 
    double Theta;

    //! Transversal momentum (GeV/c) of the particle 
    double Pt;

    //! The x position of the hadron leaving the target
    double Vx;

    //! The y position of the hadron leaving the target
    double Vy;

    //! The z position of the hadron leaving the target
    double Vz;

    std::ostream& print(std::ostream& os) const;

  };

  
};
#endif
