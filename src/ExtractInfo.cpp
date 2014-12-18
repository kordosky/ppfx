
#include "ExtractInfo.h"

namespace NeutrinoFluxReweight{

  //////  
  ExtractInfo* ExtractInfo::instance = 0;
  
  /////
  ExtractInfo::ExtractInfo(){
    
  }
  
  //////
  ExtractInfo::~ExtractInfo(){
  }
  ///
  void ExtractInfo::FillInfo(std::string name,double val){
    map_info[name] = val;
  }
  //////
  std::map<std::string,double> ExtractInfo::GetInfo(){
    std::map<std::string,double> this_map;
    this_map = map_info;
    return this_map;
  }

  /////
  void ExtractInfo::CleanInfo(){
    map_info.clear();
  }

  ///////
  ExtractInfo* ExtractInfo::getInstance(){
    if (instance == 0) instance = new ExtractInfo;
    return instance;
  }


  std::ostream& ExtractInfo::Print(std::ostream& os){
    std::map<std::string,double>::iterator it=map_info.begin();
    os<<"===== ExtractInfo ======="<<std::endl;
    for(; it!=map_info.end(); it++){
      os<<it->first<<" : "<<it->second<<std::endl;
    }
    return os;
  }  
};
