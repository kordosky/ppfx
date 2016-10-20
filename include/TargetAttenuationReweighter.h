#ifndef TARGETATTENUATIONREWEIGHTER_H
#define TARGETATTENUATIONREWEIGHTER_H

#include "IInteractionChainReweighting.h"

namespace NeutrinoFluxReweight{
  
  /*! \class TargetAttenuationReweighter
   *  \brief Reweight to account for attenuation of the beam in the target
   *
   * If the MC does not get the reaction cross-section correct the number of interactions in the target, the number of 
   * primary protons which do not interact in the target, and the distribution of interactions along z are affected. MIPP
   * measures yields per incoming proton, not per interaction, so their measurment includes the probability of interaction
   * and also the yield per interaction.
   */
  
  class TargetAttenuationReweighter : public IInteractionChainReweighting{
  public:  
    TargetAttenuationReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars);
    virtual ~TargetAttenuationReweighter();
    virtual std::vector<bool> canReweight(const InteractionChainData& aa);
    virtual double calculateWeight(const InteractionChainData& aa);
    /*!
     * Uses the input target configuration to figure out and return
     * the upstream edge of the 1st budal monitor.
     * This function will look at the input string, remove anything that's
     * not a digit, and then interpret the rest as an offset from the 000z position
     */
    static double targetStartZ(const std::string& tgtcfg);

    //!Get the additional shift for the Minerva playlist if this is defined
    static double shiftPlaylist(const int ipl);

    //! does the configuration correspond to the ME beam?
    static bool isME(const std::string& tgtcfg);

    //! does the configuration correspond to the LE beam?
    static bool isLE(const std::string& tgtcfg);
    
    static double getTargetPenetrationLE(double z_start, double z_end, double z0_budal);
    static double getTargetPenetrationME(double z_start, double z_end, double z0_budal);
    
    static double getZTgtExit(double pos_start[], double mom_start[], bool leflag, bool meflag);
    
  private:
    const ParameterTable& cvPars;
    const ParameterTable& univPars;
    int iUniv;
    float prod_prtC_xsec, qe_prtC_xsec;
    float delta_sigma_piC_xsec;
    float delta_sigma_kapC_xsec_lowP, delta_sigma_kapC_xsec_highP;
    float delta_sigma_kamC_xsec_lowP, delta_sigma_kamC_xsec_highP;

  };
  
  
};
#endif
