
#include "TheoryThinTargetReweighter.h"
#include <iostream>

namespace NeutrinoFluxReweight{
  
  TheoryThinTargetReweighter::TheoryThinTargetReweighter(int iuniv, const ParameterTable& cv_pars, const ParameterTable& univ_pars):iUniv(iuniv),cvPars(cv_pars),univPars(univ_pars){
    
  }
  
   TheoryThinTargetReweighter::~TheoryThinTargetReweighter(){
    
  }
  bool TheoryThinTargetReweighter::canReweight(const InteractionData& aa){
    
    if(aa.Proc.find("Inelastic")<40){
      if (aa.Vol == "TGT1") return false;
      if (aa.Vol == "BudalMonitor") return false;
      return true;
    }
    else return false;
    
  }
  
  double TheoryThinTargetReweighter::calculateWeight(const InteractionData& thisid){

   // factors[xf][pt] -> pt < 0.5, pt < 1.0, pt < 1.5 GeV

   static const double pipFactors[3][3] = {
      {0.100, 0.075, 0.025}, // xf < 0.25
      {0.100, 0.125, 0.100}, // xf < 0.50
      {0.075, 0.100, 0.025}  // xf < 0.75
   };

   static const double pimFactors[3][3] = {
      {0.125, 0.075, 0.025}, // xf < 0.25
      {0.100, 0.075, 0.150}, // xf < 0.50
      {0.025, 0.250, 0.100}  // xf < 0.75
   };

   static const double kapFactors[3][3] = {
      {0.125, 0.075, 0.200}, // xf < 0.25
      {0.175, 0.125, 0.125}, // xf < 0.50
      {0.200, 0.500, 0.300}  // xf < 0.75
   };

   static const double kamFactors[3][3] = {
      {0.125, 0.100, 0.050}, // xf < 0.25
      {0.025, 0.100, 0.175}, // xf < 0.50
      {0.350, 0.050, 0.300}  // xf < 0.75
   };

   std::map<std::string, double> dsig_table = univPars.table;
   double weight = dsig_table.find("inel_A_scaling")->second;

   double factor = 0.2;

   if (thisid.xF < 0.75)
   {
      if (thisid.Pt < 1.5)
      {
         const int iXf = thisid.xF > 0 ? thisid.xF / 0.25 : -thisid.xF / 0.25;
         const int iPt = thisid.Pt / 0.50;
	 
         switch (thisid.Prod_pdg)
         {
            case  211: factor = pipFactors[iXf][iPt]; break;
            case -211: factor = pimFactors[iXf][iPt]; break;
            case  321: factor = kapFactors[iXf][iPt]; break;
            case -321: factor = kamFactors[iXf][iPt]; break;
            default: break;
         }
      }
   }
   
   if (weight > 1.1) weight = 1.1;
   else if (weight < 0.9) weight = 0.9; 
   
   weight -= 1.0;
   weight *= factor / 0.1;
   weight += 1.0;
     
   return weight;
  }
  

}
