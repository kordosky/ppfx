
#include "dk2nu.h"
#include <cmath>
#include <iostream>

namespace bsim {
  
  NuRay::NuRay(){
    
  }
  NuRay::NuRay(double pxx, double pyy, double pzz, double EE, double thiswgt){
    px  = pxx;
    py  = pyy;
    pz  = pzz;
    E   = EE;
    wgt = thiswgt;
  }
  NuRay::~NuRay(){
    
  }
  void NuRay::clear(const std::string &opt){
    std::cout<<opt<<std::endl;
  }
  std::string NuRay::AsString(const std::string& opt) const{
    return opt;
  }
  Decay::Decay(){

  }
  Decay::~Decay(){

  }
  void Decay::clear(const std::string &opt){
    std::cout<<opt<<std::endl;
  }
  std::string Decay::AsString(const std::string& opt) const{
    return opt;
  }

  Ancestor::Ancestor(){

  }
  Ancestor::~Ancestor(){

  }
  void Ancestor::clear(const std::string &opt){
    std::cout<<opt<<std::endl;
  }
  std::string Ancestor::AsString(const std::string& opt) const{
    return opt;
  }

  void Ancestor::SetStartXYZT(Double_t x, Double_t y, Double_t z, Double_t t){
    startx = x;
    starty = y;
    startz = z;
    startt = t;
  }
  void Ancestor::SetStartP(Double_t px, Double_t py, Double_t pz){
    startpx = px;
    startpy = py;
    startpz = pz;
  }
  void Ancestor::SetStopP(Double_t px, Double_t py, Double_t pz){
    stoppx = px;
    stoppy = py;
    stoppz = pz;    
  }
  void Ancestor::SetPProdP(Double_t px, Double_t py, Double_t pz){
    pprodpx = px;
    pprodpy = py;
    pprodpz = pz;
    
  }
  Double_t Ancestor::r() const{
    return std::sqrt(startx*startx + starty*starty + startz*startz);
  }
  Double_t Ancestor::startpt() const{
    return sqrt(startpx*startpx + startpy*startpy);
  }
  Double_t Ancestor::startp() const{
    return sqrt(startpx*startpx + startpy*startpy + startpz*startpz);
  }
  Double_t Ancestor::stoppt() const{
    return sqrt(stoppx*stoppx + stoppy*stoppy);
  }
  Double_t Ancestor::stopp() const{
    return sqrt(stoppx*stoppx + stoppy*stoppy + stoppz*stoppz);
  }
  Double_t Ancestor::pprodpt() const{
    return sqrt(pprodpx*pprodpx + pprodpy*pprodpy);
  }
  Double_t Ancestor::pprodp() const{
    return sqrt(pprodpx*pprodpx + pprodpy*pprodpy + pprodpz*pprodpz);
  }
  
  TgtExit::TgtExit(){
    
  }
  TgtExit::~TgtExit(){

  }
  void TgtExit::clear(const std::string &opt){
    std::cout<<opt<<std::endl;
  }
  std::string TgtExit::AsString(const std::string& opt) const{
    return opt;
  }
  
  Traj::Traj(){

  }
  Traj::~Traj(){

  }
  void Traj::clear(const std::string &opt){
    std::cout<<opt<<std::endl;
  }
  std::string Traj::AsString(const std::string& opt) const{
    return opt;
  }
  
  Dk2Nu::Dk2Nu(){

  }
  Dk2Nu::~Dk2Nu(){

  }
  void Dk2Nu::clear(const std::string &opt){
    std::cout<<opt<<std::endl;
  }
  std::string Dk2Nu::AsString(const std::string& opt) const{
     return opt;
  }
  void Dk2Nu::Print(Option_t* option) const{
    std::cout<<option<<std::endl;
  }

  size_t  Dk2Nu::indxnu() const{
    // return size_t(ancestor.size()-1);
    return ancestor.size()-1;

  }
  size_t  Dk2Nu::indxp() const{
    return ancestor.size()-2;
  }
  size_t  Dk2Nu::indxgp() const{
    return ancestor.size()-3;
  }
  bool Dk2Nu::overflow() const{
    return false;
  }
  
}
