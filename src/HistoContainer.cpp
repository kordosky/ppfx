
#include "HistoContainer.h"

namespace NeutrinoFluxReweight{

  //////  
  HistoContainer* HistoContainer::instance = 0;
  
  /////
  HistoContainer::HistoContainer(){
    
  }
  
  //////
  HistoContainer::~HistoContainer(){ 
  }
  
  //////
  void HistoContainer::plot1D(double val,std::string hname,std::string htitle, int Nbins,double minval,double maxval,double histwgt){
    std::map<std::string,TH1D*>::iterator it = map_1D.begin();
    it = map_1D.find(hname);
    if(it!=map_1D.end()){
      (it->second)->Fill(val,histwgt);
    }
    else{
      TH1D* hnew = new TH1D(hname.c_str(),htitle.c_str(),Nbins,minval,maxval);
      map_1D[hname] = hnew;
      hnew->Fill(val,histwgt);
    }

  }

  /////
  void HistoContainer::plot2D(double val,std::string hname,std::string htitle, int NbinsX,double minvalX,double maxvalX, int NbinsY,double minvalY,double maxvalY,double histwgt){
    
    std::map<std::string,TH2D*>::iterator it = map_2D.begin();
    it = map_2D.find(hname);
    if(it!=map_2D.end()){
      (it->second)->Fill(val,histwgt);
    }
    else{
      TH2D* hnew = new TH2D(hname.c_str(),htitle.c_str(),NbinsX,minvalX,maxvalX,NbinsY,minvalY,maxvalY);
      map_2D[hname] = hnew;
      hnew->Fill(val,histwgt);
    }

  }

  /////
  std::map<std::string,TH1D*> HistoContainer::Get1DHistos(){
    std::map<std::string,TH1D*> this_histos = map_1D;
    return this_histos;
  }

  //////
  std::map<std::string,TH2D*> HistoContainer::Get2DHistos(){
    std::map<std::string,TH2D*> this_histos = map_2D;
    return this_histos;
  }

  ///////
  HistoContainer* HistoContainer::getInstance(){
    if (instance == 0) instance = new HistoContainer;
    return instance;
  }
  
};
