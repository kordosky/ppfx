///----------------------------------------------------------------------------
/**
 * \class bsim::Dk2Nu
 * \file  dk2nu.h
 *
 * \brief A class that defines the "dk2nu" object used as the primary
 *        branch for a TTree for the output of neutrino flux simulations
 *        such as g4numi, g4numi_flugg, etc.
 *
 * \author (last to touch it) $Author: kordosky $
 *
 * \version $Revision: 1.1 $
 *
 * \date $Date: 2014/12/19 21:27:42 $
 *
 * Contact: rhatcher@fnal.gov
 *
 * $Id: dk2nu.h,v 1.1 2014/12/19 21:27:42 kordosky Exp $
 *
 * Notes tagged with "DK2NU" are questions that should be answered
 */
///----------------------------------------------------------------------------

#ifndef BSIM_DK2NU_H
#define BSIM_DK2NU_H

#include "TROOT.h"
#include "TObject.h"

#include <vector>
#include <string>

#define DK2NUVER 9   // KEEP THIS UP-TO-DATE!  increment for each change

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

   /**
    *=======================================================================
    *  Specialized enumerations
    */

   /**
    *  Proposed flag bits:
    */
   typedef enum flgbitval {
     kFlgOverflow    = 0x00000001,
     kMaskReserved   = 0x0000FFFF,
     kMaskUser       = 0xFFFF0000
   } flgbitval_t;

   /**
    *  Enumeration of decay processes, stored in "ndecay"
    *  stored as integer; these are for reference
    *  DK2NU:  should there be an associated AsString() method
    *          that returns a text (optionally formatted for latex?)?
    */
   typedef enum dkproc {
     dkp_unknown         =  0,
     dkp_k0l_nuepimep    =  1,  ///< k0long => nu_e + pi- + e+
     dkp_k0l_nuebpipem   =  2,  ///< k0long => nu_e_bar + p+ + e-
     dkp_k0l_numupimmup  =  3,  ///< k0long => nu_mu + pi- + mu+
     dkp_k0l_numubpipmum =  4,  ///< k0long => nu_mu_bar + pi+ + mu-
     dkp_kp_numumup      =  5,  ///< k+ => nu_mu + mu+
     dkp_kp_nuepi0ep     =  6,  ///< k+ => nu_e + pi0 + e+
     dkp_kp_numupi0mup   =  7,  ///< k+ => nu_mu + pi0 + mu+
     dkp_kp_numubmum     =  8,  ///< k- => nu_mu_bar + mu-
     dkp_kp_nuebpi0em    =  9,  ///< k- => nu_e_bar + pi0 + e-
     dkp_kp_numubpi0mum  = 10,  ///< k- => nu_mu_bar + pi0 + mu-
     dkp_mup_nusep       = 11,  ///< mu+ => nu_mu_bar + nu_e + e+
     dkp_mum_nusep       = 12,  ///< mu- => nu_mu + nu_e_bar + e-
     dk_pip_numumup      = 13,  ///< pi+ => nu_mu + mu+
     dk_pim_numubmum     = 14,  ///< pi- => nu_mu_bar + mu-
     dkp_maximum,               ///< one-beyond end for iterating
     dkp_other           = 999  ///< flag for unusual cases
   } dkproc_t;
  
  ///---------------------------------------------------------------------------
  /**
   *============================================================================
   *  Fixed Decays through a specific point:
   */
  class NuRay
  {
  public:
    Double_t px;     ///< px for nu at location
    Double_t py;     ///< py for nu at location
    Double_t pz;     ///< pz for nu at location
    Double_t E;      ///< E for nu at location
    Double_t wgt;    ///< weight for nu at location
    
  public:
    NuRay();
    NuRay(double px, double py, double pz, double E, double wgt);
    virtual     ~NuRay();
    void        clear(const std::string &opt = "");    ///< reset everything
    std::string AsString(const std::string& opt = "") const;
    
  private:
    ClassDef(bsim::NuRay,DK2NUVER)
  };  // end-of-class bsim::NuRay

  ///---------------------------------------------------------------------------
  /**
   *============================================================================
   *  Decay Data:
   *  Core information about the neutrino and the decay that gave rise to it.
   *  % = necessary for reweighting
   */
  class Decay
  {
  public:
   Int_t    norig;        ///< not used?
   Int_t    ndecay;       ///< decay process (see dkproc_t)
   Int_t    ntype;        ///< % neutrino flavor (PDG? code)

   Double_t vx;           ///< % neutrino production vertex x
   Double_t vy;           ///< % neutrino production vertex y
   Double_t vz;           ///< % neutrino production vertex z
   Double_t pdpx;         ///< % px momentum of nu parent at (vx,vy,vz)
   Double_t pdpy;         ///< % py momentum of nu parent at (vx,vy,vz)
   Double_t pdpz;         ///< % pz momentum of nu parent at (vx,vy,vz)

   /**  these are used in muon decay case? */
   Double_t ppdxdz;       ///< % direction of nu parent at its production point
   Double_t ppdydz;       ///< % direction of nu parent at its production point
   Double_t pppz;         ///< % z momentum of nu parent at its production point
   Double_t ppenergy;     ///< % energy of nu parent at its production point

   Int_t    ppmedium;     ///< material nu parent was produced in
   Int_t    ptype;        ///< % nu parent species (PDG? code)

   /** momentum and energy of nu grandparent at
       muons:    grandparent decay point
       hadrons:  grandparent production point
       Huh?  this needs better documentation
    */
   Double_t muparpx;      ///< %
   Double_t muparpy;      ///< %
   Double_t muparpz;      ///< %
   Double_t mupare;       ///< % energy of nu grandparent

   Double_t necm;         ///< % nu energy in center-of-mass frame
   Double_t nimpwt;       ///< % cumulative importance weight prod to decay

  public:
    Decay();
    virtual     ~Decay();
    void        clear(const std::string &opt = "");    ///< reset everything
    std::string AsString(const std::string& opt = "") const;
    
  private:
    ClassDef(bsim::Decay,DK2NUVER)
  };  // end-of-class bsim::Decay

  ///---------------------------------------------------------------------------
  /**
   *============================================================================
   *  Ancestor Data:
   *  Information about the chain of particles from the initial proton (indx=0)
   *  to the final neutrino.
   */
  class Ancestor
  {
  public:
    Int_t    pdg;      ///< ancestor species
    
    Double_t startx;   ///< particle x initial position
    Double_t starty;   ///< particle y initial position
    Double_t startz;   ///< particle z initial position
    Double_t startt;   ///< particle initial time

    Double_t startpx;  ///< particle x initial momentum
    Double_t startpy;  ///< particle y initial momentum
    Double_t startpz;  ///< particle z initial momentum
    
    Double_t stoppx;   ///< particle x final momentum
    Double_t stoppy;   ///< particle y final momentum
    Double_t stoppz;   ///< particle z final momentum
    
    Double_t polx;     ///< x component of polarization
    Double_t poly;     ///< y component of polarization
    Double_t polz;     ///< z component of polarization
    
    // what are these ... somehow different from stoppx[-1]?
    Double_t pprodpx;  ///< parent x momentum when producing this particle
    Double_t pprodpy;  ///< parent y momentum when producing this particle
    Double_t pprodpz;  ///< parent z momentum when producing this particle
    
    Int_t    nucleus;  ///< nucleus (PDG) type causing the scatter
    
    std::string proc;  ///< name of the process that creates this particle
    std::string ivol;  ///< name of the volume where the particle starts
    std::string imat;  ///< name of the material where the particle starts

  public:
    Ancestor();
    virtual     ~Ancestor();
    void        clear(const std::string &opt = "");    ///< reset everything
    std::string AsString(const std::string& opt = "") const;
    
    /// set triplets
    void        SetStartXYZT(Double_t x, Double_t y, Double_t z, Double_t t);
    void        SetStartP(Double_t px, Double_t py, Double_t pz);
    void        SetStopP(Double_t px, Double_t py, Double_t pz);
    void        SetPProdP(Double_t px, Double_t py, Double_t pz);

    /// helper functions
    Double_t    r() const;
    Double_t    startpt() const;
    Double_t    startp() const;
    Double_t    stoppt() const;
    Double_t    stopp() const;
    Double_t    pprodpt() const;
    Double_t    pprodp() const;

  private:
    ClassDef(bsim::Ancestor,DK2NUVER)
  };  // end-of-class bsim::Ancestor

  ///---------------------------------------------------------------------------
  /**
   *============================================================================
   * these ancestors are possibly, but not necessarily, the direct nu parent
   * DK2NU: can these be removed in favor of cascade info (ancestor above)?
   * 2012-11-08: for now keep these
   */
  class TgtExit
  {
  public:
    Double_t tvx;          ///< x position of nu ancestor as it exits target
    Double_t tvy;          ///< y position of nu ancestor as it exits target
    Double_t tvz;          ///< z position of nu ancestor as it exits target
    Double_t tpx;          ///< x momentum of nu ancestor as it exits target
    Double_t tpy;          ///< y momentum of nu ancestor as it exits target
    Double_t tpz;          ///< z momentum of nu ancestor as it exits target
    Int_t    tptype;       ///< species of ancestor exiting the target
    Int_t    tgen;         ///< nu parent generation in cascade:
                           ///<   1=primary proton
                           ///<   2=particles produced by proton interaction
                           ///<   etc
  public:
    TgtExit();
    virtual     ~TgtExit();
    void        clear(const std::string &opt = "");    ///< reset everything
    std::string AsString(const std::string& opt = "") const;
    
  private:
    ClassDef(bsim::TgtExit,DK2NUVER)
  };  // end-of-class TgtExit
  
  ///---------------------------------------------------------------------------
  /**
   *============================================================================
   * Track points stored at special locations for plotting trajectories
   */
  class Traj
  {
  public:
    Double_t trkx;
    Double_t trky;
    Double_t trkz;
    Double_t trkpx;
    Double_t trkpy;
    Double_t trkpz;

  public:
    Traj();
    virtual     ~Traj();
    void        clear(const std::string &opt = "");    ///< reset everything
    std::string AsString(const std::string& opt = "") const;
    
  private:
    ClassDef(bsim::Traj,DK2NUVER)
  };  // end-of-class bsim::Traj
  
  ///---------------------------------------------------------------------------
  /**
   *============================================================================
   * This is the structure that is the basis for the flux ntuple
   */
  class Dk2Nu
  {
  public:
   Int_t job;                        ///< identifying job #
   Int_t potnum;                     ///< proton # processed by simulation
   bsim::Decay decay;                ///< basic decay information
   std::vector<bsim::NuRay> nuray;   ///< rays through detector fixed points
   std::vector<bsim::Ancestor> ancestor;  ///< chain from proton to neutrino

   /**
    * These are ancestor.vx[size-2]  kept, for now, for convenience
    */
   Double_t ppvx;                   ///< production vertex x of nu parent
   Double_t ppvy;                   ///< production vertex y of nu parent
   Double_t ppvz;                   ///< production vertex z of nu parent

   bsim::TgtExit tgtexit;           ///< info about leaving the target
   std::vector<bsim::Traj> traj;    ///< [optional] trajectory points

   /**
    *=======================================================================
    *  Special Info:
    */
   Int_t    flagbits;      ///< bits signify non-std setting such as
                           ///< Geant vs. PDG codes, mm vs. cm, Mev vs. GeV
                           ///< bit00 = ancestore overflow (carry over from g4minerva)
   std::vector<Int_t>    vint;    ///< user defined vector of integers
   std::vector<Double_t> vdbl;    ///< user defined vector of doubles

  public:
    /**
     *   Public methods for constructing/destruction and resetting the data
     */
    Dk2Nu();
    virtual     ~Dk2Nu();
    void        clear(const std::string &opt = "");    ///< reset everything
    std::string AsString(const std::string& opt = "") const;
    void	    Print(Option_t* option = "") const;

    size_t      indxnu() const;    ///< ancestor index of nu ancestor.size()-1
    size_t      indxp() const;     ///< ancestor index of parent ancestor.size()-2
    size_t      indxgp() const;    ///< ancestor index of grandparent ancestor.size()-3
    bool        overflow() const;  ///< ancestor list is incomplete (g4 minerva overflow)

  private:
    ClassDef(bsim::Dk2Nu,DK2NUVER)
   
 };

} // end-of-namespace "bsim"

// not part of namespace bsim
std::ostream& operator<<(std::ostream& os, const bsim::Dk2Nu& dk2nu);
std::ostream& operator<<(std::ostream& os, const bsim::NuRay& nuray);
std::ostream& operator<<(std::ostream& os, const bsim::Decay& decay);
std::ostream& operator<<(std::ostream& os, const bsim::Ancestor& ancestor);
std::ostream& operator<<(std::ostream& os, const bsim::TgtExit& tgtexit);
std::ostream& operator<<(std::ostream& os, const bsim::Traj& traj);

#endif  // BSIM_DK2NU_H
