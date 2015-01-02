
#include "dkmeta.h"
#include "iostream"

ClassImp(bsim::Location)
ClassImp(bsim::DkMeta)

namespace bsim{

  Location::Location(){
    
  }
  Location::Location(double xx, double yy, double zz, std::string namedet){
    x = xx;
    y = yy;
    z = zz;
    name = namedet;    
  }
  Location::~Location(){
    
  }
  void Location::clear(const std::string &opt){
    std::cout<<opt<<std::endl;
  }
  std::string Location::AsString(const std::string& opt) const{
    return opt;
  }
  DkMeta::DkMeta(){
    
  }
  DkMeta::~DkMeta(){

  }
  void DkMeta::clear(const std::string &opt){
    std::cout<<opt<<std::endl;
  }
  std::string DkMeta::AsString(const std::string& opt) const{
    return opt;
  }
}
