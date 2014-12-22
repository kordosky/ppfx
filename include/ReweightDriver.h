#ifndef REWEIGHTDRIVER_H
#define REWEIGHTDRIVER_H

#include "NA49Reweighter.h"
#include "MIPPThinTargetReweighter.h"
#include "TheoryThinTargetReweighter.h"

#include "MIPPNumiYieldsReweighter.h"
#include "MIPPNumiKaonsYieldsReweighter.h"
#include "TargetAttenuationReweighter.h"

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
     * the constructor
    */
    ReweightDriver(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars);
    

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
    double calculateWeight(const InteractionChainData& icd);
    
  private:    
    /*!
     * Configures each of the reweighing tools.
     */
    void Configure();
    int iUniv;
    ParameterTable cvPars;
    ParameterTable univPars;

    NA49Reweighter* NA49_Universe;
    MIPPThinTargetReweighter* MIPP_THIN_Universe;
    TheoryThinTargetReweighter* THEORY_Universe;

    MIPPNumiYieldsReweighter* MIPP_NUMI_Universe;
    MIPPNumiKaonsYieldsReweighter* MIPP_NUMI_KAONS_Universe;
    TargetAttenuationReweighter* TARG_ATT_Universe;



  };

  
};
#endif
