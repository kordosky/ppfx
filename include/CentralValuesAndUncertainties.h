#ifndef CENTRALVALUESANDUNCERTAINTIES_H
#define CENTRALVALUESANDUNCERTAINTIES_H

#include <utility>
#include <map>
#include <vector>
#include "ParameterTable.h"
#include "TMatrixD.h"
#include "TVectorD.h"
#include <TDecompChol.h>
#include <TDecompSVD.h>
//#include "MatrixClass.h"

class TRandom3;

namespace NeutrinoFluxReweight{
  
  /*! \class CentralValuesAndUncertainties
   *  \brief A class to manage parameter central values and their uncertanities
   */
  class CentralValuesAndUncertainties{
  public:
    
    CentralValuesAndUncertainties();
    static CentralValuesAndUncertainties* getInstance();
    
    //! Read a xml file name to parse the parameters.
    void readFromXML(const char* filename);

    //! Add a parameter with its central value and its uncertainty. The parameter is specified as uncorrelated with all other parameters in the table, and any others that will be added. In that case, the uncertainty can be represented as a single floating point value.
    void addUncorrelated(Parameter& cv_par, double uncertainty);
    
    //!Add a set of parameters with correlated uncertainties. The central values of the parameters must be provided along with the covariance matrix of the parameters. These parameters are assumed to be block uncorrelated with all the other parameters.

    //    void addCorrelated(ParameterTable& cv_pars, MatrixClass& cov_mx);
    void addCorrelated(ParameterTable& cv_pars, TMatrixD& cov_mx);
    
    //!Set a beggining/base seed to be used in generating random parameter shifts for the many universe method. This base seed should be unique and not shared by any other many universe error calculator.
    void setBaseSeed(int val);

    //! Calculate a table of randomly varied parameters for a particular universe i. The universe number is used, along with the base seed, to calculate a unique seed for this universe, allowing our computation to be repeatable. We loop over all uncorrelated parameters and sample each from a normal distribution with mean=CV parameter and variance=sqrate of the uncertainty. We loop over all correlated sets of parameters. For each, we have a list of N central values x and a single joint NxN covariance matrix V. We execute a cholesky decomposition of V to find a lower left diagonal matrix L such that L L^T =V. Then we generate
    ParameterTable calculateParsForUniverse(int universe);
    
    //!Get the central value parameters.
    ParameterTable getCVPars();
    
  private:
    static CentralValuesAndUncertainties* instance;
    //for now commenting out:    ParameterTable& uncorrelated_pars;
    ParameterTable uncorrelated_pars;
    boost::interprocess::flat_map<std::string, double> uncorrelated_errors;
    std::vector<ParameterTable> correlated_par_tables;
    std::vector<TMatrixD> covariance_matrices;
    TRandom3 *r3;
    TRandom3 *r3_pip;   //BHUMIKA
    TRandom3 *r3_ch;   //BHUMIKA

    int baseSeed;
    
  };


  
}
#endif
