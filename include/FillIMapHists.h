#ifndef FILLIMAPHISTS_H
#define FILLIMAPHISTS_H

#include "CommonIMapIncludes.h"
#include "TChain.h"
#include "dk2nu.h"
#include "dkmeta.h"
#include <string>


struct FillIMapHistsOpts {
  float elow, ehigh;
  Int_t nuid;
  bool cut_na49, cut_mipp;
};


/*! \fn FillIMapHists
 * \brief Fills the interaction map hists, looping over dk2nu input
 * Loop over the events in the Dk2Nu chain passed in and  call FillOneEntry()
 */
void FillIMapHists(TChain* tdk2nu, TChain* dkmeta, HistList* hists, const FillIMapHistsOpts* opts);

/*! \fn FillOneEntry
 * \brief Fill hists with the data inside of the dk2nu and dkmeta ntuples
 * This is a reimplementation of the routine originally written by 
 * Maria (Majo) Bustamante and Leo Aliaga, and improved by others.
 *
 * this function returns the weight used in filling the histograms,
 * which is zero if the event was skipped, but otherwise corresponds
 * to the product of the decay and importance weights.
 */
double FillOneEntry(bsim::Dk2Nu* dk2nu, bsim::DkMeta* dkmeta, HistList* hists, const FillIMapHistsOpts* opts);


int FindIndexFromVolume(const std::string& volname);

#endif // FILLIMAPHISTS_H
