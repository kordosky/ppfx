
#ifndef EXTRACTINFO_H
#define EXTRACTINFO_H

#include <map>
#include <string>
#include <iostream>

namespace NeutrinoFluxReweight{
  /*! \class ExtractInfo
   * \brief Tabulates and stores information useful for debugging the reweighting code.
   */
  class ExtractInfo{
  public:
    
    ExtractInfo();
    ~ExtractInfo();
    static ExtractInfo* getInstance();
    void FillInfo(std::string name,double val);
    std::map<std::string,double> GetInfo();
    void CleanInfo();
    std::ostream& Print(std::ostream& os);

  private:
    static ExtractInfo* instance;
    std::map<std::string,double> map_info;

    
  };
  
};

#endif 
