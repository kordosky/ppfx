#ifndef REWEIGHTDRIVER_H
#define REWEIGHTDRIVER_H

#include "NA49Reweighter.h"
#include "MIPPThinTargetReweighter.h"
#include "TheoryThinTargetReweighter.h"

#include "MIPPNumiYieldsReweighter.h"
#include "MIPPNumiKaonsYieldsReweighter.h"
#include "AttenuationReweighter.h"

#include "InteractionChainData.h"

namespace NeutrinoFluxReweight{
    
  /*! \class ReweightDriver
   *  \brief A class to manage and drive the weight calculation procedure. 
   *
   * There is a one-to-one correspondence between ReweightDrivers and universes.
   */

  class ReweightDriver{
  public:  
    /*!
 
    */
    ReweightDriver();
    
    void Configure();
    /*!
     * Calculate weight for this event, given a set of central value parameters
     * and a set of randomly varied parameters corresponding to the universe
     * that this ReweightDriver is responsible for.
     *
     * - Call mipp_yields on input chain. record interactions covered with a weight in interaction_covered.
     * - Call attenuation on input chain
     * - Call thin target reweighters. 
     *    -# Call na49 for interactions not yet covered. record in interaction_covered
     *    -# ditto. call MIPP, then theory
     */
    double calculateWeight(const InteractionChainData& icd, ParameterTable& cv_pars, ParameterTable& univ_pars);
    void SetUniverseID(int univid);
    
  private:    
    int UniverseID;

    NA49Reweighter* NA49_Universe;
    MIPPThinTargetReweighter* MIPP_THIN_Universe;
    TheoryThinTargetReweighter* THEORY_Universe;

    MIPPNumiYieldsReweighter* MIPP_NUMI_Universe;
    MIPPNumiKaonsYieldsReweighter* MIPP_NUMI_KAONS_Universe;
    AttenuationReweighter* ATT_Universe;


  };

  
};
#endif
