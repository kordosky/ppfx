
#ifndef NUMI2PDG_H
#define NUMI2PDG_H

/*! Convert numi particle id number to pdg codes */
class Numi2Pdg
{  

 public:

  Numi2Pdg();
  virtual ~Numi2Pdg();

  /*! Get Pdg code from Numi codes(Fluka?) */ 
  int GetPdg(int numipart);     

};

#endif
