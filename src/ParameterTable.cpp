
#include <algorithm>
#include "ParameterTable.h"
#include "Exceptions.h"

namespace NeutrinoFluxReweight{ 
  
  void ParameterTable::setParameter(Parameter p){
    if(m_vectorMode){
      m_vector.push_back(p);
    }
    else{
      table[p.first] = p.second;
    }
  }

  Parameter ParameterTable::getParameter(const std::string& name) const {
    mapify();
    Parameter pOut;
    boost::interprocess::flat_map<std::string, double>::const_iterator it=table.find(name);
    
    if(it!=table.end()){
      pOut.first  = name;
      pOut.second = it->second;
    }
    else{
      throw NoParameterFound(name);
    }
    return pOut;
    
  }

  double ParameterTable::getParameterValue(const std::string& name) const {
    mapify();

    double val;
    boost::interprocess::flat_map<std::string, double>::const_iterator it=table.find(name);
    
    if(it!=table.end()){
      val = it->second;
    }
    else{
      throw NoParameterFound(name);
    }
    
    return val;
    
  }

  bool ParameterTable::hasParameter(const std::string& name) const {
    mapify();
    return table.find(name)!=table.end();
  }  

  void ParameterTable::mapify() const
  {
    // If we're already out of vector mode, there's nothing to do
    if(!m_vectorMode) return;

    // Sort the vector, populate the flat_map from it, and then clear
    // the vector
    std::sort(m_vector.begin(), m_vector.end());
    table=boost::interprocess::flat_map<std::string, double>(m_vector.begin(), m_vector.end());
    m_vector.clear();
    // Magic to force the memory to be released: clear() doesn't do that
    std::vector<std::pair<std::string, double> >().swap(m_vector);
    // We're no longer in vector mode, so we won't run this function
    // again
    m_vectorMode=false;
  }

}
