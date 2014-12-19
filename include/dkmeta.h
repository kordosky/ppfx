/**
 * \class bsim::DkMeta
 * \file  dkmeta.h
 *
 * \brief A class that defines the "DkMeta" object used as the 
 *        branch for a TTree for the output of meta-data from 
 *        neutrino flux simulations such as g4numi, g4numi_flugg, etc.
 *        This tree has one entry of this type for the file.  Kept
 *        as a tree so files can be chained.
 *
 * \author (last to touch it) $Author: kordosky $
 *
 * \version $Revision: 1.1 $
 *
 * \date $Date: 2014/12/19 21:27:42 $
 *
 * Contact: rhatcher@fnal.gov
 *
 * $Id: dkmeta.h,v 1.1 2014/12/19 21:27:42 kordosky Exp $
 *
 * Notes tagged with "DKMETA" are questions that should be answered
 */

#ifndef BSIM_DKMETA_H
#define BSIM_DKMETA_H

#include "TROOT.h"
#include "TObject.h"

#include <vector>
#include <string>

#define DKMETAVER 7   // KEEP THIS UP-TO-DATE!  increment for each change

namespace bsim {
  /**
   *  All the data members are public as these classes are used as
   *  generalized structs.  As they will be branches of a TTree no
   *  specialized naming indicators signifying that they are member data
   *  of a class will be used, nor will any fancy capitalization schemes.
   *
   *  All classes must implement a clear() method that resets their values
   *  to an identifiably invalid state or clears any vectors.  Additionally
   *  classes should provide a AsString() method for formatting themselves
   *  for use output.
   */
  
  ///---------------------------------------------------------------------------
  /**
   *============================================================================
   *  Specified location where energy and weight will be evaluated
   *  Assume to be in beam coordinate system w/ units of cm
   */
  class Location
  {
  public:
    Double_t x;        ///< x position of location
    Double_t y;        ///< y position of location
    Double_t z;        ///< z position of location
    std::string name;  ///< identifying name
    
  public:
    Location();
    Location(double x, double y, double z, std::string name);
    virtual     ~Location();
    void        clear(const std::string &opt = "");    ///< reset everything
    std::string AsString(const std::string& opt = "") const;
    
  private:
    ClassDef(bsim::Location,DKMETAVER)
  };  // end-of-class bsim::Location

  ///---------------------------------------------------------------------------
  /**
   *============================================================================
   *  Metadata for the flux file
   */
  class DkMeta
  {
  public:

    /**
     *============================================================================
     *  General information
     */
    Int_t    job;           ///< identifying job # (keep files distinct)
    Double_t pots;          ///< protons-on-target
    
    /**
     * DKMETA:  
     * formatted strings are most flexible ...
     * but not necessarily convenient to use
     * ??? Should parts of these be standardized ??? 
     */
    std::string beamsim;    ///< e.g. "flugg" or "g4numi/<tag>"
    std::string physics;    ///< e.g. "fluka08", "g4.9.3p01"
    std::string physcuts;   ///< tracking cuts    e.g. "threshold=0.1GeV"
    std::string tgtcfg;     ///< target config    e.g. "minos/epoch3/-10cm"
    std::string horncfg;    ///< horn config      e.g. "FHC/185A/LE/h1xoff=1mm"
    std::string dkvolcfg;   ///< decay vol config e.g. "helium" or "vacuum"
    
    /**
     *============================================================================
     *  Beam Info:
     */
    Double_t beam0x;       ///< x of beam center at start
    Double_t beam0y;       ///< y of beam center at start
    Double_t beam0z;       ///< z of beam start
    Double_t beamhwidth;   ///< horizontal width of beam
    Double_t beamvwidth;   ///< vertical width of beam
    Double_t beamdxdz;     ///< beam slope dx/dz
    Double_t beamdydz;     ///< beam slope dy/dz
    
    /**
     *============================================================================
     *  Detector Position Info:
     *  Values are in beam coordinate system w/ units of "cm"
     */
    std::vector<bsim::Location> location;   ///< locations
    
    /**
     *============================================================================
     *  Special Info:
     *  Document extensibility enhancements 
     */
    std::vector<std::string> vintnames;    ///< names of elements for user defined vector of integers
    std::vector<std::string> vdblnames;    ///< names of elements for user defined vector of doubles
    
  public:
    /**
     *   Public methods for constructing/destruction and resetting the data
     */
    DkMeta();
    virtual     ~DkMeta();
    void        clear(const std::string &opt = "");    ///< reset everything
    std::string AsString(const std::string& opt = "") const; ///< output as a string
    
  private:
    ClassDef(bsim::DkMeta,DKMETAVER)
    
  }; // end-of-class bsim::DkMeta

} // end-of-namespace "bsim"

// not part of namespace bsim
std::ostream& operator<<(std::ostream& os, const bsim::DkMeta& dkmeta);
std::ostream& operator<<(std::ostream& os, const bsim::Location& location);

#endif   // BSIM_DKMETA_H
