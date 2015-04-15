#include "MakeReweight.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace NeutrinoFluxReweight{

  MakeReweight::MakeReweight(std::string fileIn): fileOptions(fileIn){
    ParseOptions();
    Initialize();
  }
  void MakeReweight::ParseOptions(){
    //Parsing the file input:
    using boost::property_tree::ptree;
    ptree top;
    read_xml(fileOptions.c_str(),top,2); // option 2 removes comment strings
    ptree& options = top.get_child("inputs.Settings");

    mippCorrOption = options.get<std::string>("MIPPCorrOption");
    Nuniverses = options.get<int>("NumberOfUniverses");
    
  }
  void MakeReweight::Initialize(){
    
    //Getting MIPP binning and the correlation parameters:
    CentralValuesAndUncertainties* cvu = CentralValuesAndUncertainties::getInstance();;
    MIPPNumiYieldsBins*  myb =  MIPPNumiYieldsBins::getInstance(); 
    const char* ppfxDir = getenv("PPFX_DIR");
    
    std::cout<<"Initializing correlation parameters"<<std::endl;
    cvu->readFromXML(Form("%s/uncertainties/Parameters_%s.xml",ppfxDir,mippCorrOption.c_str()));
    myb->readPIP_FromXML(Form("%s/data/BINS/MIPPNumiData_PIP_Bins.xml",ppfxDir));
    myb->readPIM_FromXML(Form("%s/data/BINS/MIPPNumiData_PIM_Bins.xml",ppfxDir));
    //Reweighter drivers:
    vec_rws.reserve(Nuniverses);
    std::cout<<"Initializing reweight drivers for "<<Nuniverses<<" universes"<<std::endl;
 
    const int base_universe=1000000;
    for(int ii=0;ii<Nuniverses;ii++){
      ParameterTable cvPars=cvu->getCVPars();
      ParameterTable univPars=cvu->calculateParsForUniverse(ii+base_universe);
      vec_rws.push_back(new ReweightDriver(ii,cvPars,univPars,fileOptions));  
      vec_wgts.push_back(1.0);
    }
    
    std::cout<<"Done configuring universes"<<std::endl;

  }
  
  std::vector<double> MakeReweight::GetTotalWeights(){
    return vec_wgts;
  }
  /////
  void MakeReweight::calculateWeights(nu_g4numi* nu, const char* tgtcfg, const char* horncfg){
    InteractionChainData* inter_chain = new InteractionChainData(nu,tgtcfg,horncfg);
    doTheJob(inter_chain);
  }
  /////
  void MakeReweight::calculateWeights(bsim::Dk2Nu* nu, bsim::DkMeta* meta){  
    InteractionChainData* inter_chain = new InteractionChainData(nu,meta);
    doTheJob(inter_chain);
  }
  /////
  void MakeReweight::doTheJob(InteractionChainData* icd){
  
    map_rew_wgts.clear();

    for(int ii=0;ii<Nuniverses;ii++){
      vec_wgts[ii] = vec_rws[ii]->calculateWeight(*icd);
      map_rew_wgts["MIPPNumiYields"].push_back(vec_rws[ii]->mipp_wgt);
      map_rew_wgts["TargetAttenuation"].push_back(vec_rws[ii]->att_wgt);
      map_rew_wgts["AbsorptionIC"].push_back(vec_rws[ii]->abs_ic_wgt);
      map_rew_wgts["AbsorptionDPIP"].push_back(vec_rws[ii]->abs_dpip_wgt);
      map_rew_wgts["AbsorptionDVOL"].push_back(vec_rws[ii]->abs_dvol_wgt);
      map_rew_wgts["AbsorptionNucleon"].push_back(vec_rws[ii]->abs_nucleon_wgt);
      map_rew_wgts["AbsorptionOther"].push_back(vec_rws[ii]->abs_other_wgt);
      map_rew_wgts["MIPPNumiKaonsYields"].push_back(vec_rws[ii]->mipp_kaons_wgt);
      map_rew_wgts["NA49"].push_back(vec_rws[ii]->na49_wgt);
      map_rew_wgts["MIPPThinTarget"].push_back(vec_rws[ii]->mipp_thin_wgt);
      map_rew_wgts["TheoryThinTarget"].push_back(vec_rws[ii]->theory_wgt);
      
    }
  }
  ////
  std::vector<double> MakeReweight::GetWeights(std::string nameReweighter){

    std::vector<double> tmp_vec;
    std::map<std::string,std::vector<double> >::iterator it = map_rew_wgts.begin();
    it = map_rew_wgts.find(nameReweighter);
    if(it!=map_rew_wgts.end())tmp_vec = it->second;
    
    return tmp_vec;
   
  }
  ////
  int  MakeReweight::GetNumberOfUniversesUsed(){
    return Nuniverses;
  }
  ////
}
