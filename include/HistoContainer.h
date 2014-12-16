
#ifndef HISTOCONTAINER_H
#define HISTOCONTAINER_H

#include "TH1D.h"
#include "TH2D.h"
#include <map>
#include <string>

namespace NeutrinoFluxReweight{
  
  class HistoContainer{
  public:
    
    HistoContainer();
    ~HistoContainer();
    static HistoContainer* getInstance();
    void plot1D(double val, std::string hname,std::string htitle, int Nbins,double minval,double maxval,double histwgt);
    void plot2D(double val, std::string hname,std::string htitle, int NbinsX,double minvalX,double maxvalX, int NbinsY,double minvalY,double maxvalY,double histwgt);

    std::map<std::string,TH1D*> Get1DHistos();
    std::map<std::string,TH2D*> Get2DHistos();

  private:
    static HistoContainer* instance;
    std::map<std::string,TH1D*> map_1D;
    std::map<std::string,TH2D*> map_2D;
    
  };
  
};

#endif 
