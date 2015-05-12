#ifndef MAKEREWEIGHT_H
#define MAKEREWEIGHT_H

#include "ReweightDriver.h"
#include "InteractionData.h"
#include "TargetData.h"
#include "ParticlesThroughVolumesData.h"
#include "InteractionChainData.h"
#include "ParameterTable.h"
#include "CentralValuesAndUncertainties.h"
#include "MIPPNumiYieldsBins.h"
#include "MIPPNumiMC.h"
#include "HistoContainer.h"
#include "ExtractInfo.h"

#include "nu_g4numi.h"
#include "Numi2Pdg.h"

#include "dkmeta.h"
#include "dk2nu.h"

#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <iomanip>

namespace NeutrinoFluxReweight{
    
  /*! \class MakeReweight
   *  \brief A class to make the reweight event by event.
   */
  class MakeReweight{
  public: 
    /*! constructor 
     */
    MakeReweight(std::string fileIn);
    
    /*! calculate the weights for this event using the old flux ntuple format
     * (filled in nu_g4numi object),  the tgtcfg (the target configuration, example: le010z) 
     * and the horncfg (horn configuration, example: 185i)
     */
    void calculateWeights(nu_g4numi* nu, const char* tgtcfg, const char* horncfg);
    //! create an interaction chain from the new dk2nu(dkmeta) format
    void calculateWeights(bsim::Dk2Nu* nu, bsim::DkMeta* meta);
    
    std::vector<double> GetTotalWeights();
    int GetNumberOfUniversesUsed();
    std::vector<double> GetWeights(std::string nameReweighter);
  private:    
    /*!
     * Inititalize the job and configurethe ReweighterDrivier
     */
    void Initialize();
    /*! Parse seeting options */
    void ParseOptions();
    /*! Parse seeting options */
    void doTheJob(InteractionChainData* icd);
    
    int Nuniverses;
    std::string mippCorrOption;
    std::string fileOptions;
    std::vector<ReweightDriver*> vec_rws;
    std::vector<double> vec_wgts;
    std::map<std::string,std::vector<double> > map_rew_wgts;

  };

  
};
#endif
