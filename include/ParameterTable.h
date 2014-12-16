#ifndef PARAMETERTABLE_H
#define PARAMETERTABLE_H
#include <vector>
#include <map>
#include <string>

namespace NeutrinoFluxReweight{
  
  /*! \typedef typedef std::pair<std::string, double> Parameter;
    The name and value of a single parameter used in cv reweighting, many universes uncertainty calculations, or both.
  */
  typedef std::pair<std::string, double> Parameter;
  
  
  /*! \class ParameterTable
   * \brief A list/table of parameter names and values. At its simplest this is just a map, but some additinoal features might be nice.
  */
  class ParameterTable{
  public:
    //! add a parameter to the table or, if already there, reset its value
    void setParameter(Parameter p); 
    //! get a parameter by name. throw an exception of a well defined type if we don't have it
    Parameter getParameter(const std::string& name);
    //! get the value of a parameter. throw an exception of a well defined type if we don't have it
    double getParameterValue(const std::string& name);
    //! is the named parameter in the table?
    bool hasParameter(const std::string& name);
    //! get the parameter table
    std::map<std::string, double> table;
  };
  
};
#endif
