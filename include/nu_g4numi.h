#ifndef nu_g4numi_h
#define nu_g4numi_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

/*! Class to load the nudata tree */
class nu_g4numi{

 public: 

/** the constructor needs a TChain*/
  nu_g4numi(TChain* chain);
  virtual ~nu_g4numi();
  
  /** Get ntuple entry*/
  void GetEntry(Int_t ientry); 
  
/** Neutrino type \f$ 52: \bar{\nu}_{e}, 53: \nu_{e}, 55: \bar{\nu}_{\mu}, 56: \nu_{\mu} \f$ */
  Int_t     Ntype;

  /** Neutrino energy Position 0 is in the front of Minos*/
  Double_t  NenergyN[11];

  /** Geometrical weight to get the neutrino in front of Minos*/
  Double_t  NWtNear[11];

  /** Neutrino energy in far Minos*/
  Double_t  NenergyF[2];
 
  /** Geometrical weight to get the neutrino in the far Minos*/
  Double_t  NWtFar[2];
  
  /** Recover the right stats*/
  Double_t  Nimpwt; 

  //Ancestries:
  /** number of trajectory*/
  Int_t     ntrajectory; 

  /** overflow flag*/
  Bool_t    overflow;    

 /** pdg code*/
  Int_t     pdg[10];     

  /** start px */
  Double_t  startpx[10];

 /** start py */
  Double_t  startpy[10];

  /** start pz */
  Double_t  startpz[10]; 

 /** stop px */
  Double_t  stoppx[10];  

 /** stop py */
  Double_t  stoppy[10];

 /** stop pz */
  Double_t  stoppz[10]; 

 /** parent px at production point*/
  Double_t  pprodpx[10];

 /** parent py at production point*/
  Double_t  pprodpy[10];

 /** parent pz at production point*/
  Double_t  pprodpz[10];

  /** start x */
  Double_t  startx[10];

  /** start y */
  Double_t  starty[10];

  /** start z */
  Double_t  startz[10]; 

  /** stop x */
  Double_t  stopx[10];  

  /** stop y */
  Double_t  stopy[10];

  /** stop z */
  Double_t  stopz[10];  

 /** process name*/
  TString   proc[10];  

 /** initial volume of the track*/
  TString   ivol[10];

  /** final volume of the track*/
  TString   fvol[10];  
  
  //Tar:

  /** hadron off the target px*/
  Double_t tpx;

  /** hadron off the target py*/
  Double_t tpy;

  /** hadron off the target pz*/
  Double_t tpz;

  /** hadron off the target exit position x*/
  Double_t tvx;

  /** hadron off the target exit position y*/
  Double_t tvy;

  /** hadron off the target exit position z*/
  Double_t tvz;


  /** Neutrino parent type \f$ 8: \pi^{+}, 9: \pi^{-}, 11: K^{+}, 12: K^{-}\f$ */
  Int_t tptype;
    
 /** number of trajectory*/
  Int_t ntrees;

 /** number of trajectory*/
  Int_t nentries;
  
 private:
  TChain* fChain;

};

#endif
