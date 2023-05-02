#ifndef PDG
#define PDG

class pdg {
public:
  static const int PIP = 211;
  static const int PIM = -PIP;
  static const int K0L = 130;
  static const int K0S = 310;
  static const int KP = 321;
  static const int KM = -KP;
  static const int N = 2112;
  static const int P = 2212;

  static const int C = 6;
  static const int AL = 13;
  static const int FE = 26;
  
  bool GetZ(int p) {
    /**
     * Extract Z number from pdg format: ±10LZZZAAAI.
     */  
    return (p/10000)%1000;
  }
};


#endif
