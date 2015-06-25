#ifndef FILLIMAPHISTS_H
#define FILLIMAPHISTS_H

#include "CommonIMapIncludes.h"
#include "TChain.h"
#include "dk2nu.h"
#include "dkmeta.h"

#include "InteractionChainData.h"

struct FillIMapHistsOpts {
  float elow, ehigh;
  Int_t nu_pdg;
  bool cut_na49, cut_mipp;
};


/*! \fn FillIMapHists
 * \brief Fills the interaction map hists, looping over dk2nu input
 * Loop over the events in the Dk2Nu chain passed in,
 * construct an InteractionChainData and then call FillOneEntry()
 */
void FillIMapHists(TChain* tdk2nu, TChain* dkmeta, HistList* hists, const FillIMapHistsOpts* opts);

/*! \fn FillOneEntry
 * \brief Fill hists with the data inside of icd
 * This is a reimplementation of the routine originally written by 
 * Maria (Majo) Bustamante and Leo Aliaga, and improved by others.
 */
void FillOneEntry(const  NeutrinoFluxReweight::InteractionChainData& icd, HistList* hists, const FillIMapHistsOpts* opts);



#endif // FILLIMAPHISTS_H
