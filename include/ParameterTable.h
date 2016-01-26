#ifndef PARAMETERTABLE_H
#define PARAMETERTABLE_H
// This lives in boost/containers/flat_map.hpp in versions of boost
// less ancient than the one in the Minerva software
#include <boost/interprocess/containers/flat_map.hpp>
#include <vector>
#include <map>
#include <string>

namespace NeutrinoFluxReweight{
  
  /*! \typedef typedef std::pair<std::string, double> Parameter;
    The name and value of a single parameter used in cv reweighting, many universes uncertainty calculations, or both.
  */
  typedef std::pair<std::string, double> Parameter;
  
  
  /*! \class ParameterTable
   * \brief A list/table of parameter names and values.
   *
   * The memory overhead of std::map turns out to be large for our
   * case, so we use a boost::flat_map instead. This uses an ordered
   * vector, so memory overhead is small. Insertion is slow, though,
   * so we start out by putting all our parameters in a vector. At the
   * first request for parameter or the entire map, we sort the
   * vector, copy it into the map and free the vector. This is
   * efficient when there is a "filling" phase when no reads are done,
   * and a "reading" phase, when no more fills are done.
  */
  class ParameterTable{
  public:
    ParameterTable()
      : m_vectorMode(true)
      {}
    
    //! add a parameter to the table or, if already there, reset its value
    void setParameter(Parameter p); 
    //! get a parameter by name. throw an exception of a well defined type if we don't have it
    Parameter getParameter(const std::string& name) const;
    //! get the value of a parameter. throw an exception of a well defined type if we don't have it
    double getParameterValue(const std::string& name) const;
    //! is the named parameter in the table?
    bool hasParameter(const std::string& name) const;

    const boost::interprocess::flat_map<std::string, double>& getMap() const { mapify(); return table; }
  protected:

    //! Move the parameters from the vector to the map
    void mapify() const;
    mutable boost::interprocess::flat_map<std::string, double> table;
    mutable std::vector<std::pair<std::string, double> > m_vector;
    mutable bool m_vectorMode;
  };
  
};
#endif
