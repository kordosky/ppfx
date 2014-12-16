
#ifndef EXTRACTINFO_H
#define EXTRACTINFO_H

#include <map>
#include <string>

namespace NeutrinoFluxReweight{
  
  class ExtractInfo{
  public:
    
    ExtractInfo();
    ~ExtractInfo();
    static ExtractInfo* getInstance();
    void FillInfo(std::string name,double val);
    std::map<std::string,double> GetInfo();
    void CleanInfo();
    
  private:
    static ExtractInfo* instance;
    std::map<std::string,double> map_info;

    
  };
  
};

#endif 
