
#include "ParameterTable.h"
#include "Exceptions.h"

namespace NeutrinoFluxReweight{ 
  
  void ParameterTable::setParameter(Parameter p){

    table[p.first] = p.second;
    
  }
  Parameter ParameterTable::getParameter(const std::string& name){
    
    Parameter pOut;
    std::map<std::string, double>::iterator it;
    it=table.find(name);
    
    if(it!=table.end()){
    pOut.first  = name;
    pOut.second = it->second;
    }
    else{
      throw NoParameterFound();
    }
    return pOut;
    
  }
  double ParameterTable::getParameterValue(const std::string& name){

    double val;
    std::map<std::string, double>::iterator it;
    it=table.find(name);
    
    if(it!=table.end()){
      val = it->second;
    }
  else{
    throw NoParameterFound();
  }
    
    return val;
    
  }
  bool ParameterTable::hasParameter(const std::string& name){

    bool val;
    
    std::map<std::string, double>::iterator it;
    it=table.find(name);
    
    if(it!=table.end()){
      val = true;
    }
    else{
      val = false;
    }
    return val;
    
  }  
};
