#ifndef INTERACTIONCHAINDATA_H
#define INTERACTIONCHAINDATA_H

#include "InteractionData.h"
#include "TargetData.h"
#include "ParticlesThroughVolumesData.h"
#include <vector>
#include "nu_g4numi.h" 
#include "dk2nu/tree/dk2nu.h"
#include "dk2nu/tree/dkmeta.h"
#include <iostream>

namespace NeutrinoFluxReweight{
  
  /*! \class InteractionChainData
   *  \brief Information about the chain of interactions leading to a neutrino 
  */

  class InteractionChainData{
  public:

    //! boring old default constructor
    InteractionChainData();

    /*! create an interaction chain from the old g4numi nudata structure
     * tgtcfg specified the target configuration. Example: le010z
     * horncfg specifies the horn configuration. Example: 185i
     */
    InteractionChainData(nu_g4numi* nu,
			 const char* tgtcfg, 
			 const char* horncfg);

    //! create an interaction chain from the new dk2nu format
    InteractionChainData(bsim::Dk2Nu* nu, bsim::DkMeta* meta);
    

    //! vector of neutrino ancestors 
    std::vector<InteractionData> interaction_chain;
    
    //! Information about the hadron which exited the target
    TargetData tar_info; 

    //! Information about all particles that pass through volumes without interacting
    std::vector<ParticlesThroughVolumesData>  ptv_info;

    //! The target configuration. Example: le010z
    std::string target_config;
    
    //! The horn configuration. Example: 185i
    std::string horn_config;

    //! The tgt playlist (exact position of the target after survey)
    int playlist;

    std::ostream& print(std::ostream& os=std::cout) const;
    
  private:
    bool is_fast_decay(int pdg);

  };
  
  
};
#endif
